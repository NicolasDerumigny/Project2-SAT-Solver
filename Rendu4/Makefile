#example name
NAME=expr
#compiler name
CC=clang++
#lex name+path
LEX=flex
LIBLEX=-lfl
#yacc/bison
YACC=bison
#flags
CXX= -std=c++11
CFLAGS= -g $(CXX) -Wall -Wextra -Wpedantic -Weverything -Wno-write-strings -I Header/
SRC= $(wildcard Sources/*.cpp)
OBJ= $(patsubst Sources/%.cpp, Build/%.o, $(SRC))


all: Bison/$(NAME).tab.cpp Bison/$(NAME).yy.cpp Bison/$(NAME).cpp $(OBJ)
	$(CC) $(CFLAGS) -o resol $^ $(LIBLEX)

Bison/$(NAME).yy.cpp :  Bison/$(NAME).l
	$(LEX)  -w -o $@ $^

Bison/$(NAME).tab.cpp : Bison/$(NAME).ypp
	$(YACC) --report=all -o $@ -d $^

clean:
	rm -f *~ Bison/$(NAME).yy.cpp Bison/$(NAME).tab.cpp Bison/$(NAME).tab.hpp Bison/$(NAME).output Build/*.o

Build/%.o: Sources/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS) 

