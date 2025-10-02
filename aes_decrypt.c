#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

/**
 * Convert hex string to binary data
 * @param hex_str: hex string input
 * @param bin_data: output binary data
 * @param hex_len: length of hex string
 * @return: length of binary data, -1 on error
 */
int hex_to_bin(const char *hex_str, unsigned char *bin_data, int hex_len) {
    if (hex_len % 2 != 0) {
        fprintf(stderr, "Error: Hex string length must be even\n");
        return -1;
    }
    
    int bin_len = hex_len / 2;
    for (int i = 0; i < bin_len; i++) {
        char hex_byte[3] = {hex_str[i*2], hex_str[i*2+1], '\0'};
        bin_data[i] = (unsigned char)strtol(hex_byte, NULL, 16);
    }
    
    return bin_len;
}

/**
 * Print binary data as hex string
 * @param data: binary data
 * @param len: length of data
 */
void print_hex(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

/**
 * Decrypt data using AES128 ECB mode
 * @param encrypted_data: encrypted data
 * @param data_len: length of encrypted data
 * @param key: 16-byte AES key
 * @param decrypted_data: output buffer for decrypted data
 * @return: length of decrypted data, -1 on error
 */
int aes128_decrypt(const unsigned char *encrypted_data, int data_len, 
                   const unsigned char *key, unsigned char *decrypted_data) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int decrypted_len;
    
    // Create and initialize the context
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        fprintf(stderr, "Error: Failed to create cipher context\n");
        return -1;
    }
    
    // Initialize the decryption operation with AES128 ECB
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) {
        fprintf(stderr, "Error: Failed to initialize decryption\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    
    // Disable padding for ECB mode
    EVP_CIPHER_CTX_set_padding(ctx, 0);
    
    // Provide the message to be decrypted, and obtain the plaintext output
    if(1 != EVP_DecryptUpdate(ctx, decrypted_data, &len, encrypted_data, data_len)) {
        fprintf(stderr, "Error: Failed to decrypt data\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    decrypted_len = len;
    
    // Finalize the decryption
    if(1 != EVP_DecryptFinal_ex(ctx, decrypted_data + len, &len)) {
        fprintf(stderr, "Error: Failed to finalize decryption\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    decrypted_len += len;
    
    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    
    return decrypted_len;
}

/**
 * Print usage information
 */
void print_usage(const char *program_name) {
    printf("Usage: %s <hex_encrypted_data> <hex_key>\n", program_name);
    printf("  hex_encrypted_data: Encrypted data in hexadecimal format\n");
    printf("  hex_key: 32-character hexadecimal key (16 bytes for AES128)\n");
    printf("\nExample:\n");
    printf("  %s \"6bc1bee22e409f96e93d7e117393172a\" \"2b7e151628aed2a6abf7158809cf4f3c\"\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char *hex_data = argv[1];
    const char *hex_key = argv[2];
    
    // Validate key length (must be 32 hex characters = 16 bytes)
    if (strlen(hex_key) != 32) {
        fprintf(stderr, "Error: Key must be exactly 32 hexadecimal characters (16 bytes)\n");
        return 1;
    }
    
    // Validate data length (must be multiple of 32 hex characters = 16 bytes for AES block size)
    int data_hex_len = strlen(hex_data);
    if (data_hex_len % 32 != 0) {
        fprintf(stderr, "Error: Encrypted data length must be multiple of 32 hex characters (16 bytes)\n");
        return 1;
    }
    
    // Convert hex key to binary
    unsigned char key[16];
    if (hex_to_bin(hex_key, key, 32) != 16) {
        fprintf(stderr, "Error: Failed to convert key from hex\n");
        return 1;
    }
    
    // Convert hex data to binary
    int data_bin_len = data_hex_len / 2;
    unsigned char *encrypted_data = malloc(data_bin_len);
    if (!encrypted_data) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }
    
    if (hex_to_bin(hex_data, encrypted_data, data_hex_len) != data_bin_len) {
        fprintf(stderr, "Error: Failed to convert encrypted data from hex\n");
        free(encrypted_data);
        return 1;
    }
    
    // Allocate buffer for decrypted data
    unsigned char *decrypted_data = malloc(data_bin_len);
    if (!decrypted_data) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(encrypted_data);
        return 1;
    }
    
    // Perform decryption
    int decrypted_len = aes128_decrypt(encrypted_data, data_bin_len, key, decrypted_data);
    if (decrypted_len < 0) {
        fprintf(stderr, "Error: Decryption failed\n");
        free(encrypted_data);
        free(decrypted_data);
        return 1;
    }
    
    // Print results
    printf("Encrypted data (hex): %s\n", hex_data);
    printf("Key (hex): %s\n", hex_key);
    printf("Decrypted data (hex): ");
    print_hex(decrypted_data, decrypted_len);
    
    printf("Decrypted data (text): ");
    for (int i = 0; i < decrypted_len; i++) {
        if (decrypted_data[i] >= 32 && decrypted_data[i] <= 126) {
            printf("%c", decrypted_data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
    
    // Clean up
    free(encrypted_data);
    free(decrypted_data);
    
    return 0;
}