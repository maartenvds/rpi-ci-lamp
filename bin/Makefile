LIBDIR = ../lib
ifeq ($(NOWIRINGPI), 0)
	LDLIBS += -lwiringPi -lrt -lm -lcrypt
endif
CFLAGS += -I$(LIBDIR)

all: rpi-ci-lamp

rpi-ci-lamp: rpi-ci-lamp.o $(LIBDIR)/$(LIBCILAMP)

clean:
	rm -rf rpi-ci-lamp *.o *.gcno *.gcda

.PHONY: clean

