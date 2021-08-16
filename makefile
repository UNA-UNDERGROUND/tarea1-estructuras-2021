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
BUILD_OUT_ROOT=build-out
SOURCE_OUT_ROOT=$(BUILD_OUT_ROOT)/build
#si quisieramos podemos especificar rutas separadas para los assets
BIN_OUTPUT=$(BUILD_OUT_ROOT)/$(PROJECT)/
ASSETS_OUTPUT=$(BUILD_OUT_ROOT)/$(PROJECT)/

all: configure program post-setup

program:
	$(CXX) -o $(BIN_OUTPUT)$(PROJECT) $(SOURCES_SRC) $(CPPFLAGS) $(INC_PARAMS)

configure:
	@echo configuring project
	@mkdir -p $(SOURCE_OUT_ROOT)
	@mkdir -p $(BIN_OUTPUT)
	@mkdir -p $(ASSETS_OUTPUT)

check: post-setup
	cd $(BIN_OUTPUT) && ./$(PROJECT)

post-setup:
	chmod +x ./$(BIN_OUTPUT)$(PROJECT)
	$(CP_DIR) assets/. $(ASSETS_OUTPUT)

clean:
	@echo cleaning directory
	@$(RM) -rf $(BUILD_OUT_ROOT)

clean-cmake:
	@echo cleaning cmake directory
	@$(RM) -rf build
	@$(RM) -rf out

clean-cache:
	@echo cleaning cache
	@$(RM) -rf .cache
	@$(RM) -rf .vs

clean-all: clean clean-cmake clean-cache

