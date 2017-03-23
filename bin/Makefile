LIBDIR = ../lib
JSMNDIR = ../third_party/jsmn
CFLAGS += -I$(LIBDIR) -I$(JSMNDIR)

all: test_https_request

test_https_request: test_https_request.o $(LIBDIR)/$(LIBCILAMP) $(JSMNDIR)/$(LIBJSMN)

clean:
	rm -rf test_https_request *.o

.PHONY: clean all

