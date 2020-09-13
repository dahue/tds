# final_target: sub_target final_target.c
#	 Recipe_to_create_final_target
#
# sub_target: sub_target.c
#	 Recipe_to_create_sub_target
#
##################################################
# all: lang
#
# lang.tab.c lang.tab.h:	lang.y
# 	bison -d lang.y
#
# lex.yy.c: lang.l lang.tab.h
# 	flex lang.l
#
# lang: lex.yy.c lang.tab.c lang.tab.h
# 	g++ -o lang lang.tab.c lex.yy.c
#
# clean:
# 	rm lang lang.tab.c lex.yy.c lang.tab.h
#

.PHONY: all tdsc syntactic_analyzer lexical_analyzer clean

CC 		= gcc
LEXA 	= flex
SYNA 	= bison

SRCDIR = src

all: tdsc

tdsc: syntactic_analyzer
	@echo "Building tdsc"
	${CC} ${SRCDIR}/syntactic_analyzer.c ${SRCDIR}/lexical_analyzer.c -o $@

syntactic_analyzer: lexical_analyzer
	@echo "Compiling syntactic analyzer"
	${SYNA} -d -o ${SRCDIR}/$@.c ${SRCDIR}/01_$@.y

lexical_analyzer: 
	@echo "Compiling lexical analyzer"
	${LEXA} -o ${SRCDIR}/$@.c ${SRCDIR}/00_$@.l

clean:
	@echo "Cleaning up..."
	# Delete .c files
	rm -f ${SRCDIR}/lexical_analyzer.c ${SRCDIR}/syntactic_analyzer.c

	# Delete .h files
	rm -f ${SRCDIR}/syntactic_analyzer.h

	# Delete .o files
	rm -f ${SRCDIR}/lexical_analyzer.o ${SRCDIR}/syntactic_analyzer.o

	# Delete executable file
	rm -f tdsc
