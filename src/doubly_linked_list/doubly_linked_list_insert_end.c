#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

int doubly_linked_list_insert_end(doubly_linked_list_t* dll, void* data)
{

    MODEL_ASSERT(dll != NULL);
    MODEL_ASSERT(dll->elements >= 0);
    MODEL_ASSERT(dll->options != NULL);
    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    MODEL_ASSERT(data != NULL);

    /* is this the first element in the list? */
    if (dll->last == NULL)
    {
        doubly_linked_list_insert_beginning(dll, data);
    }
    else
    {
        doubly_linked_list_insert_after(dll, dll->last, data);
    }

    //success
    return VPR_STATUS_SUCCESS;
}
