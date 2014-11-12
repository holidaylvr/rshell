COMPILE=g++ 
#
FLAGS=-W -Wall -Werror -ansi -pedantic 
#
#



all: rshell cp ls

rshell: src/rshell.cpp
	([ ! -d bin/ ] && mkdir bin) || [ -d bin/ ] 
	$(COMPILE) $(FLAGS) src/rshell.cpp -o bin/rshell
cp: src/cp.cpp
	([ ! -d bin/ ] && mkdir bin) || [ -d bin/ ]
	$(COMPILE) $(FLAGS) src/cp.cpp -o bin/cp
ls: src/ls.cpp
	([ ! -d bin/ ] && mkdir bin) || [ -d bin/ ]
	$(COMPILE) $(FLAGS) src/ls.cpp -o bin/ls
clean: 
	rm -rf *.o

