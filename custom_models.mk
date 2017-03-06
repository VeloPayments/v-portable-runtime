model/dynamic_array/custom_fail_%.custmodel: model/dynamic_array/custom_fail_%.c
	( \
	 cbmc $(COMMON_INCLUDES) $(CBMC_STANDARD_OPTS) --unwinding-assertions \
	      --unwind 10 --unwindset __builtin___memset_chk.0:60,malloc_allocator_options_init.0:40,dynamic_array_grow.0:6,dynamic_array_grow.1:6 \
		  $(MODEL_CHECK_SOURCES) $(SOURCES) $<;\
	 if [ $$? -eq 0 ]; then \
        echo "This model should have failed.  Error!"; \
	    false; \
     fi )
	@echo "This is okay.  Failure in a fail_model is expected."

model/dynamic_array/custom_%.custmodel: model/dynamic_array/custom_%.c
	cbmc $(COMMON_INCLUDES) $(CBMC_STANDARD_OPTS) --unwinding-assertions \
	      --unwind 10 --unwindset __builtin___memset_chk.0:60,malloc_allocator_options_init.0:40,dynamic_array_grow.0:6,dynamic_array_grow.1:6 \
	    $(MODEL_CHECK_SOURCES) $(SOURCES) $<
