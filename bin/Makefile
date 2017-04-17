LIBDIR = ../lib
ifeq ($(NOWIRINGPI), 0)
	LDLIBS += -lwiringPi -lrt -lm -lcrypt
endif
JSMNDIR = ../third_party/jsmn
CFLAGS += -I$(LIBDIR) -I$(JSMNDIR)

all: rpi-ci-lamp

rpi-ci-lamp: rpi-ci-lamp.o $(LIBDIR)/$(LIBCILAMP) $(JSMNDIR)/$(LIBJSMN)

clean:
	rm -rf rpi-ci-lamp *.o *.gcno *.gcda

.PHONY: clean

