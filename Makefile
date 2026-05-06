CC=~/llvm-mos/bin/mos-common-clang

default: bin
	$(CC) -Os -T src/link.ld -o bin/prog.bin src/main.c

bin:
	mkdir -p bin

upload: default
	./minipro/minipro -p AT28C256 -u -w bin/prog.bin

clean:
	rm -rf bin/*