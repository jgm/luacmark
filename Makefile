include config.mk

SRC = ext/blocks.c ext/buffer.c ext/cmark.c ext/html/houdini_href_e.c ext/html/houdini_html_e.c ext/html/houdini_html_u.c ext/html/html.c ext/inlines.c ext/node.c ext/print.c ext/references.c ext/scanners.c ext/utf8.c luacmark.c
OBJ = ${SRC:.c=.o}

all: cmark.so

.c.o:
	${CC} -c -o $@ ${CFLAGS} $<

${OBJ}: config.mk

cmark.so: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f cmark.so ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/lib/lua/${LUA}
	cp -f cmark.so ${DESTDIR}${PREFIX}/lib/lua/${LUA}/

test: all
	./test.lua

.PHONY: all clean install test
