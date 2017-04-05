LIBDIR = ../lib
JSMNDIR = ../third_party/jsmn
CFLAGS += -I$(LIBDIR) -I$(JSMNDIR)
CFLAGS += $(shell pkg-config --cflags cmocka)
LDLIBS += $(shell pkg-config --libs cmocka)

all: test_settings_parser test_response_parser test_lamp_control_red_green test_application

test_response_parser: test_response_parser.o $(LIBDIR)/$(LIBCILAMP)
test_lamp_control_red_green: test_lamp_control_red_green.o $(LIBDIR)/$(LIBCILAMP)
test_settings_parser: test_settings_parser.o $(LIBDIR)/$(LIBCILAMP) $(JSMNDIR)/$(LIBJSMN)
test_application: test_application.o $(LIBDIR)/$(LIBCILAMP) $(JSMNDIR)/$(LIBJSMN)

clean:
	rm -rf *.o *.gcda *.gcno test_settings_parser test_response_parser \
						test_lamp_control_red_green	test_application

.PHONY: clean all

