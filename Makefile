CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lssl -lcrypto
TARGET = aes_decrypt
SOURCE = aes_decrypt.c

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Install OpenSSL development libraries (if needed)
install-deps:
	sudo apt-get update && sudo apt-get install -y libssl-dev

# Test the tool with a sample
test: $(TARGET)
	@echo "Testing AES128 decryption tool..."
	@echo "Test 1: Basic AES128 decryption"
	./$(TARGET) "3ad77bb40d7a3660a89ecaf32466ef97" "2b7e151628aed2a6abf7158809cf4f3c"

.PHONY: all clean install-deps test