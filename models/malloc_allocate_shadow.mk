CBMC_DIR?=/opt/cbmc
CBMC?=$(CBMC_DIR)/bin/cbmc
VCMODEL_DIR?=../../vcmodel
MODEL_CHECK_DIR?=../../vcmodel

include $(MODEL_CHECK_DIR)/model_check.mk

ALL:
	$(CBMC) --bounds-check --pointer-check --memory-leak-check \
	--div-by-zero-check --signed-overflow-check --unsigned-overflow-check \
    --pointer-overflow-check --conversion-check \
	--conversion-check --trace --stop-on-fail -DCBMC \
    --object-bits 16 --drop-unused-functions \
	-I $(VCMODEL_DIR)/include -I ../include \
	-DMODEL_CHECK_vpr_malloc_allocator_shadowed \
	$(MODEL_CHECK_SOURCES) \
    ../src/allocator/allocate.c \
    ../src/allocator/release.c \
    ../src/allocator/reallocate.c \
    ../src/disposable/dispose.c \
    ../src/allocator/malloc_allocator_options_init_shadow.c \
    ../src/allocator/malloc_allocator_options_dispose_shadow.c \
	malloc_allocate_shadow_main.c
