LIBSRC=$(wildcard *.c)
LIBOBJ=$(LIBSRC:.c=.o)
JSMNDIR = ../third_party/jsmn
CFLAGS += -I$(JSMNDIR)

$(LIBCILAMP): $(LIBOBJ)
	ar rcs $@ $(LIBOBJ)

clean:
	rm -rf $(LIBOBJ) $(LIBCILAMP) *.gcda *.gcno

.PHONY: clean
