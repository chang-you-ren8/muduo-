CXX=g++
CXXFLAGS=-Wall -g -std=c++11 -I../
LDFLAGS=-L./lib
LIBS=-lbase -lpthread
LIBARAY=./lib/libbase.a

RM=rm -rf 
MKDIR=mkdir -p
AR=ar r

SRC_DIR=../
OBJ_DIR=./obj
BIN_DIR=./bin
LIB_DIR=./lib

bins=Singleton_test CountDownLatch_test
bins:=$(addprefix $(BIN_DIR)/, $(bins))
all:$(bins)
.PHONY:all 

#奇怪了，为什么我这个makefile直接把Signleton_test.o删除了，
#而且再次make时居然不会重新生成??
$(BIN_DIR)/%:$(OBJ_DIR)/%.o $(LIBARAY)
	@$(MKDIR) $(dir $@)
	$(CXX) -g -o $@ $< $(LDFLAGS) $(LIBS)

$(OBJ_DIR)/%.o:%.cc 
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cc
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

SRCS=$(wildcard $(SRC_DIR)/*.cc)
SRCS:=$(notdir $(SRCS))
OBJS=$(patsubst %.cc, %.o, $(SRCS))
OBJS:=$(addprefix $(OBJ_DIR)/, $(OBJS))
$(LIBARAY):$(OBJS)
	@$(MKDIR) $(dir $@)
	@$(AR) $@ $^


.PHONY:clean
clean:
	$(RM) $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR)
