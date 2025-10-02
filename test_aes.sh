#!/bin/bash

# Test script for AES128 decryption tool

echo "=== AES128 Decryption Tool Test ==="
echo

# Test 1: Using OpenSSL to encrypt and then decrypt with our tool
echo "Test 1: Encrypt 'Hello World 123!' with OpenSSL and decrypt with our tool"
echo

# Create a test key (16 bytes = 32 hex chars)
KEY="2b7e151628aed2a6abf7158809cf4f3c"
echo "Key: $KEY"

# Create test plaintext (must be 16 bytes for AES block size)
PLAINTEXT="Hello World 123!"
echo "Original text: '$PLAINTEXT'"

# Encrypt with OpenSSL (AES-128-ECB, no padding)
echo "Encrypting with OpenSSL..."
ENCRYPTED_HEX=$(echo -n "$PLAINTEXT" | openssl enc -aes-128-ecb -K "$KEY" -nopad | xxd -p | tr -d '\n')
echo "Encrypted (hex): $ENCRYPTED_HEX"

# Decrypt with our tool
echo "Decrypting with our tool..."
./aes_decrypt "$ENCRYPTED_HEX" "$KEY"

echo
echo "=== Test 2: Manual test with known values ==="
echo

# Test with NIST test vectors
# Plaintext: 6bc1bee22e409f96e93d7e117393172a
# Key: 2b7e151628aed2a6abf7158809cf4f3c
# Expected Ciphertext: 3ad77bb40d7a3660a89ecaf32466ef97

echo "Using NIST AES test vectors..."
echo "Key: 2b7e151628aed2a6abf7158809cf4f3c"
echo "Encrypted data: 3ad77bb40d7a3660a89ecaf32466ef97"
echo "Expected plaintext: 6bc1bee22e409f96e93d7e117393172a"
echo

./aes_decrypt "3ad77bb40d7a3660a89ecaf32466ef97" "2b7e151628aed2a6abf7158809cf4f3c"