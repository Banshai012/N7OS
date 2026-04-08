#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL
#include <n7OS/mem.h>

PDE *page_directory = NULL;

PDE* initialise_paging() {
    init_mem();
    page_directory= (PDE *) kmalloc_a(sizeof(PDE) * 1024);
    for (size_t i = 0; i < 1024; i++) {
        page_directory[i].dir_entry.P = 1;
        page_directory[i].dir_entry.W = 1;
        page_directory[i].dir_entry.U = 0;
    }

    for (uint32_t i = 0; i < 0x400000; i += 0x1000) {
        alloc_page_entry(i, 1, 1);
    }
    
    __asm__ __volatile__("mov %0, %%cr3"::"r"(page_directory));
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x1F;
    __asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));

    return page_directory;
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    PageTable pgtab= NULL;
    uint32_t dir_index = address >> 22; 
    uint32_t table_index = (address >> 12) & 0x03FF;
    if (page_directory[dir_index].dir_entry.P == 0) {
        PageTable page_table = (PageTable) kmalloc_a(sizeof(PTE) * 1024);
        for (size_t i = 0; i < 1024; i++) {
            page_table[i].value = 0;
        }
        page_directory[dir_index].dir_entry.PAGE = ((uint32_t)page_table) >> 12;
        page_directory[dir_index].dir_entry.P = 1; 
        page_directory[dir_index].dir_entry.W = 1;
        page_directory[dir_index].dir_entry.U = 1;
    }
    pgtab = (PageTable) (page_directory[dir_index].dir_entry.PAGE << 12);
    if (pgtab[table_index].page_entry.P == 0) {
        uint32_t phys_page_addr = findfreePage(); 
        pgtab[table_index].page_entry.PAGE = phys_page_addr >> 12;
        pgtab[table_index].page_entry.P = 1;
        pgtab[table_index].page_entry.W = is_writeable;
        pgtab[table_index].page_entry.U = is_kernel;
    }
    return pgtab;
}
