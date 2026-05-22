#include <n7OS/cpu.h>
#include <n7OS/processus.h>
#include <n7OS/time.h>

uint32_t free_processus_bitmap_table[NB_PROC_MAX/32];
process_t free_processus_table[NB_PROC_MAX];

extern void ctx_sw(void *ctx_old, void *ctx_nex);
extern void idle();
extern void fork2();

process_id find_free_pid() {
    uint32_t page_index = 0;
    uint32_t adresse= 0;
    uint32_t libre=1;
    
    while (libre && page_index < NB_PROC_MAX){
        uint32_t array_index = page_index / 32;
        uint32_t bit_index = page_index % 32;
        if ((free_processus_bitmap_table[array_index] & (1 << bit_index)) == 0) {
            libre=0;
            free_processus_bitmap_table[array_index] |= (1 << bit_index);
            adresse = page_index;
        }
        else{
            page_index++;
        }
    }
    return adresse;
}

void clear_pid(process_id pid) {
    uint32_t array_index = pid / 32;
    uint32_t bit_index = pid % 32;
    free_processus_bitmap_table[array_index] &= ~(1 << bit_index);
}

int get_pid(process_id pidx) {
	uint32_t offset = pidx % 32;
	return (free_processus_bitmap_table[pidx / 32] & (1 << offset)) >> offset == 1;
}

process_id get_pid_elu() {
    for (int i = 0; i < NB_PROC_MAX; i++) {
        if (free_processus_table[i].state == ELU) {
            return i;
        }
    }
    return -1;
}

process_id spawn(const char *name, void* main) {
    process_id pid = find_free_pid();
    free_processus_table[pid].pid = pid;
    free_processus_table[pid].name = name;
    free_processus_table[pid].state = PRET;
	free_processus_table[pid].stack = (uint32_t *)malloc(sizeof(uint32_t) * STACK_SIZE);
	for (int i = 0; i < 5; i++) {
		free_processus_table[pid].regs[i] = 0;
    }
	free_processus_table[pid].stack[STACK_SIZE - 1] = (uint32_t)main;
	free_processus_table[pid].regs[1] = (uint32_t)&free_processus_table[pid].stack[STACK_SIZE - 1];
	return pid;
}

process_id fork(const char *name, fnptr function) {
    process_id pid = find_free_pid();
    free_processus_table[pid].ppid = get_pid_elu();
    free_processus_table[pid].pid = pid;
    free_processus_table[pid].name = name;
    free_processus_table[pid].state = PRET;
	free_processus_table[pid].stack = (uint32_t *)malloc(sizeof(uint32_t) * STACK_SIZE);
	for (int i = 0; i < 5; i++) {
		free_processus_table[pid].regs[i] = 0;
    }
	free_processus_table[pid].stack[STACK_SIZE - 1] = (uint32_t)function;
	free_processus_table[pid].regs[1] = (uint32_t)&free_processus_table[pid].stack[STACK_SIZE - 1];
	return pid;
}

int sleep(int seconds) {
    int t = get_time();
    while (seconds - ((t % 60000) / 1000) >= 0) {
        t = get_time();
    }
    return 1;
}

void terminate_processus(process_id pidx) {
	if (!get_pid(pidx))
		return;
	free((void *)free_processus_table[pidx].stack);
	clear_pid(pidx);
}

void block_process(process_id pidx) {
    if (!get_pid(pidx))
		return;
    free_processus_table[pidx].state = BLOQUE;
}

void unbloc_process(process_id pidx) {
    if (!get_pid(pidx))
		return;
    free_processus_table[pidx].state = PRET;
}

void stop_process(process_id pidx) {
    if (!get_pid(pidx))
		return;
    free_processus_table[pidx].state = PRET;
}

void start_process(process_id pidx) {
    if (!get_pid(pidx))
		return;
    free_processus_table[pidx].state = ELU;
}

void scheduler() {
    uint32_t pid = get_pid_elu();
	for (int32_t idx = 0; idx < NB_PROC_MAX; idx++) {
        uint32_t new_pid = (pid + idx) % NB_PROC_MAX;
		if (get_pid(new_pid) && free_processus_table[new_pid].state == PRET) {
            printf("scheduler en action");
			stop_process(pid);
			start_process(new_pid);

			// printf("Switching to process %d : %s\n", new_pid, process_table[new_pid].name);
			sti();
            ctx_sw(free_processus_table[pid].regs, free_processus_table[new_pid].regs);
			return;
		}
	}
}


void init_process() {
    for (int i = 0; i < NB_PROC_MAX/32; i++) {
        free_processus_bitmap_table[i] = 0;
    }
    free_processus_table[spawn("idle", *idle)].state = ELU;
    free_processus_table[spawn("fork2", *fork2)].state = PRET;
    idle();
}