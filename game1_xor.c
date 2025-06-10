#include <stdio.h>
#include <string.h>

#define XOR_KEY 'S'

void encryptDecrypt(char inpString[]) {
  int len = strlen(inpString);

  for (int i = 0; i < len; i++) {
    inpString[i] = inpString[i] ^ XOR_KEY;
  }
}

int main() {

  printf("\nWelcome to the 1st out of 5 games!\n"
         "Here is your encrypted message (in Hex):\n\n");

  char originalString[] = "Holepunch is awesome";
  encryptDecrypt(originalString);

  for (int i = 0; i < strlen(originalString); i++) {
    printf("0x%02X ", (unsigned char)originalString[i]);
  }
  
  printf("\n\nKey: %c \n\n"
         "Now you have to decrypt the message by using XOR. "
         "Your answer must be the decrypted string:\n", XOR_KEY);

  char guess[100];

  fgets(guess, sizeof(guess), stdin);
  guess[strcspn(guess, "\n")] = 0;

  encryptDecrypt(originalString);

  if (strcmp(guess, originalString) == 0) {
    printf("YOU GUESSED IT!🥳 You can now move on to the next challenge!\n");
  } else {
    printf("❌ Wrong guess - try again❌ \n");
  }
  return 0;
}

