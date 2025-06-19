#include <stdio.h>
#include <string.h>

#include "game1_xor.h"

#define XOR_KEY 'S'

void encryptDecrypt(char inpString[]) {
  int len = strlen(inpString);

  for (int i = 0; i < len; i++) {
    inpString[i] = inpString[i] ^ XOR_KEY;
  }
}

int game1_xor(void) {

  printf("\nThis is a XOR challenge, where you have to decrypt this hex-message:\n");

  char originalString[] = "Holepunch is awesome";
  encryptDecrypt(originalString);

  for (int i = 0; i < strlen(originalString); i++) {
    printf("0x%02X ", (unsigned char)originalString[i]);
  }

  printf("\nKey: %c \n\n"
         "Enter your answer below — it should be the decrypted string:\n",
         XOR_KEY);

  char guess[100];

  fgets(guess, sizeof(guess), stdin);
  guess[strcspn(guess, "\n")] = 0;

  encryptDecrypt(originalString);

  if (strcmp(guess, originalString) == 0) {
    printf("YOU GUESSED IT!🥳 You can now move on to the next challenge!\n");
    return 1;
  } else {
    printf("❌ Wrong guess - try again❌ \n");
    return 0;
  }
}
