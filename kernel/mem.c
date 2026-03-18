#include <n7OS/mem.h>

#define NBPAGE 4096
#define SIZE (NBPAGE / 32)

int nbpage;
int size;
uint32_t free_page_bitmap_table[SIZE];

/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    uint32_t page_index = addr / nbpage;
    uint32_t array_index = page_index / 32;
    uint32_t bit_index = page_index % 32;
    free_page_bitmap_table[array_index] |= (1 << bit_index);
}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    uint32_t page_index = addr / nbpage;
    uint32_t array_index = page_index / 32;
    uint32_t bit_index = page_index % 32;
    free_page_bitmap_table[array_index] &= ~(1 << bit_index);
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
    uint32_t page_index = 0;
    uint32_t adresse= 0x0;
    uint32_t libre=1;
    
    while (libre && page_index < nbpage){
        uint32_t array_index = page_index / 32;
        uint32_t bit_index = page_index % 32;
        if (free_page_bitmap_table[array_index] & (1 << bit_index) == 0)
        {
            libre=0;
            free_page_bitmap_table[array_index] |= (1 << bit_index);
            adresse = page_index * size;
        }
        else{
            page_index++;
        }
    }
    return adresse;
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    nbpage = NBPAGE;
    size = SIZE;

    // 3. En C, pour vider un tableau dans une fonction, on utilise une boucle
    for (int i = 0; i < size; i++) {
        free_page_bitmap_table[i] = 0; // 0 signifie "page libre"
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    
}