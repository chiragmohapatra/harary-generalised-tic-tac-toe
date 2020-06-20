BIN_DIR := ./bin
INC_DIR := ./src/includes
LIB_DIR := ./lib
OBJ_DIR := ./obj
SRC_DIR := ./src
VPATH := $(SRC_DIR)
CXXFLAGS := -I $(INC_DIR) #-pedantic-errors -Wall -Wextra -Werror

SRC := $(SRC_DIR)/generalised_tic_tac_toe.cpp $(SRC_DIR)/pn_search_unified.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/minimax.cpp $(SRC_DIR)/monte_carlo_simulator.cpp

OBJECTS := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRC:%.cpp=%.o))
LIBS := $(LIB_DIR)/generalised_tic_tac_toe.a $(LIB_DIR)/minimax.a
HEADERS := $(INC_DIR)/generalised_tic_tac_toe.h $(INC_DIR)/minimax.h
EXEC := $(BIN_DIR)/exec

.PHONY: all clean execute
all: $(EXEC)
#	g++ pn_search_unified.o -o exec -L. -lgeneralised_tic_tac_toe -lminimax

$(BIN_DIR)/exec: $(OBJECTS) $(LIBS)
	@mkdir -p $(BIN_DIR)
	g++ $(CXXFLAGS) -o $(BIN_DIR)/exec $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	g++ $(CXXFLAGS) -c $< -o $@

$(LIB_DIR)/%.a: $(OBJ_DIR)/%.o
	@mkdir -p $(LIB_DIR)
	ar cr $@ $<

clean :
	$(RM) $(OBJECTS) $(LIBS) $(BIN_DIR)/exec

execute:
	$(BIN_DIR)/exec
