LIBDIR = ../lib
ifeq ($(NOWIRINGPI), 0)
	LDLIBS += -lwiringPi -lrt -lm -lcrypt
endif
JSMNDIR = ../third_party/jsmn
CFLAGS += -I$(LIBDIR) -I$(JSMNDIR)

all: rpi-ci-lamp test_lamp_io_red_green

rpi-ci-lamp: rpi-ci-lamp.o $(LIBDIR)/$(LIBCILAMP) $(JSMNDIR)/$(LIBJSMN)
test_lamp_io_red_green:	test_lamp_io_red_green.o $(LIBDIR)/$(LIBCILAMP)

clean:
	rm -rf rpi-ci-lamp test_lamp_io_red_green *.o *.gcno *.gcda

.PHONY: clean

