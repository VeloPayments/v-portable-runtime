#include <cbmc/model_assert.h>
#include <stdbool.h>
#include <string.h>
#include <vpr/compare.h>

/* forward decls */
static void verify_compare_char();
static void verify_compare_uint16();
static void verify_compare_unsigned_char();
static void verify_compare_uint64();
static void verify_compare_float();
static void verify_compare_long_double();
static void verify_compare_int64();
static void verify_compare_double();
static void verify_compare_int8();
static void verify_compare_bool();
static void verify_compare_long_long();
static void verify_compare_uint8();
static void verify_compare_unsigned_short();
static void verify_compare_int();
static void verify_compare_unsigned_long();
static void verify_compare_uint32();
static void verify_compare_unsigned_long_long();
static void verify_compare_int16();
static void verify_compare_int32();
static void verify_compare_unsigned_int();
static void verify_compare_long();
static void verify_compare_short();

int main(int argc, char* argv[])
{
    verify_compare_char();
    verify_compare_uint16();
    verify_compare_unsigned_char();
    verify_compare_uint64();
    verify_compare_float();
    verify_compare_long_double();
    verify_compare_int64();
    verify_compare_double();
    verify_compare_int8();
    verify_compare_bool();
    verify_compare_long_long();
    verify_compare_uint8();
    verify_compare_unsigned_short();
    verify_compare_int();
    verify_compare_unsigned_long();
    verify_compare_uint32();
    verify_compare_unsigned_long_long();
    verify_compare_int16();
    verify_compare_int32();
    verify_compare_unsigned_int();
    verify_compare_long();
    verify_compare_short();

    return 0;
}

static void verify_compare_char()
{
    char CH1 = 'a';
    char CH2 = 'z';

    MODEL_ASSERT(0 > compare_char(&CH1, &CH2, sizeof(char)));
    MODEL_ASSERT(0 < compare_char(&CH2, &CH1, sizeof(char)));
    MODEL_ASSERT(0 == compare_char(&CH1, &CH1, sizeof(char)));
}

static void verify_compare_uint16()
{
    uint16_t U1 = 71;
    uint16_t U2 = 85;

    MODEL_ASSERT(0 > compare_uint16(&U1, &U2, sizeof(uint16_t)));
    MODEL_ASSERT(0 < compare_uint16(&U2, &U1, sizeof(uint16_t)));
    MODEL_ASSERT(0 == compare_uint16(&U1, &U1, sizeof(uint16_t)));
}

static void verify_compare_unsigned_char()
{
    unsigned char CH1 = 'a';
    unsigned char CH2 = 'z';

    MODEL_ASSERT(0 > compare_unsigned_char(&CH1, &CH2, sizeof(unsigned char)));
    MODEL_ASSERT(0 < compare_unsigned_char(&CH2, &CH1, sizeof(unsigned char)));
    MODEL_ASSERT(0 == compare_unsigned_char(&CH1, &CH1, sizeof(unsigned char)));
}

static void verify_compare_uint64()
{
    uint64_t U1 = 71;
    uint64_t U2 = 85;

    MODEL_ASSERT(0 > compare_uint64(&U1, &U2, sizeof(uint64_t)));
    MODEL_ASSERT(0 < compare_uint64(&U2, &U1, sizeof(uint64_t)));
    MODEL_ASSERT(0 == compare_uint64(&U1, &U1, sizeof(uint64_t)));
}

static void verify_compare_float()
{
    float F1 = 7.1;
    float F2 = 8.5;

    MODEL_ASSERT(0 > compare_float(&F1, &F2, sizeof(float)));
    MODEL_ASSERT(0 < compare_float(&F2, &F1, sizeof(float)));
    MODEL_ASSERT(0 == compare_float(&F1, &F1, sizeof(float)));
}

static void verify_compare_long_double()
{
    long double F1 = 7.1;
    long double F2 = 8.5;

    MODEL_ASSERT(0 > compare_long_double(&F1, &F2, sizeof(long double)));
    MODEL_ASSERT(0 < compare_long_double(&F2, &F1, sizeof(long double)));
    MODEL_ASSERT(0 == compare_long_double(&F1, &F1, sizeof(long double)));
}

static void verify_compare_int64()
{
    int64_t I1 = 71;
    int64_t I2 = 85;

    MODEL_ASSERT(0 > compare_int64(&I1, &I2, sizeof(int64_t)));
    MODEL_ASSERT(0 < compare_int64(&I2, &I1, sizeof(int64_t)));
    MODEL_ASSERT(0 == compare_int64(&I1, &I1, sizeof(int64_t)));
}

static void verify_compare_double()
{
    double F1 = 7.1;
    double F2 = 8.5;

    MODEL_ASSERT(0 > compare_double(&F1, &F2, sizeof(double)));
    MODEL_ASSERT(0 < compare_double(&F2, &F1, sizeof(double)));
    MODEL_ASSERT(0 == compare_double(&F1, &F1, sizeof(double)));
}

static void verify_compare_int8()
{
    int8_t I1 = 71;
    int8_t I2 = 85;

    MODEL_ASSERT(0 > compare_int8(&I1, &I2, sizeof(int8_t)));
    MODEL_ASSERT(0 < compare_int8(&I2, &I1, sizeof(int8_t)));
    MODEL_ASSERT(0 == compare_int8(&I1, &I1, sizeof(int8_t)));
}

