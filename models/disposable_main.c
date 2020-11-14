/**
 * \file disposable_main.c
 *
 * \brief Simple model check of the disposable and dispose interface.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
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
        /* initialize the dispose method. */
        dispose_init(x, &custom_dispose);

        /* verify that we can dispose of this instance properly. */
        dispose(x);
    }

    return 0;
}
