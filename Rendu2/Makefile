#example name
NAME=expr
#compiler name
CC=clang++
#lex name+path
LEX=flex
LIBLEX=-lfl
#yacc/bison
YACC=bison

all: Bison/$(NAME).tab.cpp Bison/$(NAME).yy.c Bison/$(NAME).cpp Sources/main.cpp
	$(CC) -g -std=c++11 -pg -Wall -Wextra -Wpedantic -Wno-write-strings -I ./Headers -o resol $^ $(LIBLEX)

Bison/$(NAME).yy.c :  Bison/$(NAME).l
	$(LEX)  -o $@ $^

Bison/$(NAME).tab.cpp : Bison/$(NAME).ypp
	$(YACC) --report=all -o $@ -d $^

clean:
	rm -f *~ Bison/$(NAME).yy.c Bison/$(NAME).tab.cpp Bison/$(NAME).tab.hpp Bison/$(NAME).output
