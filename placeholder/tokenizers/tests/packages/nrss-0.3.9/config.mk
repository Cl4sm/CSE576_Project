VERSION = 0.3.9

# Customize the following


# Define where everything should be installed.

ifndef PREFIX
  PREFIX = /usr/local
endif

ifndef MANPREFIX
  MANPREFIX = ${PREFIX}/share/man
endif

# Define your ncurses library, usu. ncurses, but
# also can be ncursesw if built with unicode

ifeq (${shell find /usr/include/* -name ncursesw},/usr/include/ncursesw)
	CURSDEF = NCURSESW
	CURSES = ncursesw
	PANEL = panelw
else
	CURSDEF = NCURSES
	CURSES = ncurses
	PANEL = panel
endif

INCS = -I. -I/usr/include -I/usr/local/include
LIBS = -L/usr/lib -L/usr/local/lib

CFLAGS += -g -Wall -std=gnu99 ${INCS} -DVERSION=\"${VERSION}\" -D${CURSDEF}

WGET = ${shell which wget}
ifneq (${WGET},"wget not found")
	CFLAGS += -DWGET_PATH=\"${WGET}\"
endif


LIBS += -lexpat -l${CURSES} -l${PANEL}


LDFLAGS += ${LIBS}

CC = cc

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

