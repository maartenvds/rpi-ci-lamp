CC=gcc
LDLIBS+= -lssl -lcrypto
CFLAGS+= -I/usr/include/openssl
CFLAGS+= -D_GNU_SOURCE
CFLAGS+= -Wall -W -Wextra
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

all: test bin

$(LIBCILAMP):
	make -C lib/

$(LIBJSMN):
	make -C third_party/jsmn/

test: $(LIBCILAMP) $(LIBJSMN)
	make -C test/

bin: $(LIBCILAMP) $(LIBJSMN)
	make -C bin/

clean:
	make -C third_party/jsmn/ clean
	make -C lib/ clean
	make -C test/ clean
	make -C bin/ clean

.PHONY: all test clean
