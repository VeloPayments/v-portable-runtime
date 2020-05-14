/**
 * \file uuid/vpr_uuid_from_string.c
 *
 * \brief Convert a string value to a uuid.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <ctype.h>
#include <vpr/uuid.h>

/* forward decls. */
static uint8_t nibble_read(char c);

/**
 * \brief Convert a string to a uuid value.
 *
 * \note On success, the uuid pointer is updated to reflect the value of the
 * given string.
 *
 * \param uuid          The \ref vpr_uuid value to convert to a string.
 * \param str           Pointer to the pointer to receive the converted string.
 *
 * \returns a status code indicating success or failure.
 *      - VPR_STATUS_SUCCESS on success.
 *      - VPR_ERROR_UUID_CONVERSION_FAILED if the conversion to a uuid value
 *        failed.
 */
int vpr_uuid_from_string(
    vpr_uuid* uuid, const char* str)
{
    MODEL_ASSERT(NULL != uuid);
    MODEL_ASSERT(NULL != str);

    uint8_t b = 0;
    int offset = 0;
    enum
    {
        READ_HIGH,
        READ_LOW
    } nibble_state = READ_HIGH;

    while (*str != 0)
    {
        if ('-' == *str)
        {
            /* skip. */
        }
        else if (isxdigit(*str))
        {
            if (READ_HIGH == nibble_state)
            {
                /* read the high nibble from the string. */
                b = nibble_read(*str) << 4;
                nibble_state = READ_LOW;
            }
            else
            {
                /* read the low nibble from the string and write the value. */
                b |= nibble_read(*str);
                uuid->data[offset] = b;

                /* reset for the next byte. */
                ++offset;
                b = 0;
                nibble_state = READ_HIGH;

                /* if we're done, return success. */
                if (offset > 15)
                {
                    return VPR_STATUS_SUCCESS;
                }
            }
        }
        else
        {
            return VPR_ERROR_UUID_CONVERSION_FAILED;
        }

        ++str;
    }

    return VPR_ERROR_UUID_CONVERSION_FAILED;
}

/**
 * \brief Read a nibble value from a hex character.
 */
static uint8_t nibble_read(char c)
{
    MODEL_ASSERT(
        (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));

    switch (c)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return c - '0';

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return c - 'A' + 10;

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return c - 'a' + 10;

        default:
            return 0;
    }
}
