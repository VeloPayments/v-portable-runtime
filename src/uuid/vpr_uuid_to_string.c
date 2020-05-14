/**
 * \file uuid/vpr_uuid_to_string.c
 *
 * \brief Convert a uuid value to a string.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/uuid.h>

#define UUID_STRING_LENGTH 36

/**
 * \brief Convert a nibble to hex.
 */
static inline char nibble_to_hex(char nibble)
{
    MODEL_ASSERT(nibble >= 0 && nibble <= 15);

    switch (nibble)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            return nibble + '0';

        default:
            return nibble - 10 + 'a';
    }
}

#define LOW_NIBBLE(b) \
    (b & 0x0F)

#define HIGH_NIBBLE(b) \
    ((b & 0xF0) >> 4)

/**
 * \brief Convert a uuid value to a string.
 *
 * \note On success, the char pointer pointer is updated to point to a string
 * value allocated by this function.  This string value is owned by the caller
 * and must be released when no longer needed.
 *
 * \param str           Pointer to the pointer to receive the converted string.
 * \param alloc_opts    The allocator options to use to allocate this string
 *                      value.
 * \param uuid          The \ref vpr_uuid value to convert to a string.
 *
 * \returns a status code indicating success or failure.
 *      - VPR_STATUS_SUCCESS on success.
 *      - VPR_ERROR_UUID_ALLOCATION_FAILED if memory allocation for the string
 *        failed.
 */
int vpr_uuid_to_string(
    char** str, allocator_options_t* alloc_opts, const vpr_uuid* uuid)
{
    MODEL_ASSERT(NULL != str);
    MODEL_ASSERT(MODEL_PROP_VALID_ALLOCATOR_OPTIONS(alloc_opts));
    MODEL_ASSERT(NULL != uuid);

    /* attempt to allocate a buffer to hold our string value. */
    *str = (char*)allocate(alloc_opts, UUID_STRING_LENGTH + 1);
    if (NULL == *str)
    {
        return VPR_ERROR_UUID_ALLOCATION_FAILED;
    }

    /* set the digits. */
    (*str)[0] = nibble_to_hex(HIGH_NIBBLE(uuid->data[0]));
    (*str)[1] = nibble_to_hex(LOW_NIBBLE(uuid->data[0]));
    (*str)[2] = nibble_to_hex(HIGH_NIBBLE(uuid->data[1]));
    (*str)[3] = nibble_to_hex(LOW_NIBBLE(uuid->data[1]));
    (*str)[4] = nibble_to_hex(HIGH_NIBBLE(uuid->data[2]));
    (*str)[5] = nibble_to_hex(LOW_NIBBLE(uuid->data[2]));
    (*str)[6] = nibble_to_hex(HIGH_NIBBLE(uuid->data[3]));
    (*str)[7] = nibble_to_hex(LOW_NIBBLE(uuid->data[3]));
    (*str)[8] = '-';
    (*str)[9] = nibble_to_hex(HIGH_NIBBLE(uuid->data[4]));
    (*str)[10] = nibble_to_hex(LOW_NIBBLE(uuid->data[4]));
    (*str)[11] = nibble_to_hex(HIGH_NIBBLE(uuid->data[5]));
    (*str)[12] = nibble_to_hex(LOW_NIBBLE(uuid->data[5]));
    (*str)[13] = '-';
    (*str)[14] = nibble_to_hex(HIGH_NIBBLE(uuid->data[6]));
    (*str)[15] = nibble_to_hex(LOW_NIBBLE(uuid->data[6]));
    (*str)[16] = nibble_to_hex(HIGH_NIBBLE(uuid->data[7]));
    (*str)[17] = nibble_to_hex(LOW_NIBBLE(uuid->data[7]));
    (*str)[18] = '-';
    (*str)[19] = nibble_to_hex(HIGH_NIBBLE(uuid->data[8]));
    (*str)[20] = nibble_to_hex(LOW_NIBBLE(uuid->data[8]));
    (*str)[21] = nibble_to_hex(HIGH_NIBBLE(uuid->data[9]));
    (*str)[22] = nibble_to_hex(LOW_NIBBLE(uuid->data[9]));
    (*str)[23] = '-';
    (*str)[24] = nibble_to_hex(HIGH_NIBBLE(uuid->data[10]));
    (*str)[25] = nibble_to_hex(LOW_NIBBLE(uuid->data[10]));
    (*str)[26] = nibble_to_hex(HIGH_NIBBLE(uuid->data[11]));
    (*str)[27] = nibble_to_hex(LOW_NIBBLE(uuid->data[11]));
    (*str)[28] = nibble_to_hex(HIGH_NIBBLE(uuid->data[12]));
    (*str)[29] = nibble_to_hex(LOW_NIBBLE(uuid->data[12]));
    (*str)[30] = nibble_to_hex(HIGH_NIBBLE(uuid->data[13]));
    (*str)[31] = nibble_to_hex(LOW_NIBBLE(uuid->data[13]));
    (*str)[32] = nibble_to_hex(HIGH_NIBBLE(uuid->data[14]));
    (*str)[33] = nibble_to_hex(LOW_NIBBLE(uuid->data[14]));
    (*str)[34] = nibble_to_hex(HIGH_NIBBLE(uuid->data[15]));
    (*str)[35] = nibble_to_hex(LOW_NIBBLE(uuid->data[15]));
    (*str)[36] = 0;

    MODEL_ASSERT(UUID_STRING_LENGTH == 36);

    return VPR_STATUS_SUCCESS;
}
