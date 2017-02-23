/**
 * \file disposable_model_check.c
 *
 * Simple model check of the disposable interface.  Happy path.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <vpr/disposable.h>

/**
 * Our custom dispose method frees the data structure.
 */
void custom_dispose(void* x)
{
    free(x);
}

int main(int argc, char* argv[])
{
    disposable_t* x = (disposable_t*)malloc(sizeof(disposable_t));

    /* in our model, malloc() can return NULL.  Only follow path where it
     * doesn't. */
    if (x != NULL)
    {
        /* set the dispose method */
        x->dispose = &custom_dispose;

        /* verify model assertions */
        dispose(x);
    }

    return 0;
}
