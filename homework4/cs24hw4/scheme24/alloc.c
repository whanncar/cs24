/*
 * This allocator now implements a mark and sweep garbage collector
 * via the collect_garbage() method.
 *
 */


#include "alloc.h"
#include "ptr_vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


/*! Change to #define to output garbage-collector statistics. */
#define GC_STATS

/*!
 * Change to #undef to cause the garbage collector to only run when it has to.
 * This dramatically improves performance.
 *
 * However, while testing GC, it's easiest if you try it all the time, so that
 * the number of objects being manipulated is small and easy to understand.
 */
#define ALWAYS_GC


/* Change to #define for other verbose output. */
#undef VERBOSE


void free_value(Value *v);
void free_lambda(Lambda *f);
void free_environment(Environment *env);

void mark_environment(Environment *);
void mark_value(Value *);
void mark_lambda(Lambda *);
void mark_eval_stack(PtrStack *);
void sweep_values();
void sweep_lambdas();
void sweep_environments();

/*!
 * A growable vector of pointers to all Value structs that are currently
 * allocated.
 */
static PtrVector allocated_values;


/*!
 * A growable vector of pointers to all Lambda structs that are currently
 * allocated.  Note that each Lambda struct will only have ONE Value struct that
 * points to it.
 */
static PtrVector allocated_lambdas;


/*!
 * A growable vector of pointers to all Environment structs that are currently
 * allocated.
 */
static PtrVector allocated_environments;


#ifndef ALWAYS_GC

/*! Starts at 1MB, and is doubled every time we can't stay within it. */
static long max_allocation_size = 1048576;

#endif


void init_alloc() {
    pv_init(&allocated_values);
    pv_init(&allocated_lambdas);
    pv_init(&allocated_environments);
}


/*!
 * This helper function prints some helpful details about the current allocation
 * status of the program.
 */
void print_alloc_stats(FILE *f) {
    /*
    fprintf(f, "Allocation statistics:\n");
    fprintf(f, "\tAllocated environments:  %u\n", allocated_environments.size);
    fprintf(f, "\tAllocated lambdas:  %u\n", allocated_lambdas.size);
    fprintf(f, "\tAllocated values:  %u\n", allocated_values.size);
    */

    fprintf(f, "%d vals \t%d lambdas \t%d envs\n", allocated_values.size,
        allocated_lambdas.size, allocated_environments.size);
}


/*!
 * This helper function returns the amount of memory currently being used by
 * garbage-collected objects.  It is NOT the total amount of memory being used
 * by the interpreter!
 */ 
long allocation_size() {
    long size = 0;
    
    size += sizeof(Value) * allocated_values.size;
    size += sizeof(Lambda) * allocated_lambdas.size;
    size += sizeof(Value) * allocated_environments.size;
    
    return size;
}


/*!
 * This function heap-allocates a new Value struct, initializes it to be empty,
 * and then records the struct's pointer in the allocated_values vector.
 */
Value * alloc_value(void) {
    Value *v = malloc(sizeof(Value));
    memset(v, 0, sizeof(Value));

    pv_add_elem(&allocated_values, v);

    return v;
}


/*!
 * This function frees a heap-allocated Value struct.  Since a Value struct can
 * represent several different kinds of values, the function looks at the
 * value's type tag to determine if additional memory needs to be freed for the
 * value.
 *
 * Note:  It is assumed that the value's pointer has already been removed from
 *        the allocated_values vector!  If this is not the case, serious errors
 *        will almost certainly occur.
 */
void free_value(Value *v) {
    assert(v != NULL);

    /*
     * If value refers to a lambda, we don't free it here!  Lambdas are freed
     * by the free_lambda() function, and that is called when cleaning up
     * unreachable objects.
     */

    if (v->type == T_String || v->type == T_Atom || v->type == T_Error)
        free(v->string_val);

    free(v);
}



/*!
 * This function heap-allocates a new Lambda struct, initializes it to be empty,
 * and then records the struct's pointer in the allocated_lambdas vector.
 */
Lambda * alloc_lambda(void) {
    Lambda *f = malloc(sizeof(Lambda));
    memset(f, 0, sizeof(Lambda));

    pv_add_elem(&allocated_lambdas, f);

    return f;
}


/*!
 * This function frees a heap-allocated Lambda struct.
 *
 * Note:  It is assumed that the lambda's pointer has already been removed from
 *        the allocated_labmdas vector!  If this is not the case, serious errors
 *        will almost certainly occur.
 */
