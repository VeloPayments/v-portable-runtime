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
	--trace --stop-on-fail \
	--unwind 2 --unwindset __builtin___memset_chk.0:60,memcpy.0:10 \
	--unwinding-assertions --drop-unused-functions \
	-I $(VCMODEL_DIR)/include -I ../include \
	-DCBMC \
	-DMODEL_CHECK_vpr_malloc_allocator_shadowed \
	-DMODEL_CHECK_vpr_allocator_shadowed \
	-DMODEL_CHECK_vpr_dispose_shadowed \
	-DMODEL_CHECK_vpr_dll_shadowed \
	$(MODEL_CHECK_SOURCES) \
	hashmap_main.c \
	../src/hashmap/hashmap_init.c \
	../src/hashmap/hashmap_options_init.c \
	../src/hashmap/hashmap_options_init_ex.c \
	../src/hashmap/hashmap_get.c \
	../src/hashmap/hashmap_get64.c \
	../src/hashmap/hashmap_put.c \
	../src/hashmap/hashmap_put64.c \
	../src/doubly_linked_list/doubly_linked_list_options_init.c \
	../src/doubly_linked_list/doubly_linked_list_options_init_ex.c \
	../src/doubly_linked_list/doubly_linked_list_init_shadow.c \
	../src/doubly_linked_list/doubly_linked_list_insert_beginning_shadow.c \
	../src/doubly_linked_list/doubly_linked_list_remove.c \
	../src/hash_func/hash_func.c \
	../src/allocator/allocate_shadow.c \
	../src/allocator/malloc_allocator_options_init_shadow.c \
	../src/allocator/malloc_allocator_options_dispose_shadow.c \
	../src/allocator/release_shadow.c \
	../src/disposable/dispose_shadow.c
