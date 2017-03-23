CC=gcc
LDLIBS+=-lssl -lcrypto
CFLAGS+=-I/usr/include/openssl
CFLAGS+=-Wall
LDFLAGS+=
LIBCILAMP=libcilamp.a
LIBJSMN=libjsmn.a

ifeq ($(DEBUG), 1)
    CMNFLAGS = -fno-omit-frame-pointer -fsanitize=address
    CFLAGS  += -DDEBUG -g $(CMNFLAGS)
    LDFLAGS  += $(CMNFLAGS)
else
    CFLAGS += -O2
endif

export CFLAGS LDFLAGS LIBCILAMP LIBJSMN LDLIBS CC

all: test

$(LIBCILAMP):
	make -C lib/

$(LIBJSMN):
	make -C third_party/jsmn/

test: $(LIBCILAMP) $(LIBJSMN)
	make -C test/

clean:
	make -C third_party/jsmn/ clean
	make -C lib/ clean
	make -C test/ clean

.PHONY: all test clean
