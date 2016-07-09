include ../Make.defines

all: ${LIB_OBJS}
	ar rv ${LIBIM_NAME} $?
	${RANLIB} ${LIBIM_NAME}

clean:
	rm -f ${CLEANFILES}
