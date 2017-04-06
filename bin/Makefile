LIBDIR = ../lib
JSMNDIR = ../third_party/jsmn
CFLAGS += -I$(LIBDIR) -I$(JSMNDIR)

all: main test_lamp_io_red_green

main: main.o $(LIBDIR)/$(LIBCILAMP) $(JSMNDIR)/$(LIBJSMN)
test_lamp_io_red_green:	test_lamp_io_red_green.o $(LIBDIR)/$(LIBCILAMP)

clean:
	rm -rf main test_lamp_io_red_green *.o *.gcno *.gcda

.PHONY: clean

