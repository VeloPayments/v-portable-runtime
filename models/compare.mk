CBMC_DIR?=/opt/cbmc
CBMC?=$(CBMC_DIR)/bin/cbmc
VCMODEL_DIR?=../../vcmodel
MODEL_CHECK_DIR?=../../vcmodel

include $(MODEL_CHECK_DIR)/model_check.mk

.PHONY: ALL run_check

#ALL: run_check
ALL:

run_check:
	$(CBMC) --bounds-check --pointer-check --memory-leak-check \
	--div-by-zero-check --signed-overflow-check --unsigned-overflow-check \
    --pointer-overflow-check --conversion-check \
	--conversion-check --trace --stop-on-fail -DCBMC \
    --object-bits 16 --drop-unused-functions \
	-I $(VCMODEL_DIR)/include -I ../include \
	$(MODEL_CHECK_SOURCES) \
	../src/compare/compare_char.c \
	../src/compare/compare_uint16.c \
	../src/compare/compare_unsigned_char.c \
	../src/compare/compare_uint64.c \
	../src/compare/compare_float.c \
	../src/compare/compare_long_double.c \
	../src/compare/compare_int64.c \
	../src/compare/compare_double.c \
	../src/compare/compare_int8.c \
	../src/compare/compare_bool.c \
	../src/compare/compare_long_long.c \
	../src/compare/compare_uint8.c \
	../src/compare/compare_unsigned_short.c \
	../src/compare/compare_int.c \
	../src/compare/compare_unsigned_long.c \
	../src/compare/compare_uint32.c \
	../src/compare/compare_unsigned_long_long.c \
	../src/compare/compare_int16.c \
	../src/compare/compare_int32.c \
	../src/compare/compare_unsigned_int.c \
	../src/compare/compare_long.c \
	../src/compare/compare_short.c \
	compare_main.c
