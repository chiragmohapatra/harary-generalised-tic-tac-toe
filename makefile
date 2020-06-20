CXXFLAGS := #-pedantic-errors -Wall -Wextra -Werror
OBJ_DIR := ./obj
LIB_DIR := ./lib
BIN_DIR := ./bin

SRC := generalised_tic_tac_toe.cpp pn_search_unified.cpp minimax.cpp

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
LIBS := $(LIB_DIR)/generalised_tic_tac_toe.a $(LIB_DIR)/minimax.a
HEADERS := generalised_tic_tac_toe.h minimax.h
EXEC := $(BIN_DIR)/exec

.PHONY: all clean execute
all: $(EXEC)
#	g++ pn_search_unified.o -o exec -L. -lgeneralised_tic_tac_toe -lminimax

$(BIN_DIR)/exec: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	g++ $(CXXFLAGS) -o $(BIN_DIR)/exec $^

$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	g++ $(CXXFLAGS) -c $< -o $@

$(LIB_DIR)/%.a: $(OBJ_DIR)/%.o
	@mkdir -p $(LIB_DIR)
	ar cr $@ $<

clean :
	$(RM) $(OBJECTS) $(LIBS) $(BIN_DIR)/exec

execute:
	$(BIN_DIR)/exec
