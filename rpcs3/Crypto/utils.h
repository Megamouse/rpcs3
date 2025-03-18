#pragma once

// Copyright (C) 2014       Hykem <hykem@hotmail.com>
// Licensed under the terms of the GNU GPL, version 2.0 or later versions.
// http://www.gnu.org/licenses/gpl-2.0.txt

#include "util/types.hpp"

#include <stdlib.h>

enum { CRYPTO_MAX_PATH = 4096 };

char* extract_file_name(const char* file_path, char real_file_name[CRYPTO_MAX_PATH]);

std::string sha256_get_hash(const char* data, usz size, bool lower_case);

// Hex string conversion auxiliary functions.
u64 hex_to_u64(const char* hex_str);
void hex_to_bytes(unsigned char* data, const char* hex_str, unsigned int str_length);

// Crypto functions (AES128-CBC, AES128-ECB, SHA1-HMAC and AES-CMAC).
void aescbc128_decrypt(const unsigned char* key, unsigned char* iv, const unsigned char* in, unsigned char *out, usz len);
void aescbc128_encrypt(const unsigned char* key, unsigned char* iv, const unsigned char* in, unsigned char *out, usz len);
void aesecb128_encrypt(const unsigned char* key, const unsigned char* in, unsigned char* out);
bool hmac_hash_compare(const unsigned char* key, int key_len, const unsigned char* in, usz in_len, const unsigned char* hash, usz hash_len);
void hmac_hash_forge(const unsigned char* key, int key_len, const unsigned char* in, usz in_len, unsigned char* hash);
bool cmac_hash_compare(const unsigned char* key, int key_len, const unsigned char* in, usz in_len, const unsigned char* hash, usz hash_len);
void cmac_hash_forge(const unsigned char* key, int key_len, const unsigned char* in, usz in_len, unsigned char* hash);
void mbedtls_zeroize(void *v, size_t n);

// SC passphrase crypto

int vtrm_decrypt(int type, u8* iv, u8* input, u8* output);
int vtrm_decrypt_master(s64 laid, s64 paid, u8* iv, const u8* input, u8* output);
int vtrm_decrypt_with_portability(int type, u8* iv, const u8* input, u8* output);
