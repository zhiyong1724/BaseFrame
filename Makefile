BUILD_DIR := build
OBJ_DIR	:= $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
DEMO_FILE := $(BIN_DIR)/demo

CXX = g++

vpath %.o $(OBJ_DIR)

INCLUDES := -I.

CXXFLAGS += -Wall -Werror -std=c++17 -g

SRCS := main.cpp \
	Handler.cpp \
	MessageQueue.cpp \
	ActivityManager.cpp

BASE_OBJS := $(patsubst %.cpp, %.o, $(SRCS))
OBJS := $(addprefix $(OBJ_DIR)/,$(BASE_OBJS))

all : $(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR) $(DEMO_FILE)

$(DEMO_FILE) : $(BASE_OBJS)
	@echo CXX $(DEMO_FILE)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(DEMO_FILE) $(INCLUDES)
	@ls -l $(BIN_DIR)

$(BASE_OBJS) : %.o : %.cpp
	@echo CXX $<
	@$(CXX) $(CXXFLAGS) -c $< -o $(OBJ_DIR)/$@ $(INCLUDES)

$(BUILD_DIR) :
	mkdir $(BUILD_DIR)
	
$(OBJ_DIR) :
	mkdir $(OBJ_DIR)
	
$(BIN_DIR) :
	mkdir $(BIN_DIR)
	
.PHONY : clean

clean :
	rm -rf $(BUILD_DIR)

	
