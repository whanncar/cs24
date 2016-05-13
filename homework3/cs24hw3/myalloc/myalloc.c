#include <stdio.h>
#include <stdlib.h>

#include "myalloc.h"


int MEMORY_SIZE;
unsigned char *mem;


/*
 * Each block of memory (whether free or allocated) has a header.
 *
 * The header includes:
 *
 *  + The size of the block (excluding the header)
 *  + A pointer to the header of the adjacent block to the left
 *  + A pointer to the header of the adjacent block to the right
 *
 * The pointers allow for implementation of a doubly linked list.
 *
 */
typedef struct header {

    int size;
    struct header *left;
    struct header *right;

} block;

block *get_best_fit(int);
void split(block *, int);
void coalesce(block *);
void set_free(block *);
void set_not_free(block *);
int is_free(block *);

/*
 * The memory is initialized with two blocks.
 *
 * The first block ("root") is set to "not free" (ie allocated), so
 * that it will always persist. The second block ("first_free_block")
 * is set to free.
 *
 * The size of first_free_block is the largest memory request that
 * could the memory could theoretically support.
 *
 * The doubly linked list structure is initialized by having root
 * and first_free_block point to each other in both directions.
 *
 * 
 */

void init_myalloc() {

    mem = (unsigned char *) malloc(MEMORY_SIZE);

    block *root = (block *) mem;
    block *first_free_block = (block *) (mem + sizeof(block));

    root->size = -1;
    set_not_free(root);

    first_free_block->size = MEMORY_SIZE - 2 * sizeof(block);
    set_free(first_free_block);

    root->left = first_free_block;
    root->right = first_free_block;

    first_free_block->left = root;
    first_free_block->right = root;

}


/*
 * To allocate, myalloc calls get_best_fit to find the
 * best fitting free block for the requested size.
 *
 * If the best fitting free block is large enough to
 * provide the space for the request and at least store
 * store a header afterwards, then the block is split,
 * and otherwise it is set to free.
 *
 */


unsigned char *myalloc(int size) {

    block *b = get_best_fit(size);

    if (mem == ((unsigned char *) b)) {
        return ((unsigned char *) 0);	
    }
	
    if (b->size >= (size + sizeof(block))) {
        split(b, size);	
    }
    else {
        set_not_free(b);
    }

    return (((unsigned char *) b) + sizeof(block));

}

/*
 * myfree sets the allocated block to free, then
 * coalesces the block with neighboring free blocks
 * if needed.
 */

void myfree(unsigned char *oldptr) {

    block *b = (block *) (oldptr - sizeof(block));
    set_free(b);

    coalesce(b);

}

/*
 * get_best_fit iterates through the linked list of blocks and
 * finds the smallest free block which is at least as large as
 * the requested length.
 * If such a block is found, a pointer to its header is returned;
 * otherwise, a pointer to the beginning of memory is returned.
 *
 * get_best_fit is linear in the number of allocated
 * blocks plus the number of free blocks.
 */

block *get_best_fit(int length) {

    block *best_fit = (block *) mem;
    int best_fit_size = -1;

    block *current_block = (block *) mem;
    current_block = current_block->right;
	
    while (current_block != ((block *) mem)) {
        if (is_free(current_block) && (current_block->size >= length)) {
            if (current_block->size < best_fit_size || best_fit_size == -1) {
                best_fit = current_block;
                best_fit_size = current_block->size;		
            }	
        }
        current_block = current_block->right;	
    }
	
    return best_fit;

}
 
/*
 * split shortens the given free block to the requested length,
 * sets it to not free, and creates a new free block with the
 * leftover space.
 * 
 * split runs in constant time.
 *
 */
 
void split(block *b, int length) {

    block *new_free_block = (block *)(((unsigned char *) b) + (sizeof(block) + length));

    /* Set the values of new_free_block */
    new_free_block->size = b->size - length - sizeof(block);
    new_free_block->left = b;
    new_free_block->right = b->right;

    /* Make other blocks point at new_free_block */
    new_free_block->right->left = new_free_block;
    b->right = new_free_block;

    set_free(new_free_block);
	
    b->size = length;
    set_not_free(b);

}

/*
 * coalesce checks the neighbors of the given newly free block, then
 * replaces the block and its free neighbors with a single large block.
 *
 * coalesce runs in constant time.
 */

void coalesce(block *b) {

    if (is_free(b->left)) {

        /* Update left's size */
        b->left->size += b->size + sizeof(block);

        /* Update linked list */
        b->left->right = b->right;
        b->left->right->left = b->left;

        /* Switch b to b->left */
        b = b->left;
    }

    if (is_free(b->right)) {

        /* Update b's size */
        b->size += b->right->size + sizeof(block);

        /* Update linked list */
        b->right = b->right->right;
        b->right->left = b;

    }

}



void set_free(block *b) {

    if (b->size < 0) {

        b->size = -(b->size);

    }

}

void set_not_free(block *b) {

    if (b->size > 0) {

        b->size = -(b->size);

    }

}

int is_free(block *b) {

    return (int) (b->size >= 0);

}
