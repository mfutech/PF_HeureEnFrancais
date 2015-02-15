all: test_minutes.exe test_rounded.exe

test_minutes.exe: test_minutes.c ../french_number.h

%.exe: %.c
	gcc -o $@ $<

clean:
	rm *.exe
