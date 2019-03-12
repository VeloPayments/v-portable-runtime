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
    --object-bits 16 --unwind 10 --drop-unused-functions \
	-I $(VCMODEL_DIR)/include -I ../include \
    -DMODEL_CHECK_vpr_malloc_allocator_shadowed \
	$(MODEL_CHECK_SOURCES) \
	doubly_linked_list_main.c \
	../src/doubly_linked_list/doubly_linked_list_init.c \
	../src/doubly_linked_list/doubly_linked_list_options_init.c \
	../src/doubly_linked_list/doubly_linked_list_options_init_ex.c \
	../src/allocator/malloc_allocator_options_init_shadow.c
