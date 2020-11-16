CBMC_DIR?=/opt/cbmc
CBMC?=$(CBMC_DIR)/bin/cbmc
VCMODEL_DIR?=../../vcmodel
MODEL_CHECK_DIR?=../../vcmodel

include $(MODEL_CHECK_DIR)/model_check.mk

.PHONY: ALL run_check

ALL: run_check

run_check:
	$(CBMC) --bounds-check --pointer-check --memory-leak-check \
	--div-by-zero-check --signed-overflow-check --unsigned-overflow-check \
    --pointer-overflow-check --conversion-check \
	--conversion-check --trace --stop-on-fail -DCBMC \
    --object-bits 16 --drop-unused-functions \
	-I $(VCMODEL_DIR)/include -I ../include \
	$(MODEL_CHECK_SOURCES) \
    ../src/allocator/allocate.c \
    ../src/allocator/release.c \
    ../src/allocator/reallocate.c \
    ../src/disposable/dispose.c \
    ../src/disposable/dispose_init.c \
    ../src/allocator/bump_allocator_options_init.c \
	shadow/allocator/prop_allocator_valid.c \
	shadow/disposable/prop_disposable_valid.c \
	bump_allocator_main.c
