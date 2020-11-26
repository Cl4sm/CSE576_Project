#This system type borrowed from dwm

include config.mk

VPATH += cnf

OBJ = args.o commands.o conf.o feeds.o filter.o interface.o \
	  interface_draw.o list.o log.o main.o parse.o reader.o system.o \
	  usage.o utility.o cnf.o cache.o keys.o

all: verbiage nrss

verbiage:
	@echo CFLAGS = -v ${CFLAGS}
	@echo LDFLAGS = ${LDFLAGS}

nrss: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f *.o nrss

install: nrss
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f nrss ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/nrss
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < nrss.1 > ${DESTDIR}${MANPREFIX}/man1/nrss.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/nrss.1

uninstall:
	@rm -f ${DESTDIR}${PREFIX}/bin/nrss
	@rm -f ${DESTDIR}${MANPREFIX}/man1/nrss.1

.PHONY: all clean install uninstall
