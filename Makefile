include ./Make.defines

all: main.o
	cd ${LIB_DIR} && make
	${CC} -o ${PROG} $< ${LIBS}

main.o: main.c
	${CC} ${CFLAGS} -c $<

.PHONY: clean
clean:
	cd ${LIB_DIR} && make clean
	rm -f *.a ${CLEANFILES} ${PROG}
