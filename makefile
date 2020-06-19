dirs = util bin obj output
objs = obj/generalised_tic_tac_toe.o obj/pn_search_unified.o obj/minimax.o
srcs = src/generalised_tic_tac_toe.cpp src/pn_search_unified.cpp src/minimax.cpp
lib = util/libgeneralised_tic_tac_toe.a util/libminimax.a

.PHONY: all $(dirs) $(objs) $(lib) clean $(srcs)
all : $(dirs) $(objs) $(lib) $(srcs)
	g++ obj/pn_search_unified.o -o bin/exec -L./util -lgeneralised_tic_tac_toe -lminimax

$(dirs) :
	-mkdir -p $@

$(objs) : $(srcs)
	g++ -c src/generalised_tic_tac_toe.cpp -o obj/generalised_tic_tac_toe.o
	g++ -c src/minimax.cpp -o obj/minimax.o
	g++ -c src/pn_search_unified.cpp -o obj/pn_search_unified.o


$(lib) : obj/generalised_tic_tac_toe.o obj/minimax.o
	ar cr util/libgeneralised_tic_tac_toe.a obj/generalised_tic_tac_toe.o
	ar cr util/libminimax.a obj/minimax.o

clean : 
	-rm -r bin
	-rm -r util
	-rm -r obj
	-rm -r output

execute:
	./bin/exec