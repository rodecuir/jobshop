#A simple makefile is a unicorn.
CXX=g++
CXXFLAGS=-std=c++17

SHELL = /bin/sh
OUTPUT_DIR=output
SRC_DIR=./src
SOURCES = $(wildcard $(SRC_DIR)/*.cc)
OBJECTS = $(SOURCES:$(SRC_DIR)%.cc=$(OUTPUT_DIR)/%.o)

EXE=$(OUTPUT_DIR)/main

# TARGETS to test the program
# $@ is the incoming source filename
# $< is the output filename

# Compilación  +++++++++++++++++++++++++++++++++++++++
compile: $(OUTPUT_DIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJECTS)

$(OUTPUT_DIR)/%.o: $(SRC_DIR)%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
# ++++++++++++++++++++++++++++++++++++++++++++++++++++

# Ejecución ++++++++++++++++++++++++++++++++++++++++++
run: compile
	./$(EXE)
# ++++++++++++++++++++++++++++++++++++++++++++++++++++

${OUTPUT_DIR}: 
	mkdir -p $(OUTPUT_DIR)

.PHONY: clean
clean:
	rm -rf $(OUTPUT_DIR)
