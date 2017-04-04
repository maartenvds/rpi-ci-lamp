CC=gcc
LDLIBS+= -lssl -lcrypto
CFLAGS+= -I/usr/include/openssl
CFLAGS+= -D_GNU_SOURCE
CFLAGS+= -Wall -W -Wextra -Werror
LDFLAGS+=
LIBCILAMP=libcilamp.a
LIBJSMN=libjsmn.a
CMNFLAGS=

ifeq ($(DEBUG), 1)
    CMNFLAGS += -fno-omit-frame-pointer -fsanitize=address --coverage
    CFLAGS  += -DDEBUG -g
else
    CFLAGS += -O2
endif

CFLAGS += $(CMNFLAGS)
LDFLAGS += $(CMNFLAGS)

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

coverage:
	lcov --capture --rc lcov_branch_coverage=1 --directory . --follow --output-file coverage.info
	#genhtml coverage.info --rc genhtml_branch_coverage=1 --output-directory output

clean:
	make -C third_party/jsmn/ clean
	make -C lib/ clean
	make -C test/ clean
	make -C bin/ clean
	rm -rf coverage.info output

.PHONY: all test bin coverage clean
