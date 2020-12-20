CBMC_DIR?=/opt/cbmc
CBMC?=$(CBMC_DIR)/bin/cbmc
VCMODEL_DIR?=../../vcmodel
MODEL_CHECK_DIR?=../../vcmodel

include $(MODEL_CHECK_DIR)/model_check.mk

.PHONY: ALL run_check

ALL:
#ALL: run_check

run_check:
	$(CBMC) --bounds-check --pointer-check --memory-leak-check \
	--div-by-zero-check --signed-overflow-check --unsigned-overflow-check \
    --pointer-overflow-check --conversion-check \
	--conversion-check --trace --stop-on-fail -DCBMC \
    --drop-unused-functions \
    --unwind 10 \
    --unwindset __builtin___memset_chk.0:60,malloc_allocator_options_init.0:40,dynamic_array_grow.0:6,dynamic_array_grow.1:6 \
	-I $(VCMODEL_DIR)/include -I ../include \
	$(MODEL_CHECK_SOURCES) \
	../src/abstract_factory/abstract_factory.c \
    ../src/allocator/allocate.c \
    ../src/allocator/release.c \
    ../src/allocator/reallocate.c \
    ../src/disposable/dispose.c \
    ../src/allocator/malloc_allocator_options_init.c \
    ../src/allocator/malloc_allocator_options_dispose.c \
    ../src/dynamic_array/dynamic_array_append.c \
    ../src/dynamic_array/dynamic_array_init.c \
    ../src/dynamic_array/dynamic_array_options_init.c \
	../src/dynamic_array/dynamic_array_grow.c \
	../src/dynamic_array/dynamic_array_linear_search.c \
    ../src/dynamic_array/dynamic_array_options_init_ex.c \
    ../src/compare/compare_uint32.c \
	abstract_factory_main.c
