LIBSRC=$(wildcard *.c)
LIBOBJ=$(LIBSRC:.c=.o)
JSMNDIR = ../third_party/jsmn
CFLAGS += -I$(JSMNDIR)

ifeq ($(NOWIRINGPI), 1)
	# include dummy wiringPi.h header file
	CFLAGS += -I../test
endif


$(LIBCILAMP): $(LIBOBJ)
	ar rcs $@ $(LIBOBJ)

clean:
	rm -rf $(LIBOBJ) $(LIBCILAMP) *.gcda *.gcno

.PHONY: clean
