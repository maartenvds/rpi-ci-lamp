LIBDIR = ../lib
CFLAGS += -I$(LIBDIR)

all: test_https_request

test_https_request: test_https_request.o $(LIBDIR)/$(LIBCILAMP)

clean:
	rm -rf test_https_request *.o

.PHONY: clean all

