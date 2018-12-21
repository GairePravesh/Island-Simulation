CXX = g++

SRC_DIR = src
MAIN = main.cpp
MAIN_FILE = $(SRC_DIR)/$(MAIN)

BUILD_DIR = build
OBJ_FILE = $(BUILD_DIR)/main.o
OUTPUT = app
OUT_FILE = $(BUILD_DIR)/$(OUTPUT)

LIBS = -lGL -lglut -lGLEW -lGLU

$(OUT_FILE): $(OBJ_FILE)
	@$(CXX) -o $@ $< $(LIBS)
	@./$(BUILD_DIR)/$(OUTPUT)

$(OBJ_FILE): $(MAIN_FILE)
	@$(CXX) -c $< -o $(OBJ_FILE)

clean:  
	@rm $(BUILD_DIR)/* -r
