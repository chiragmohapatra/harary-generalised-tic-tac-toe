objs = generalised_tic_tac_toe.o pn_search_unified.o minimax.o
srcs = generalised_tic_tac_toe.cpp pn_search_unified.cpp minimax.cpp
lib = libgeneralised_tic_tac_toe.a libminimax.a

.PHONY: all $(objs) $(lib) clean $(srcs)
all : $(objs) $(lib) $(srcs)
	g++ pn_search_unified.o -o exec -L. -lgeneralised_tic_tac_toe -lminimax

$(objs) : $(srcs)
	g++ -c generalised_tic_tac_toe.cpp -o generalised_tic_tac_toe.o
	g++ -c minimax.cpp -o minimax.o
	g++ -c pn_search_unified.cpp -o pn_search_unified.o


$(lib) : generalised_tic_tac_toe.o minimax.o
	ar cr libgeneralised_tic_tac_toe.a generalised_tic_tac_toe.o
	ar cr libminimax.a minimax.o

clean : 
	rm *.o *.a exec

execute:
	./exec
