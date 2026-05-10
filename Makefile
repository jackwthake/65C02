CC = ~/llvm-mos/bin/mos-common-clang
AFLAGS = -mcpu=mos65c02
CFLAGS = $(AFLAGS) -Os
LDFLAGS = -T src/link.ld

# Define the build directory and target
BUILD_DIR = bin
TARGET = $(BUILD_DIR)/prog.bin

# List your source files
SRCS = src/jlibc.c src/main.c src/lcd.s
# Convert source list to object list in the build directory
OBJS = $(SRCS:src/%=$(BUILD_DIR)/%.o)

.PHONY: default clean upload

default: $(TARGET)

# Define the size tool path
SIZE = ~/llvm-mos/bin/llvm-size

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)
	@echo "--- Memory Usage ---"
	@$(SIZE) --format=berkeley $@.elf
	@echo "--------------------\n"


# Rule to compile C files
$(BUILD_DIR)/%.c.o: src/%.c | bin
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to assemble S files
$(BUILD_DIR)/%.s.o: src/%.s | bin
	$(CC) $(AFLAGS) -c $< -o $@

# Ensure the build directory exists
bin:
	mkdir -p $(BUILD_DIR)

upload: $(TARGET)
	./minipro/minipro -p AT28C256 -u -w $(TARGET)

clean:
	rm -rf $(BUILD_DIR)