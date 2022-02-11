SRC_DIR=./src
INCLUDE_DIR =./include
BIN_DIR=./bin
LIB_DIR=./lib
TEST_DIR=./test
OBJ_DIR=./obj

#LDFLAGS用来指明链接时应该查找哪些目录（自己写的库所在目录）
#LIBS用来指明链接时应该链接哪些库（自己实现的库）这里要写库名而不是库的文件名
#比如库的文件名是libutility.a，那么库名就是utility
CXX=g++
CXXFLAGS=-g -Wall -I$(INCLUDE_DIR)
LDFLAGS= -L$(LIB_DIR)
LIBS=-lutility

MKDIR=mkdir -p
RM=rm -rf 
AR=ar r 

STATIC_LIB=$(LIB_DIR)/libutility.a
all_bin=test
all_bin:=$(addprefix $(BIN_DIR)/, $(all_bin))

.PHONY:all 
all:$(STATIC_LIB) $(all_bin)

$(BIN_DIR)/test:$(OBJ_DIR)/test.o
	@$(MKDIR) $(dir $@) 
	$(CXX) -o $@ $< $(LDFLAGS) $(LIBS) 

#静态库中的所有.o对应src目录下面的.cc
srcs=$(wildcard $(SRC_DIR)/*)
srcs:=$(notdir $(srcs))
objs=$(patsubst %.cc, %.o, $(srcs))
objs:=$(addprefix $(OBJ_DIR)/, $(objs))
$(STATIC_LIB):$(objs)
	@$(MKDIR) $(dir $@)
	@$(AR) $@ $^
	## @$(RM) $^ -- 加上这句话就会导致无用的更新，如何解决？？

$(OBJ_DIR)/%.o:$(TEST_DIR)/%.cc 
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cc 
	@$(MKDIR) $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY:clean 
clean:
	@$(RM) $(BIN_DIR) $(LIB_DIR) $(OBJ_DIR)