# Makefile for the Warehouse Management System project

# Compiler settings - Can be customized.
CC=g++
CFLAGS=-g -Wall -Weffc++ -std=c++11 -Iinclude

# Define the binary and source directories.
BIN_DIR=bin
SRC_DIR=src
INCLUDE_DIR=include
CONFIG_DIR=$(BIN_DIR)/rest

# Define the final target name.
TARGET=$(BIN_DIR)/warehouse

# Define source files.
SOURCES=$(SRC_DIR)/main.cpp $(SRC_DIR)/Action.cpp $(SRC_DIR)/Customer.cpp $(SRC_DIR)/Volunteer.cpp $(SRC_DIR)/Order.cpp $(SRC_DIR)/Parser.cpp $(SRC_DIR)/WareHouse.cpp

# Define object files.
OBJECTS=$(SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Config file
CONFIG_FILE=$(CONFIG_DIR)/input_file.txt

# Default make target.
all: clean $(TARGET) post-build

# Linking the executable.
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compiling source files.
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Post-build actions
post-build:
	mkdir -p $(CONFIG_DIR)
	[ -f $(CONFIG_FILE) ] || cp input_file.txt $(CONFIG_DIR)/

# Cleaning up the build.
clean:
	find $(BIN_DIR) -type f -not -path "$(CONFIG_DIR)/*" -delete

# Phony targets for clean and all.
.PHONY: all clean post-build
