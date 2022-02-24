OBJ_DIR=./obj
BIN_DIR=./bin
LIB_DIR=./lib
SRC_DIR=../

CXX=g++
CXXFLAGS=-Wall -g -std=c++11 -I../ 
LDFLAGS=-L$(LIB_DIR)
LIBS=-lnet -lpthread

MKDIR=mkdir -p
RM=rm -rf
AR=ar r

all_bin=EventLoop_test client EventLoopThread_test EchoServer_test \
EchoClient_test

all_bin:=$(addprefix $(BIN_DIR)/, $(all_bin))
.PHONY:all
all:$(all_bin)

LIBFILES=$(LIB_DIR)/libnet.a

$(BIN_DIR)/%:$(OBJ_DIR)/%.o $(LIBFILES)
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $<	$(LDFLAGS) $(LIBS)

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
$(LIBFILES):$(OBJS)
	@$(MKDIR) $(dir $@)
	@$(AR) $@ $^

.PHONY:clean
clean:
	$(RM) $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR)
