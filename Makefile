CC=cc65
CA=ca65
LD=ld65

CFG=--target none

default: bin bin/main.o bin/start.o
	$(LD) -C cc65.cfg bin/*.o /usr/share/cc65/lib/none.lib -o bin/prog.bin

bin/main.o: src/main.c
	$(CC) $(CFG) src/main.c -o bin/main.s
	$(CA) bin/main.s -o bin/main.o

bin/start.o: src/start.s
	$(CA) src/start.s -o bin/start.o

bin:
	mkdir -p bin

upload: default
	./minipro/minipro -p AT28C256 -u -w bin/prog.bin -s

clean:
	rm -rf bin/*