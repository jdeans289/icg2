CXX = clang++

CXXFLAGS = -g -Wall

CLANG_LIBS= -lclang

DATATYPES_HOME=/users/jndeans/icg_rebuild/DataTypes
DATATYPES_LIB_DIR = $(DATATYPES_HOME)/lib
DATATYPES_INCLUDES = $(DATATYPES_HOME)/include
DATATYPES_LIB = Decl
DATATYPES_LIB_FULL_PATH = $(DATATYPES_LIB_DIR)/lib$(DATATYPES_LIB).a
DATATYPES_FLAGS = -L$(DATATYPES_LIB_DIR) -l$(DATATYPES_LIB) -I$(DATATYPES_INCLUDES)

all: io_src_main

datatypes: $(DATATYPES_LIB_FULL_PATH)

.PHONY: $(DATATYPES_LIB_FULL_PATH)
$(DATATYPES_LIB_FULL_PATH):
	$(MAKE) -C $(DATATYPES_HOME)

clang-parse: clang-parse.cpp $(DATATYPES_LIB_FULL_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@ $(CLANG_LIBS) $(DATATYPES_FLAGS) 

io_src.hh: clang-parse
	./clang-parse

io_src_main: io_src_main.cpp io_src.hh $(DATATYPES_LIB_FULL_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@ $(DATATYPES_FLAGS) 
 
clean:
	rm -f clang-parse io_src_main io_src.hh