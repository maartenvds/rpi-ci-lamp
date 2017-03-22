CC=gcc
LDLIBS+=-lssl -lcrypto
CFLAGS+=-I/usr/include/openssl
CFLAGS+=-Wall
LDFLAGS+=
LIBCILAMP=libcilamp.a

ifeq ($(DEBUG), 1)
    CMNFLAGS = -fno-omit-frame-pointer -fsanitize=address
    CFLAGS  += -DDEBUG -g $(CMNFLAGS)
    LDFLAGS  += $(CMNFLAGS)
else
    CFLAGS += -O2
endif

export CFLAGS LDFLAGS LIBCILAMP LDLIBS CC

all: test

lib:
	make -C lib/

test: lib
	make -C test/

clean:
	make -C lib/ clean
	make -C test/ clean

.PHONY: all lib test clean
