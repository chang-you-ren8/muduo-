CXX=g++
CXXFLAGS=-g -Wall -std=c++11
LDFLAGS=-lpthread

MKDIR=mkdir -p  
RM= rm -rf 

BIN_DIR=./bin
OBJ_DIR=./obj

all_bin=test
all_bin:=$(addprefix $(BIN_DIR)/, $(all_bin))

.PHONY:all 
all:$(all_bin)

objs=test.o Thread.o CountDownLatch.o
objs:=$(addprefix $(OBJ_DIR)/, $(objs))
$(BIN_DIR)/test:$(objs)
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o:%.cc
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY:clean
clean:
	$(RM) $(OBJ_DIR) $(BIN_DIR)