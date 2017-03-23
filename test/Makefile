LIBDIR = ../lib
JSMNDIR = ../third_party/jsmn
CFLAGS += -I$(LIBDIR) -I$(JSMNDIR)
CFLAGS += $(shell pkg-config --cflags cmocka)
LDLIBS += $(shell pkg-config --libs cmocka)

all: test_settings_parser test_response_parser

test_settings_parser: test_settings_parser.o $(LIBDIR)/$(LIBCILAMP) $(JSMNDIR)/$(LIBJSMN)
test_response_parser: test_response_parser.o $(LIBDIR)/$(LIBCILAMP) $(JSMNDIR)/$(LIBJSMN)

clean:
	rm -rf test_settings_parser test_response_parser *.o

.PHONY: clean all

