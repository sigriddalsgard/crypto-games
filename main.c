#include "game1_xor.h"
#include "game2_pow.h"
#include "game3_merkletree.h"
#include <stdio.h>

int main(void) {
  printf("🎮 Welcome to the Challenge Hub!\n"
         "This is the first of three challenges.\n"
         "Once you solve this one, you'll automatically advance to the next "
         "challenge, and so on.\n");

  if (!game1_xor()) {
    printf("❌ You failed game 1. Try again.\n");
    return 1;
  }

  printf("\n✅ Game 1 complete! Moving on to Game 2...\n\n");

  if (!game2_pow()) {
    printf("❌ You failed game 2. Try again.\n");
    return 1;
  }

  printf("\n✅ Game 2 complete! Time for the final challenge...\n\n");

  if (!game3_merkletree()) {
    printf("❌ Game 3 failed. Try again.\n");
    return 1;
  }

  printf("\n🏆 Congratulations! You completed all challenges!\n");
  return 0;
}
