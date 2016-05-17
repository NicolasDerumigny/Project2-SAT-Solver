#example name
NAME=expr
#compiler name
CXX=clang++
#lex name+path
LEX=flex
LIBLEX=-lfl
#yacc/bison
YACC=bison
#flags
FLAGXX= -std=c++11
CFLAGS= -g -pg $(FLAGXX) -Wall -Wextra -Wpedantic -Wno-write-strings -I Header/
#CFLAGS= -g -pg -fprofile-arcs -ftest-coverage -lgcov -coverage $(FLAGXX) -Wall -Wextra -Wpedantic -Wno-write-strings -I Header/
WFLAG= -Weverything
SRC= $(wildcard Sources/*.cpp)
BIZ= $(wildcard Bison/*.cpp)
OBJ= $(patsubst Sources/%.cpp, Build/%.o, $(SRC))


all: Build/$(NAME).o Build/$(NAME).tab.o Build/$(NAME).yy.o $(OBJ)
	$(CXX) $(CFLAGS) $(WFLAG) -o resol $^ $(LIBLEX) -lpthread -pthread

Build/$(NAME).tab.o: Bison/$(NAME).tab.cpp
	$(CXX) $(CFLAGS) -o $@ -c $< $(CFLAGS) 

Build/$(NAME).yy.o: Bison/$(NAME).yy.cpp
	$(CXX) $(CFLAGS) -o $@ -c $< $(CFLAGS) 

Build/$(NAME).o: Bison/$(NAME).cpp
	$(CXX) $(CFLAGS) $(WFLAG) -o $@ -c $< $(CFLAGS) 

Bison/$(NAME).yy.cpp :  Bison/$(NAME).l
	$(LEX)  -w -o $@ $^

Bison/$(NAME).tab.cpp : Bison/$(NAME).ypp
	$(YACC) --report=all -o $@ -d $^

clean:
	rm -f *~ Bison/$(NAME).yy.cpp Bison/$(NAME).tab.cpp Bison/$(NAME).tab.hpp Bison/$(NAME).output Build/*.o

Build/%.o: Sources/%.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(WFLAG)

Build/assignation.o: Sources/assignation.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(WFLAG) -pthread -lpthread
