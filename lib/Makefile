LIBSRC=$(wildcard *.c)
ifeq ($(NOWIRINGPI), 1)
	# include dummy wiringPi.h header file
	CFLAGS += -I.
else
	LIBSRC := $(filter-out wiringPi.c, $(LIBSRC))
endif

LIBOBJ=$(LIBSRC:.c=.o)
CFLAGS += -I../third_party/jsmn

$(LIBCILAMP): $(LIBOBJ)
	ar rcs $@ $(LIBOBJ)

clean:
	rm -rf $(LIBOBJ) $(LIBCILAMP) *.gcda *.gcno

.PHONY: clean
