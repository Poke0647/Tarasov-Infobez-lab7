all: lab7


WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2
GMPLIB = -lgmp


lab7: Makefile main.c
	$(CC) -o ./$@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) main.c $(GMPLIB)

clean:
	rm -f lab7

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./lab7 1024

