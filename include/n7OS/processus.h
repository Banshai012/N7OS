#ifndef __PAGING_H__
#define __PAGING_H__

#include <inttypes.h>
#include <n7OS/mem.h>

#define STACK_SIZE PAGE_SIZE
#define NB_PROC_MAX 255

typedef enum {ELU,PRET,BLOQUE} process_state;

typedef uint32_t process_id;

typedef void* (*fnptr)();

typedef struct
{
	char *name;
	uint32_t *stack;
	process_state state;
    uint32_t prio;
    process_id ppid;
	process_id pid;
	uint32_t regs[5];
} process_t;

process_id find_free_pid();

void clear_pid(process_id pid);

int get_pid(process_id pidx);

process_id get_pid_elu();

process_id spawn(const char *name, void* main);

process_id fork(const char *name, fnptr function);

int sleep(int seconds);

void terminate_processus(process_id pidx);

void block_process(process_id pidx);

void unbloc_process(process_id pidx);

void stop_process(process_id pidx);

void start_process(process_id pidx);

void scheduler();

void init_process();

#endif // _PROCESSUS_H