void free_lambda(Lambda *f) {
    assert(f != NULL);

    /* Lambdas typically reference lists of Value objects for the argument-spec
     * and the body, but we don't need to free these here because they are
     * managed separately.
     */

    free(f);
}


/*!
 * This function heap-allocates a new Environment struct, initializes it to be
 * empty, and then records the struct's pointer in the allocated_environments
 * vector.
 */
Environment * alloc_environment(void) {
    Environment *env = malloc(sizeof(Environment));
    memset(env, 0, sizeof(Environment));

    pv_add_elem(&allocated_environments, env);

    return env;
}


/*!
 * This function frees a heap-allocated Environment struct.  The environment's
 * bindings are also freed since they are owned by the environment, but the
 * binding-values are not freed since they are externally managed.
 *
 * Note:  It is assumed that the environment's pointer has already been removed
 *        from the allocated_environments vector!  If this is not the case,
 *        serious errors will almost certainly occur.
 */
void free_environment(Environment *env) {
    int i;

    /* Free the bindings in the environment first. */
    for (i = 0; i < env->num_bindings; i++) {
        free(env->bindings[i].name);
        /* Don't free the value, since those are handled separately. */
    }
    free(env->bindings);

    /* Now free the environment object itself. */
    free(env);
}


/*!
 * This function performs the garbage collection for the Scheme interpreter.
 * It also contains code to track how many objects were collected on each run,
 * and also it can optionally be set to do GC when the total memory used grows
 * beyond a certain limit.
 */
void collect_garbage() {
    Environment *global_env;
    PtrStack *eval_stack;

#ifdef GC_STATS
    int vals_before, procs_before, envs_before;
    int vals_after, procs_after, envs_after;

    vals_before = allocated_values.size;
    procs_before = allocated_lambdas.size;
    envs_before = allocated_environments.size;
#endif

#ifndef ALWAYS_GC
    /* Don't perform garbage collection if we still have room to grow. */
    if (allocation_size() < max_allocation_size)
        return;
#endif

    global_env = get_global_environment();
    eval_stack = get_eval_stack();

    /* Mark everything reachable from global environment */
    mark_environment(global_env);

    /* Mark everything reachable from evaluation stack */
    mark_eval_stack(eval_stack);

    /* Sweep */
    sweep_values();
    sweep_lambdas();
    sweep_environments();

#ifndef ALWAYS_GC
    /* If we are still above the maximum allocation size, increase it. */
    if (allocation_size() > max_allocation_size) {
        max_allocation_size *= 2;

        printf("Increasing maximum allocation size to %ld bytes.\n",
            max_allocation_size);
    }
#endif
    
#ifdef GC_STATS
    vals_after = allocated_values.size;
    procs_after = allocated_lambdas.size;
    envs_after = allocated_environments.size;

    printf("GC Results:\n");
    printf("\tBefore: \t%d vals \t%d lambdas \t%d envs\n",
            vals_before, procs_before, envs_before);
    printf("\tAfter:  \t%d vals \t%d lambdas \t%d envs\n",
            vals_after, procs_after, envs_after);
    printf("\tChange: \t%d vals \t%d lambdas \t%d envs\n",
            vals_after - vals_before, procs_after - procs_before,
            envs_after - envs_before);
#endif
}


/* 
 * mark_environment: If the passed environment is unmarked, this method
 *                   marks it, then calls mark on each of its values
 *                   and its parent environment.
 *
 * arguments: env: The environment to be marked
 *
 */

void mark_environment(Environment *env) {

    int i;

    /* Return if this environment has already been seen */
    if (env->marked) {
        return;
    }

    /* Mark the environment */
    env->marked = 1;

    /* Mark each of the environment's values */
    for (i = 0; i < env->num_bindings; i++) {
        mark_value(env->bindings[i].value);
    }

    /* If the environment has a parent environment, mark it */
    if (env->parent_env != NULL) {
        mark_environment(env->parent_env);
    }

}


/*
 * mark_value: If the passed value is unmarked, this method marks it,
 *             then marks its data if necessary.
 *
 * arguments: v: The value to be marked
 *
 */

void mark_value(Value *v) {

    /* Return if the value has already been seen */
    if (v->marked) {
        return;
    }

    /* Mark the value */
    v->marked = 1;

    /* If the value is a lambda type, mark its lambda */
    if (v->type == T_Lambda) {
        mark_lambda(v->lambda_val);
    }

    /* If the value is a conspair type, mark its conspair values */
    if (v->type == T_ConsPair) {
        mark_value(v->cons_val.p_car);
        mark_value(v->cons_val.p_cdr);
    }

}


