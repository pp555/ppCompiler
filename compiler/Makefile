CC=gcc
CPP=g++
LEX=flex
YACC=bison
LD=gcc

all:	leks

leks:	def.tab.o lex.yy.o
	$(CPP) lex.yy.o def.tab.o -o leks -ll

lex.yy.o:	lex.yy.c
	$(CC) -c lex.yy.c

lex.yy.c: z3.l
#	$(LEX) -d z3.l
	$(LEX) z3.l

def.tab.o:	def.tab.cc
	$(CPP) -c def.tab.cc

def.tab.cc:	def.yy classes/*
	$(YACC) -d def.yy

clean:
	rm *.o leks def.tab.cc def.tab.hh lex.yy.c
