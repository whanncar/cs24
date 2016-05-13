

/* START value structs */

typedef struct _value_chunk {

    int size;
    int max_size;

    struct _value_chunk *next;

    /* Implicit array */

} value_chunk;


typedef struct {

    value_chunk *first;
    value_chunk *last;

} value_list;

/* END value structs */


/* START key structs */

typedef struct {

    int key;

    value_list *vals;

} key_value_set;


typedef struct {

    int size;
    int max_size;

    /* Implicit array */

} small_tree;


typedef struct {

    int level;
    int size;
    int max_size;

    int min;
    int max;

    /* Implicit array */

} big_tree;


typedef struct {

    big_tree *root;

} multimap;

/* END key structs */



multimap *init_multimap() {

    multimap *mm;

    mm = (multimap *) malloc(sizeof(multimap));

    mm->root = (big_tree *) malloc(sizeof(big_tree) /* + other stuff UNRESOLVED */);

    mm->root->level = 1;
    mm->root->size = 0;
    mm->root->max_size = 64; /* UNRESOLVED */

}


void clear_multimap(multimap *mm) {

    clear_big_tree(mm->root);

    free(mm->root);

}


void clear_big_tree(big_tree *tree) {

    int i;

    big_tree *big_tree_to_free;
    small_tree *small_tree_to_free;

    if (tree->level > 1) {

        for (i = 0; i < tree->size; i++) {
            big_tree_to_free = *(((big_tree **) (tree + 1)) + i);
            clear_big_tree(big_tree_to_free);
            free(big_tree_to_free);
        }

    }

    else {

        for (i = 0; i < tree->size; i++) {
            small_tree_to_free = *(((small_tree **) (tree + 1)) + i);
            free(small_tree_to_free);
        }

    }

}



void mm_add_value(multimap *mm, int key, int val) {

    key_value_set *k_v_set;

    k_v_set = get_key_value_set_from_multimap_by_key(mm, key);

    if (k_v_set == NULL) {

        k_v_set = add_key_to_multimap(mm, key);

    }

    add_value_to_key_value_set(k_v_set, val);
}


key_value_set *add_key_to_multimap(multimap *mm, int key) {

    if (!add_key_to_big_tree(mm->root, key)) {

        /* Finish implementing UNRESOLVED */

    }

    return rectify_new_key_position(mm);

}


key_value_set *rectify_new_key_position(multimap *mm) {

    /* Implement UNRESOLVED */

}


int add_key_to_big_tree(big_tree *tree, int key) {

    if (tree->level > 1) {
        return add_key_to_big_tree(*(((big_tree **) (tree + 1)) + tree->size - 1), key);
    }

    else {
        return add_key_to_small_tree(*(((small_tree **) (tree + 1)) + tree->size - 1), key);
    }

}


int add_key_to_small_tree(small_tree *tree, int key) {

    key_value_set *new_k_v_set;

    if (tree->size == tree->max_size) {
        return 0;
    }

    else {

        new_k_v_set = ((key_value_set *) (tree + 1)) + tree->size;

        new_k_v_set->key = key;

        new_k_v_set->vals = new_value_list();

        tree->size++;

        return 1;

    }

}


key_value_set *get_key_value_set_from_multimap_by_key(multimap *mm, int key) {

    return get_key_value_set_from_big_tree_by_key(mm->root, key);

}


key_value_set *get_key_value_set_from_big_tree_by_key(big_tree *tree, int key) {

    int i;

    big_tree **big_tree_to_search_ptr;
    small_tree *small_tree_to_search;

    if (tree->level > 1) {

        for (i = 0, big_tree_to_search_ptr = (big_tree **) (tree + 1);
             i < tree->size;
             i++, big_tree_to_search_ptr++) {

            if ((*big_tree_to_search_ptr)->min <= key &&
                key <= (*big_tree_to_search_ptr)->max) {

                return get_key_value_set_from_big_tree_by_key(
                                        *big_tree_to_search_ptr, key);

            }

        }

    }

    else {

        for (i = 0, small_tree_to_search_ptr = (small_tree **) (tree + 1);
             i < tree->size;
             i++, small_tree_to_search_ptr++) {

            if ((*small_tree_to_search_ptr)->min <= key &&
                key <= (*small_tree_to_search_ptr)->max) {

                return get_key_value_set_from_small_tree_by_key(
                                        *small_tree_to_search_ptr, key);

            }

        }

    }

    return NULL;

}


