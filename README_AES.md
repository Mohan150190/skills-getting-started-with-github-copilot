# AES128 Decryption Tool

This tool decrypts data encrypted with AES-128 algorithm using ECB mode.

## Features

- Decrypts AES-128 encrypted data from hexadecimal format
- Accepts security key as hexadecimal input
- Built using OpenSSL library for reliable cryptographic operations
- Provides both hexadecimal and text output of decrypted data
- Comprehensive error handling and validation

## Requirements

- GCC compiler
- OpenSSL development libraries (`libssl-dev`)

## Building

To compile the tool:

```bash
make
```

To install dependencies (Ubuntu/Debian):

```bash
make install-deps
```

## Usage

```bash
./aes_decrypt <hex_encrypted_data> <hex_key>
```

### Parameters

- `hex_encrypted_data`: The encrypted data in hexadecimal format (must be multiple of 32 hex characters = 16 bytes)
- `hex_key`: The 32-character hexadecimal key (16 bytes for AES-128)

### Example

```bash
./aes_decrypt "3ad77bb40d7a3660a89ecaf32466ef97" "2b7e151628aed2a6abf7158809cf4f3c"
```

Output:
```
Encrypted data (hex): 3ad77bb40d7a3660a89ecaf32466ef97
Key (hex): 2b7e151628aed2a6abf7158809cf4f3c
Decrypted data (hex): 6bc1bee22e409f96e93d7e117393172a
Decrypted data (text): k....@...=~.s..*
```

## Testing

Run the test suite:

```bash
make test
```

Or run the comprehensive test script:

```bash
./test_aes.sh
```

## Technical Details

- **Algorithm**: AES-128
- **Mode**: ECB (Electronic Codebook)
- **Padding**: None (input must be multiple of block size)
- **Block Size**: 16 bytes (128 bits)
- **Key Size**: 16 bytes (128 bits)

## Error Handling

The tool validates:
- Key length (must be exactly 32 hex characters)
- Data length (must be multiple of 32 hex characters)
- Hex format validity
- Memory allocation success
- Decryption operation success

## Files

- `aes_decrypt.c` - Main source code
- `Makefile` - Build configuration
- `test_aes.sh` - Test script
- `README_AES.md` - This documentation

## Cleaning Up

To remove compiled files:

```bash
make clean
```