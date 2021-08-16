CXX=clang++
RM=rm -f
CP=cp
CP_DIR=cp -a
MKDIR=mkdir -p
CPPFLAGS=-std=c++17


PROJECT=tarea-1
SOURCES_ROOT=source
SOURCES_DIR=$(SOURCES_ROOT)/src
INCLUDE_DIR=$(SOURCES_ROOT)/include
INCLUDE_SRC=$(INCLUDE_DIR)/vector.hpp
SOURCES_SRC=$(SOURCES_DIR)/source.cpp
INC_PARAMS=$(INCLUDE_DIR:%=-I%)
BIN_OUTPUT=build-out/bin/

all: program

program: configure
	$(CXX) -o $(BIN_OUTPUT)$(PROJECT) $(SOURCES_SRC) $(CPPFLAGS) $(INC_PARAMS)

configure:
	@echo configuring project
	@mkdir -p build-out/build
	@mkdir -p build-out/bin

check: post_setup
	cd $(BIN_OUTPUT) && ./$(PROJECT)

post_setup:
	chmod +x ./$(BIN_OUTPUT)$(PROJECT)
	$(CP_DIR) assets/. $(BIN_OUTPUT)

clean:
	@echo cleaning directory
	@$(RM) -rf build-out

clean-cmake:
	@echo cleaning cmake directory
	@$(RM) -rf build
	@$(RM) -rf out

clean-cache:
	@echo cleaning cache
	@$(RM) -rf .cache
	@$(RM) -rf .vs

clean-all: clean clean-cmake clean-cache

