all: lab7


WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2
GMPLIB = -lgmp


lab7: Makefile main.c
	$(CC) -o ./exec/$@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) main.c $(GMPLIB)

clean:
	rm -f lab7

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	exec/lab7

