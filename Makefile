# final_target: sub_target final_target.c
#	 Recipe_to_create_final_target
#
# sub_target: sub_target.c
#	 Recipe_to_create_sub_target
#
.PHONY: all syntactic_analyzer lexical_analyzer clean

CC = gcc
LEXA = flex
SYNA = bison

# SRCS := $(wildcard *.c)
# BINS := $(SRCS:%.c=%)

# SUBDIRS = \
# 	00_lexical_analysis \
# 	01_syntactic_analysis \
# 	src \
# 	doc \
# 	tests

all: tdsc

tdsc: syntactic_analyzer
	${CC} src/syntactic_analyzer.c src/lexical_analyzer.c -o tdsc

syntactic_analyzer: lexical_analyzer
	@echo "Compiling syntactic analyzer"
	${SYNA} -d -o src/syntactic_analyzer.c src/01_syntactic_analyzer.y

lexical_analyzer:
	@echo "Compiling lexical analyzer"
	${LEXA} -o src/lexical_analyzer.c src/00_lexical_analyzer.l

# say_hello:
# 	@echo "Hello World"

# generate:
# 	@echo "Creating empty text files..."
# 	touch file-{1..10}.txt

clean:
	@echo "Cleaning up..."
	rm tdsc
	rm src/*.c 
	rm src/*.h

