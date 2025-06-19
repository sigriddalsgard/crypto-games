#include "sodium/randombytes.h"
#include <sodium.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define NONCE_SIZE 32
#define DATA_SIZE 32

int game2_pow(void) {
  if (sodium_init() < 0)
    return 1;

  unsigned char nonce[NONCE_SIZE];

  printf("This is a simplified version of Proof of Work.\n"
       "You’ll be given a nonce, and your task is to find some data (in hex format) so that,\n"
       "when the nonce and data are combined, the resulting hash starts with: b0 0b\n\n");

  printf("Nonce in random bytes written in hex:\n");
  randombytes_buf(nonce, NONCE_SIZE);

  for (int i = 0; i < NONCE_SIZE; i++) {
    printf("0x%02X ", nonce[i]);
  }
  printf("\n\n");

  printf("Enter your data/input here:\n");

  unsigned char data[DATA_SIZE];

  for (int i = 0; i < DATA_SIZE; i++) {
    unsigned int byte;
    if (scanf("%x", &byte) != 1 || byte > 0xFF)
      exit(!printf("❌ Invalid input: make sure you enter exactly 32 hex bytes "
                   "(e.g. `4f a1 ...`).\n"));
    data[i] = byte;
  }

  unsigned char nonce_data[NONCE_SIZE + DATA_SIZE];

  memcpy(nonce_data, nonce, NONCE_SIZE);
  memcpy(nonce_data + NONCE_SIZE, data, DATA_SIZE);

  printf("\n");

  printf("Nonce and data combined:\n");
  for (int i = 0; i < NONCE_SIZE + DATA_SIZE; i++) {
    printf("0x%02X ", nonce_data[i]);
  }

  printf("\n\n");

  unsigned char hash[crypto_generichash_BYTES];

  crypto_generichash(hash, sizeof(hash), nonce_data, NONCE_SIZE + DATA_SIZE,
                     NULL, 0);

  printf("Hash of nonce + data:\n");

  for (int i = 0; i < sizeof(hash); i++) {
    printf("0x%02X ", hash[i]);
  }

  printf("\n\n");

  if (hash[0] == 0xb0 && hash[1] == 0x0b) {
    printf("✅ You found a b00b!\n");
    return 1;
  } else {
    printf("❌ No b00b for you. Very, very sorry!\n");
    return 0;
  }
}
