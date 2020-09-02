# final_target: sub_target final_target.c
#	 Recipe_to_create_final_target
#
# sub_target: sub_target.c
#	 Recipe_to_create_sub_target
#
.PHONY: all syntactic_analyzer lexical_analyzer clean

CC 		= gcc
LEXA 	= flex
SYNA 	= bison

OUTPUT_BIN = tdsc

SRCDIR = src

all: syntactic_analyzer
	@echo "Building tdsc"
	${CC} ${SRCDIR}/syntactic_analyzer.c ${SRCDIR}/lexical_analyzer.c -o ${OUTPUT_BIN}

syntactic_analyzer: lexical_analyzer
	@echo "Compiling syntactic analyzer"
	${SYNA} -d -o ${SRCDIR}/$@.c ${SRCDIR}/01_$@.y

lexical_analyzer: 
	@echo "Compiling lexical analyzer"
	${LEXA} -o ${SRCDIR}/$@.c ${SRCDIR}/00_$@.l

clean:
	@echo "Cleaning up..."
	rm ${OUTPUT_BIN}
	rm ${SRCDIR}/*.c 
	rm ${SRCDIR}/*.h

