LIBDIR = ../lib
CFLAGS += -DTRAVIC_CI_SERVER_URI_OPEN_SOURCE=localhost:4443
CFLAGS += -DTRAVIC_CI_SERVER_URI_PRO=localhost:4443
CFLAGS += -I$(LIBDIR)
CFLAGS += $(shell pkg-config --cflags cmocka)
LDLIBS += $(shell pkg-config --libs cmocka)

all: test_settings_parser \
	test_response_parser \
	test_lamp_control_red_green \
	test_lamp_io_red_green \
	test_system

test_response_parser: test_response_parser.o $(LIBDIR)/$(LIBCILAMP)
test_lamp_control_red_green: test_lamp_control_red_green.o $(LIBDIR)/$(LIBCILAMP)
test_lamp_io_red_green: test_lamp_io_red_green.o $(LIBDIR)/$(LIBCILAMP)
test_settings_parser: test_settings_parser.o $(LIBDIR)/$(LIBCILAMP)
test_system: test_system.c $(LIBDIR)/$(LIBCILAMP)

clean:
	rm -rf *.o *.gcda *.gcno test_settings_parser \
						test_response_parser \
						test_lamp_control_red_green \
						test_lamp_io_red_green \
						test_system

.PHONY: clean all

