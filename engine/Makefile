all:
	gcc lcg_engine.c yaml_parse.c lcg_rules.c -g -o lcg_engine -llua -ldl -lm -lyaml
clean:
	rm lcg_engine

#objects = lcg_engine.o yaml_parse.o lcg_rules.o
#cc = gcc
#edit : $(objects)
#    cc -o lcg_engine $(objects) -llua  -lm
#
#lcg_engine.o : lcg_engine.c lcg_engine.h
#lcg_rules.o : lcg_rules.c   lcg_rules.h
#yaml_parse.o : yaml_parse.c   yaml_parse.h
#
#.PHONY : clean
#clean :
#    rm lcg_engine
#    	rm *.o