/*
 * mark_lambda: If the passed lambda is unmarked, this method marks it,
 *              then marks the arg_spec and body of the lambda if it is
 *              not a native implementation.
 *
 * arguments: f: The lambda to be marked
 *
 */

void mark_lambda(Lambda *f) {

    /* Return if the lambda has already been seen */
    if (f->marked) {
        return;
    }

    /* Mark the lambda */
    f->marked = 1;

    /* If the lambda is not a native implementation, mark the arg_spec and body */
    if (!f->native_impl) {
        if (f->arg_spec != NULL) {
            mark_value(f->arg_spec);
        }
        if (f->body != NULL) {
            mark_value(f->body);
        }
    }

    /* Mark the parent environment */
    mark_environment(f->parent_env);

}


/*
 * mark_eval_stack: Marks all things in any context on the evaulation stack.
 *
 * arguments: eval_stack: A pointer to the evaulation stack
 *
 */

void mark_eval_stack(PtrStack *eval_stack) {

    unsigned int i, j;

    Value **ppv;
    EvaluationContext *ctx;

    /* Iterate through each context on the evaulation stack */
    for (i = 0; i < eval_stack->size; i++) {

        ctx = (EvaluationContext *) pv_get_elem(eval_stack, i);

        /* 
         * Mark the context's environment, expression,
         * and child evaluation value if they are not NULL
         */
        if (ctx->current_env != NULL) {
            mark_environment(ctx->current_env);
        }
        if (ctx->expression != NULL) {
            mark_value(ctx->expression);
        }
        if (ctx->child_eval_result != NULL) {
            mark_value(ctx->child_eval_result);
        }

        /* Iterate through the context's local values */
        for (j = 0; j < ctx->local_vals.size; j++) {

            ppv = (Value **) pv_get_elem(&ctx->local_vals, j);

            /* Mark the value if it is not NULL */
            if (*ppv != NULL) {
                mark_value(*ppv);
            }

        }

    }

}


/*
 * sweep_values: Frees all values that have not been marked as reachable
 *               and unmarks all values that have been marked as reachable,
 *               then compacts the allocated_values pointer vector.
 *
 */

void sweep_values() {

    unsigned int i;

    Value *val_ptr;

    /* Iterate through all allocated values */
    for (i = 0; i < allocated_values.size; i++) {

        val_ptr = (Value *) pv_get_elem(&allocated_values, i);

        /* 
         * If the value is not NULL, free it;
         * otherwise, unmark it 
         */
        if (val_ptr != NULL) {
            if (!(val_ptr->marked)) {
                free_value(val_ptr);
                pv_set_elem(&allocated_values, i, NULL);
            }
            else {
                val_ptr->marked = 0;
            }
        }

    }

    /* Compact allocated_values */
    pv_compact(&allocated_values);

}


/*
 * sweep_lambdas: Frees all lambdas that have not been marked as reachable
 *                and unmarks all lambdas that have been marked as reachable,
 *                then compacts the allocated_lambdas pointer vector.
 *
 */

void sweep_lambdas() {

    unsigned int i;

    Lambda *lam_ptr;

    /* Iterate through all allocated lambdas */
    for (i = 0; i < allocated_lambdas.size; i++) {

        lam_ptr = (Lambda *) pv_get_elem(&allocated_lambdas, i);

        /* 
         * If the lambda is not NULL, free it;
         * otherwise, unmark it
         */
        if (lam_ptr != NULL) {
            if (!(lam_ptr->marked)) {
                free_lambda(lam_ptr);
                pv_set_elem(&allocated_lambdas, i, NULL);
            }
            else {
                lam_ptr->marked = 0;
            }
        }

    }

    /* Compact allocated_lambdas */
    pv_compact(&allocated_lambdas);

}


/*
 * sweep_environments: Frees all environments that have not been marked as
 *                     reachable and unmarks all environments that have
 *                     been marked as reachable, then compacts the
 *                     allocated_environments pointer vector.
 *
 */

void sweep_environments() {

    unsigned int i;

    Environment *env_ptr;

    /* Iterate through all allocated environments */
    for (i = 0; i < allocated_environments.size; i++) {

        env_ptr = (Environment *) pv_get_elem(&allocated_environments, i);

        /*
         * If the environment is not NULL, free it;
         * otherwise, unmark it
         */
        if (env_ptr != NULL) {
            if (!(env_ptr->marked)) {
                free_environment(env_ptr);
                pv_set_elem(&allocated_environments, i, NULL);
            }
            else {
                env_ptr->marked = 0;
            }
        }

    }

    /* Compact allocated_environments */
    pv_compact(&allocated_environments);

}
