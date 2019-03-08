LIB_NAME=libvpr.a
BUILD_DIR=$(CURDIR)/build
HOST_CHECKED_BUILD_DIR=$(BUILD_DIR)/host/checked

MODEL_CHECK_DIR?=../vcmodel

include $(MODEL_CHECK_DIR)/model_check.mk
include custom_models.mk

#OpenBSD compatibility
PWD?=$(shell pwd)

#library source files
SRCDIR=$(PWD)/src
DIRS=$(SRCDIR) $(SRCDIR)/abstract_factory $(SRCDIR)/allocator \
    $(SRCDIR)/compare $(SRCDIR)/disposable $(SRCDIR)/dynamic_array \
    $(SRCDIR)/doubly_linked_list
SOURCES=$(foreach d,$(DIRS),$(wildcard $(d)/*.c))
STRIPPED_SOURCES=$(patsubst $(SRCDIR)/%,%,$(SOURCES))
MODELDIR=$(PWD)/model
MODEL_MAKEFILES?= \
    $(foreach file,$(wildcard models/*.mk),$(notdir $(file)))

#library test files
TESTDIR=$(PWD)/test
TESTDIRS=$(TESTDIR) $(TESTDIR)/abstract_factory $(TESTDIR)/allocator \
    $(TESTDIR)/compare $(TESTDIR)/dynamic_array \
    $(TESTDIR)/doubly_linked_list
TEST_BUILD_DIR=$(HOST_CHECKED_BUILD_DIR)/test
TEST_DIRS=$(filter-out $(TESTDIR), \
    $(patsubst $(TESTDIR)/%,$(TEST_BUILD_DIR)/%,$(TESTDIRS)))
TEST_SOURCES=$(foreach d,$(TESTDIRS),$(wildcard $(d)/*.cpp))
STRIPPED_TEST_SOURCES=$(patsubst $(TESTDIR)/%,%,$(TEST_SOURCES))
TEST_OBJECTS=$(patsubst %.cpp,$(TEST_BUILD_DIR)/%.o,$(STRIPPED_TEST_SOURCES))
TESTLIBVPR=$(HOST_CHECKED_BUILD_DIR)/testlibvpr

#documentation configuration
DOC_BUILD_DIR=$(BUILD_DIR)/apidocs

#model check targets
MODEL_TARGETS= \
    $(patsubst %.c,%.model, \
        $(filter-out $(FAIL_MODEL_SOURCES) $(CUSTOM_MODEL_SOURCES), \
                     $(MODEL_SOURCES)))
FAIL_MODEL_TARGETS= \
    $(patsubst %.c,%.failmodel,$(FAIL_MODEL_SOURCES))
CUSTOM_MODEL_TARGETS= \
    $(patsubst %.c,%.custmodel,$(CUSTOM_MODEL_SOURCES))
#.PHONY: $(MODEL_TARGETS)

#platform options
CORTEXMSOFT_RELEASE_BUILD_DIR=$(BUILD_DIR)/cortex-m4-softfp/release
CORTEXMSOFT_RELEASE_LIB=$(CORTEXMSOFT_RELEASE_BUILD_DIR)/$(LIB_NAME)
CORTEXMSOFT_RELEASE_DIRS=$(filter-out $(SRCDIR), \
    $(patsubst $(SRCDIR)/%,$(CORTEXMSOFT_RELEASE_BUILD_DIR)/%,$(DIRS)))
CORTEXMSOFT_RELEASE_OBJECTS= \
    $(patsubst %.c,$(CORTEXMSOFT_RELEASE_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))
CORTEXMHARD_RELEASE_BUILD_DIR=$(BUILD_DIR)/cortex-m4-hardfp/release
CORTEXMHARD_RELEASE_LIB=$(CORTEXMHARD_RELEASE_BUILD_DIR)/$(LIB_NAME)
CORTEXMHARD_RELEASE_DIRS=$(filter-out $(SRCDIR), \
    $(patsubst $(SRCDIR)/%,$(CORTEXMHARD_RELEASE_BUILD_DIR)/%,$(DIRS)))
CORTEXMHARD_RELEASE_OBJECTS= \
    $(patsubst %.c,$(CORTEXMHARD_RELEASE_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))
HOST_CHECKED_LIB=$(HOST_CHECKED_BUILD_DIR)/$(LIB_NAME)
HOST_CHECKED_DIRS=$(filter-out $(SRCDIR), \
    $(patsubst $(SRCDIR)/%,$(HOST_CHECKED_BUILD_DIR)/%,$(DIRS)))
HOST_CHECKED_OBJECTS= \
    $(patsubst %.c,$(HOST_CHECKED_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))
HOST_RELEASE_BUILD_DIR=$(BUILD_DIR)/host/release
HOST_RELEASE_LIB=$(HOST_RELEASE_BUILD_DIR)/$(LIB_NAME)
HOST_RELEASE_DIRS=$(filter-out $(SRCDIR), \
    $(patsubst $(SRCDIR)/%,$(HOST_RELEASE_BUILD_DIR)/%,$(DIRS)))
HOST_RELEASE_OBJECTS= \
    $(patsubst %.c,$(HOST_RELEASE_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))
WASM_RELEASE_LIB=$(WASM_RELEASE_BUILD_DIR)/$(LIB_NAME)
WASM_RELEASE_BUILD_DIR=$(BUILD_DIR)/wasm/release
WASM_RELEASE_OBJECTS= \
	$(patsubst %.c,$(WASM_RELEASE_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))


#Dependencies
GTEST_DIR?=../googletest/googletest
GTEST_OBJ=$(TEST_BUILD_DIR)/gtest-all.o

#Toolchain location
TOOLCHAIN_DIR?=/opt/vctoolchain

#compilers
HOST_CHECKED_CC=$(TOOLCHAIN_DIR)/host/bin/gcc
HOST_CHECKED_CXX=$(TOOLCHAIN_DIR)/host/bin/g++
HOST_CHECKED_AR=$(AR)
HOST_CHECKED_RANLIB=$(RANLIB)
HOST_RELEASE_CC=$(TOOLCHAIN_DIR)/host/bin/gcc
HOST_RELEASE_CXX=$(TOOLCHAIN_DIR)/host/bin/g++
HOST_RELEASE_AR=$(AR)
HOST_RELEASE_RANLIB=$(RANLIB)

# TODO vendor these into vc-toolchain
WASM_RELEASE_CC=emcc
WASM_RELEASE_AR=emar

CORTEXMSOFT_RELEASE_CC=$(TOOLCHAIN_DIR)/cortex-m4-softfp/bin/arm-none-eabi-gcc
CORTEXMSOFT_RELEASE_CXX=$(TOOLCHAIN_DIR)/cortex-m4-softfp/bin/arm-none-eabi-g++
CORTEXMSOFT_RELEASE_AR=$(TOOLCHAIN_DIR)/cortex-m4-softfp/bin/arm-none-eabi-ar
CORTEXMSOFT_RELEASE_RANLIB=$(TOOLCHAIN_DIR)/cortex-m4-softfp/bin/arm-none-eabi-ranlib
CORTEXMHARD_RELEASE_CC=$(TOOLCHAIN_DIR)/cortex-m4-hardfp/bin/arm-none-eabi-gcc
CORTEXMHARD_RELEASE_CXX=$(TOOLCHAIN_DIR)/cortex-m4-hardfp/bin/arm-none-eabi-g++
CORTEXMHARD_RELEASE_AR=$(TOOLCHAIN_DIR)/cortex-m4-hardfp/bin/arm-none-eabi-ar
CORTEXMHARD_RELEASE_RANLIB=$(TOOLCHAIN_DIR)/cortex-m4-hardfp/bin/arm-none-eabi-ranlib

#platform compiler flags
COMMON_INCLUDES=$(MODEL_CHECK_INCLUDES) -I $(PWD)/include
COMMON_CFLAGS=$(COMMON_INCLUDES) -Wall -Werror -Wextra
WASM_RELEASE_CFLAGS=$(COMMON_CFLAGS) -O2
HOST_CHECKED_CFLAGS=$(COMMON_CFLAGS) -fPIC -O0 -fprofile-arcs -ftest-coverage
HOST_RELEASE_CFLAGS=$(COMMON_CFLAGS) -fPIC -O2
COMMON_CXXFLAGS=-I $(PWD)/include -Wall -Werror -Wextra
HOST_CHECKED_CXXFLAGS=-std=c++14 $(COMMON_CXXFLAGS) -O0 -fprofile-arcs \
    -ftest-coverage
HOST_RELEASE_CXXFLAGS=-std=c++14 $(COMMON_CXXFLAGS) -O2
TEST_CXXFLAGS=$(HOST_RELEASE_CXXFLAGS) -I $(GTEST_DIR) -I $(GTEST_DIR)/include
CORTEXMSOFT_RELEASE_CFLAGS=-std=gnu99 $(COMMON_CFLAGS) -O2 -mcpu=cortex-m4 \
    -mfloat-abi=soft -mthumb -fno-common -ffunction-sections -fdata-sections \
    -ffreestanding -fno-builtin -mapcs
CORTEXMSOFT_RELEASE_CXXFLAGS=-std=gnu++14 $(COMMON_CXXFLAGS) -O2 \
    -mcpu=cortex-m4 -mfloat-abi=soft -mthumb -fno-common -ffunction-sections \
    -fdata-sections -ffreestanding -fno-builtin -mapcs
CORTEXMHARD_RELEASE_CFLAGS=-std=gnu99 $(COMMON_CFLAGS) -O2 -mcpu=cortex-m4 \
    -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -fno-common -ffunction-sections \
    -fdata-sections -ffreestanding -fno-builtin -mapcs
CORTEXMHARD_RELEASE_CXXFLAGS=-std=gnu++14 $(COMMON_CXXFLAGS) -O2 \
    -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -fno-common \
    -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mapcs

#phony targets
.PHONY: ALL clean test model-check host.lib.checked host.lib.release
.PHONY: cortexmsoft.lib.release cortexmhard.lib.release
.PHONY: docs

#main build target
ALL: host.lib.checked host.lib.release cortexmsoft.lib.release
ALL: cortexmhard.lib.release

#Documentation target
docs:
	mkdir -p $(DOC_BUILD_DIR)
	$(TOOLCHAIN_DIR)/host/bin/doxygen doxygen.cfg

#host targets
host.lib.checked: $(HOST_CHECKED_DIRS) $(HOST_CHECKED_LIB)
host.lib.release: $(HOST_RELEASE_DIRS) $(HOST_RELEASE_LIB)

wasm.lib.release: $(WASM_RELEASE_DIRS) $(WASM_RELEASE_LIB)

#cortex M4 soft floating point targets
cortexmsoft.lib.release: $(CORTEXMSOFT_RELEASE_DIRS) $(CORTEXMSOFT_RELEASE_LIB)

#cortex M4F hard floating point targets
cortexmhard.lib.release: $(CORTEXMHARD_RELEASE_DIRS) $(CORTEXMHARD_RELEASE_LIB)

#build missing directories
$(HOST_CHECKED_DIRS) $(HOST_RELEASE_DIRS) $(TEST_DIRS):
	mkdir -p $@
$(CORTEXMSOFT_RELEASE_DIRS) $(CORTEXMHARD_RELEASE_DIRS):
	mkdir -p $@

#Checked library (used for testing)
$(HOST_CHECKED_LIB) : $(HOST_CHECKED_OBJECTS)
	$(AR) rcs $@ $(HOST_CHECKED_OBJECTS)

#Host release library
$(HOST_RELEASE_LIB) : $(HOST_RELEASE_OBJECTS)
	$(AR) rcs $@ $(HOST_RELEASE_OBJECTS)

$(WASM_RELEASE_LIB) : $(WASM_RELEASE_OBJECTS)
	$(WASM_RELEASE_AR) rcs $@ $(WASM_RELEASE_OBJECTS)

#Cortex-M4 softfp library
$(CORTEXMSOFT_RELEASE_LIB) : $(CORTEXMSOFT_RELEASE_OBJECTS)
	$(CORTEXMSOFT_RELEASE_AR) rcs $@ $(CORTEXMSOFT_RELEASE_OBJECTS)

#Cortex-M4 hardfp library
$(CORTEXMHARD_RELEASE_LIB) : $(CORTEXMHARD_RELEASE_OBJECTS)
	$(CORTEXMHARD_RELEASE_AR) rcs $@ $(CORTEXMHARD_RELEASE_OBJECTS)

#Google Test object
$(GTEST_OBJ): $(GTEST_DIR)/src/gtest-all.cc
	mkdir -p $(dir $@)
	$(HOST_RELEASE_CXX) $(TEST_CXXFLAGS) -c -o $@ $<

#Test build objects
$(TEST_BUILD_DIR)/%.o: $(TESTDIR)/%.cpp
	mkdir -p $(dir $@)
	$(HOST_RELEASE_CXX) $(TEST_CXXFLAGS) -c -o $@ $<

#Host checked build objects
$(HOST_CHECKED_BUILD_DIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(HOST_CHECKED_CC) $(HOST_CHECKED_CFLAGS) -c -o $@ $<

#Host release build objects
$(HOST_RELEASE_BUILD_DIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(HOST_RELEASE_CC) $(HOST_RELEASE_CFLAGS) -c -o $@ $<

#Cortex-M soft float release build objects
$(CORTEXMSOFT_RELEASE_BUILD_DIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CORTEXMSOFT_RELEASE_CC) $(CORTEXMSOFT_RELEASE_CFLAGS) -c -o $@ $<

#Cortex-M hard float release build objects
$(CORTEXMHARD_RELEASE_BUILD_DIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CORTEXMHARD_RELEASE_CC) $(CORTEXMHARD_RELEASE_CFLAGS) -c -o $@ $<

$(WASM_RELEASE_BUILD_DIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(WASM_RELEASE_CC) $(WASM_RELEASE_CFLAGS) -c -o $@ $<


test: $(TEST_DIRS) host.lib.checked $(TESTLIBVPR)
	LD_LIBRARY_PATH=$(TOOLCHAIN_DIR)/host/lib:$(TOOLCHAIN_DIR)/host/lib64:$(LD_LIBRARY_PATH) $(TESTLIBVPR)

clean:
	rm -rf $(BUILD_DIR)

$(TESTLIBVPR): $(HOST_CHECKED_OBJECTS) $(TEST_OBJECTS) $(GTEST_OBJ)
	find $(TEST_BUILD_DIR) -name "*.gcda" -exec rm {} \; -print
	rm -f gtest-all.gcda
	$(HOST_RELEASE_CXX) $(TEST_CXXFLAGS) -fprofile-arcs \
	    -o $@ $(TEST_OBJECTS) \
	    $(HOST_CHECKED_OBJECTS) $(GTEST_OBJ) -lpthread \
	    -L $(TOOLCHAIN_DIR)/host/lib64 -lstdc++

model-check:
	$(foreach n, $(MODEL_MAKEFILES), (cd models && $(MAKE) -f $(n)) &&) true
