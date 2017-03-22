LIBSRC=$(wildcard *.c)
LIBOBJ=$(LIBSRC:.c=.o)

$(LIBCILAMP): $(LIBOBJ)
	ar rcs $@ $(LIBOBJ)

clean:
	rm -rf $(LIBOBJ) $(LIBCILAMP)

.PHONY: clean
