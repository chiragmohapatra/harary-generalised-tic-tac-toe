dirs = util bin obj output
objs = obj/generalised_tic_tac_toe.o obj/pn_search.o
srcs = src/generalised_tic_tac_toe.cpp src/pn_search.cpp
lib = util/libgeneralised_tic_tac_toe.a

.PHONY: all $(dirs) $(objs) $(lib) clean $(srcs)
all : $(dirs) $(objs) $(lib) $(srcs)
	g++ obj/pn_search.o -o bin/exec -L./util -lgeneralised_tic_tac_toe

$(dirs) :
	-mkdir $@

$(objs) : $(srcs)
	g++ -c src/generalised_tic_tac_toe.cpp -o obj/generalised_tic_tac_toe.o
	g++ -c src/pn_search.cpp -o obj/pn_search.o


$(lib) : obj/generalised_tic_tac_toe.o
	ar cr util/libgeneralised_tic_tac_toe.a obj/generalised_tic_tac_toe.o

clean : 
	-rm -r bin
	-rm -r util
	-rm -r obj
	-rm -r output

execute:
	./bin/exec