static void verify_compare_bool()
{
    bool B1 = false;
    bool B2 = true;

    MODEL_ASSERT(0 > compare_bool(&B1, &B2, sizeof(bool)));
    MODEL_ASSERT(0 < compare_bool(&B2, &B1, sizeof(bool)));
    MODEL_ASSERT(0 == compare_bool(&B1, &B1, sizeof(bool)));
}

static void verify_compare_long_long()
{
    long long I1 = 71;
    long long I2 = 85;

    MODEL_ASSERT(0 > compare_long_long(&I1, &I2, sizeof(long long)));
    MODEL_ASSERT(0 < compare_long_long(&I2, &I1, sizeof(long long)));
    MODEL_ASSERT(0 == compare_long_long(&I1, &I1, sizeof(long long)));
}

static void verify_compare_uint8()
{
    uint8_t U1 = 71;
    uint8_t U2 = 85;

    MODEL_ASSERT(0 > compare_uint8(&U1, &U2, sizeof(uint8_t)));
    MODEL_ASSERT(0 < compare_uint8(&U2, &U1, sizeof(uint8_t)));
    MODEL_ASSERT(0 == compare_uint8(&U1, &U1, sizeof(uint8_t)));
}

static void verify_compare_unsigned_short()
{
    unsigned short U1 = 71;
    unsigned short U2 = 85;

    MODEL_ASSERT(0 > compare_unsigned_short(&U1, &U2, sizeof(unsigned short)));
    MODEL_ASSERT(0 < compare_unsigned_short(&U2, &U1, sizeof(unsigned short)));
    MODEL_ASSERT(0 == compare_unsigned_short(&U1, &U1, sizeof(unsigned short)));
}

static void verify_compare_int()
{
    int I1 = 71;
    int I2 = 85;

    MODEL_ASSERT(0 > compare_int(&I1, &I2, sizeof(int)));
    MODEL_ASSERT(0 < compare_int(&I2, &I1, sizeof(int)));
    MODEL_ASSERT(0 == compare_int(&I1, &I1, sizeof(int)));
}

static void verify_compare_unsigned_long()
{
    unsigned long U1 = 71;
    unsigned long U2 = 85;

    MODEL_ASSERT(0 > compare_unsigned_long(&U1, &U2, sizeof(unsigned long)));
    MODEL_ASSERT(0 < compare_unsigned_long(&U2, &U1, sizeof(unsigned long)));
    MODEL_ASSERT(0 == compare_unsigned_long(&U1, &U1, sizeof(unsigned long)));
}

static void verify_compare_uint32()
{
    uint32_t U1 = 71;
    uint32_t U2 = 85;

    MODEL_ASSERT(0 > compare_uint32(&U1, &U2, sizeof(uint32_t)));
    MODEL_ASSERT(0 < compare_uint32(&U2, &U1, sizeof(uint32_t)));
    MODEL_ASSERT(0 == compare_uint32(&U1, &U1, sizeof(uint32_t)));
}

static void verify_compare_unsigned_long_long()
{
    unsigned long long U1 = 71;
    unsigned long long U2 = 85;

    MODEL_ASSERT(0 >
        compare_unsigned_long_long(&U1, &U2, sizeof(unsigned long long)));
    MODEL_ASSERT(0 <
        compare_unsigned_long_long(&U2, &U1, sizeof(unsigned long long)));
    MODEL_ASSERT(0 ==
        compare_unsigned_long_long(&U1, &U1, sizeof(unsigned long long)));
}

static void verify_compare_int16()
{
    int16_t I1 = 71;
    int16_t I2 = 85;

    MODEL_ASSERT(0 > compare_int16(&I1, &I2, sizeof(int16_t)));
    MODEL_ASSERT(0 < compare_int16(&I2, &I1, sizeof(int16_t)));
    MODEL_ASSERT(0 == compare_int16(&I1, &I1, sizeof(int16_t)));
}

static void verify_compare_int32()
{
    int32_t I1 = 71;
    int32_t I2 = 85;

    MODEL_ASSERT(0 > compare_int32(&I1, &I2, sizeof(int32_t)));
    MODEL_ASSERT(0 < compare_int32(&I2, &I1, sizeof(int32_t)));
    MODEL_ASSERT(0 == compare_int32(&I1, &I1, sizeof(int32_t)));
}

static void verify_compare_unsigned_int()
{
    unsigned int U1 = 71;
    unsigned int U2 = 85;

    MODEL_ASSERT(0 > compare_unsigned_int(&U1, &U2, sizeof(unsigned int)));
    MODEL_ASSERT(0 < compare_unsigned_int(&U2, &U1, sizeof(unsigned int)));
    MODEL_ASSERT(0 == compare_unsigned_int(&U1, &U1, sizeof(unsigned int)));
}

static void verify_compare_long()
{
    long I1 = 71;
    long I2 = 85;

    MODEL_ASSERT(0 > compare_long(&I1, &I2, sizeof(long)));
    MODEL_ASSERT(0 < compare_long(&I2, &I1, sizeof(long)));
    MODEL_ASSERT(0 == compare_long(&I1, &I1, sizeof(long)));
}

static void verify_compare_short()
{
    short I1 = 71;
    short I2 = 85;

    MODEL_ASSERT(0 > compare_short(&I1, &I2, sizeof(short)));
    MODEL_ASSERT(0 < compare_short(&I2, &I1, sizeof(short)));
    MODEL_ASSERT(0 == compare_short(&I1, &I1, sizeof(short)));
}
