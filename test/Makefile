CXX=g++
CXXFLAGS= -I../ -Wall -g -std=c++11
LDFLAGS=-L./lib
LIBS=-lnet

MKDIR=mkdir -p 
RM=rm -rf
AR=ar r

OBJ_DIR=./obj
LIB_DIR=./lib
BIN_DIR=./bin
SRC_DIR=../
LIBRARYS=$(LIB_DIR)/libnet.a

all:$(BIN_DIR)/EchoServer_test $(LIBRARYS)
.PHONY:all

$(BIN_DIR)/EchoServer_test:$(OBJ_DIR)/EchoServer_test.o
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $< $(LDFLAGS) $(LIBS)
$(OBJ_DIR)/EchoServer_test.o:EchoServer_test.cc 
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $< $(CXXFLAGS)

SRCS=$(wildcard $(SRC_DIR)/*)
SRCS:=$(notdir $(SRCS))
OBJS=$(patsubst %.cc, %.o, $(SRCS))
OBJS:=$(addprefix $(OBJ_DIR)/, $(OBJS))
$(LIBRARYS):$(OBJS)
	@$(MKDIR) $(dir $@)
	@$(AR) $@ $^
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cc
	@$(MKDIR) $(dir $@)
	$(MKDIR) -o $@ $< $(CXXFLAGS)

.PHONY:clean
clean:
	$(RM) $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR)