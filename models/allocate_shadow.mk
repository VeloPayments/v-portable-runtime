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
    --drop-unused-functions \
	-I $(VCMODEL_DIR)/include -I ../include \
	$(MODEL_CHECK_SOURCES) \
	-DMODEL_CHECK_vpr_allocator_shadowed \
    ../src/allocator/allocate_shadow.c \
    ../src/allocator/release_shadow.c \
    ../src/allocator/reallocate_shadow.c \
    ../src/disposable/dispose.c \
	allocate_shadow_main.c