key_value_set *get_key_value_set_from_small_tree_by_key(small_tree *tree, int key) {

    int offset;

    key_value_set *key_arr;

    key_arr = (key_value_set *) (tree + 1) - 1;

    for (offset = 1; offset < tree->size;) {

        if (key < (key_arr + offset)->key) {

            offset *= 2;

        }

        else if (key > (key_arr + offset)->key) {

            offset *= 2;
            offset++;

        }

        else {

            return (key_arr + offset);

        }

    }

    return NULL;

}



void add_value_to_key_value_set(key_value_set *k_v_set, int val) {

    add_value_to_value_list(k_v_set->vals, val);
}



int mm_contains_key(multimap *mm, int key) {

    return (get_key_value_set_from_multimap_by_key(mm, key) != NULL);

}


int mm_contains_pair(multimap *mm, int key, int value) {

    key_value_set *k_v_set;

    k_v_set = get_key_value_set_from_multimap_by_key(mm, key);

    if (k_v_set == NULL) {
        return 0;
    }

    return value_list_contains_value(k_v_set-> vals, value);

}



void mm_traverse(multimap *mm, void (*f)(int key, int value)) {

    big_tree_traverse(mm->root, f);

}


void big_tree_traverse(big_tree *tree, void (*f)(int key, int value)) {

    int i;

    if (level > 1) {

        for (i = 0; i < tree->size; i++) {
            big_tree_traverse(*(((big_tree **) (tree + 1)) + i), f);
        }

    }

    else {

        for (i = 0; i < tree->size; i++) {
            small_tree_traverse(*(((small_tree **) (tree + 1)) + i), f);
        }

    }

}


void small_tree_traverse(small_tree *tree, void (*f)(int, int)) {

    int i, j;

    int *value_ptr;

    key_value_set *k_v_set;

    value_chunk *chunk;

    int key;

    for (i = 0, k_v_set = ((k_value_set *) (tree + 1));
         i < tree->size;
         i++, k_v_set++) {

        key = k_v_set->key;

        for (chunk = k_v_set->vals->first;
             chunk != NULL;
             chunk = chunk->next) {

            for (j = 0, value_ptr = (int *) (chunk + 1);
                 j < chunk->size;
                 j++, value_ptr++) {

                f(key, *value_ptr);

            }

        }

    }

}



value_list *new_value_list() {

    value_list *new_list;
    value_chunk *initial_chunk;

    /* Allocate space for the list */
    new_list = (value_list *) malloc(sizeof(value_list));
    initial_chunk = (value_chunk *) malloc(sizeof(value_chunk));

    /* Initialize bookkeeping for the chunk */
    initial_chunk->size = 0;
    initial_chunk->max_size = 1;
    initial_chunk->next = NULL;

    /* Add chunk to list */
    new_list->first = initial_chunk;
    new_list->last = initial_chunk;

    return new_list;
}


void add_value_to_value_list(value_list *list, int val) {

    value_chunk *chunk;

    chunk = list->last;

    if (chunk->size == chunk->max_size) {

        /* Make a new chunk and add it to the list */
        chunk->next = malloc(2 * (chunk->max_size + sizeof(value_chunk)));
        chunk = chunk->next;
        list->last = chunk;

        /* Initialize bookkeeping data for new chunk */
        chunk->size = 0;
        chunk->max_size = 2 * (chunk->max_size) + sizeof(value_chunk);
        chunk->next = NULL;

    }

    /* Add the value to the chunk */
    *(((int) (chunk + 1)) + chunk->size) = val;

    /* Update bookkeeping */
    chunk->size++;
}


int value_list_contains_value(value_list *list, int val) {

    int *i;
    int index;

    value_chunk *current_chunk;

    for (current_chunk = list->first;
         current_chunk != NULL;
         current_chunk = current_chunk->next) {

        for (i = (int *) (list + 1), index = 0;
             index < list->size;
             i++, index++) {

            if (*i == val) {
                return 1;
            }

        }

    }

    return 0;

}
