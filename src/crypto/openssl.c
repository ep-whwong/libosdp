/*
 * Copyright (c) 2021-2024 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>

//TODO: Note, to use mbedtls instead
#if 0
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#endif

void osdp_crypt_setup()
{
}

void osdp_openssl_fatal(void)
{
#if 0
	ERR_print_errors_fp(stderr);
	abort();
#endif
}

void osdp_encrypt(uint8_t *key, uint8_t *iv, uint8_t *data, int data_len)
{
#if 0	
	int len;
	EVP_CIPHER_CTX *ctx;
	const EVP_CIPHER *type;

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL) {
		osdp_openssl_fatal();
	}

	if (iv != NULL) {
		type = EVP_aes_128_cbc();
	} else {
		type = EVP_aes_128_ecb();
	}

	if (!EVP_EncryptInit_ex(ctx, type, NULL, key, iv)) {
		osdp_openssl_fatal();
	}

	if (!EVP_CIPHER_CTX_set_padding(ctx, 0)) {
		osdp_openssl_fatal();
	}

	if (!EVP_EncryptUpdate(ctx, data, &len, data, data_len)) {
		osdp_openssl_fatal();
	}

	if (!EVP_EncryptFinal_ex(ctx, data + len, &len)) {
		osdp_openssl_fatal();
	}

	EVP_CIPHER_CTX_free(ctx);
#endif	
}

void osdp_decrypt(uint8_t *key, uint8_t *iv, uint8_t *data, int data_len)
{
#if 0	
	int len;
	EVP_CIPHER_CTX *ctx;
	const EVP_CIPHER *type;

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL) {
		osdp_openssl_fatal();
	}

	if (iv != NULL) {
		type = EVP_aes_128_cbc();
	} else {
		type = EVP_aes_128_ecb();
	}

	if (!EVP_DecryptInit_ex(ctx, type, NULL, key, iv)) {
		osdp_openssl_fatal();
	}

	if (!EVP_CIPHER_CTX_set_padding(ctx, 0)) {
		osdp_openssl_fatal();
	}

	if (!EVP_DecryptUpdate(ctx, data, &len, data, data_len)) {
		osdp_openssl_fatal();
	}

	if (!EVP_DecryptFinal_ex(ctx, data + len, &len)) {
		osdp_openssl_fatal();
	}

	EVP_CIPHER_CTX_free(ctx);
#endif	
}

void osdp_fill_random(uint8_t *buf, int len)
{
#if 0	
	if (RAND_bytes(buf, len) != 1) {
		osdp_openssl_fatal();
	}
#endif	
}

void osdp_crypt_teardown()
{
}