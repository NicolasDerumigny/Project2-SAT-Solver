#example name
NAME=expr
#compiler name
CC=clang++
#lex name+path
LEX=flex
LIBLEX=-lfl
#yacc/bison
YACC=bison


all: Bison/$(NAME).tab.cpp Bison/$(NAME).yy.cpp Bison/$(NAME).cpp Sources/main.cpp
	$(CC) -g -std=c++11 -Wall -Wextra -Wpedantic -Wno-write-strings -I ./Headers -O3 -o resol $^ $(LIBLEX)
	

Bison/$(NAME).yy.cpp :  Bison/$(NAME).l
	$(LEX)  -w -o $@ $^

Bison/$(NAME).tab.cpp : Bison/$(NAME).ypp
	$(YACC) --report=all -o $@ -d $^

clean:
	rm -f *~ Bison/$(NAME).yy.cpp Bison/$(NAME).tab.cpp Bison/$(NAME).tab.hpp Bison/$(NAME).output
