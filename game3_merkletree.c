#include "game3_merkletree.h"
#include <sodium.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct merkle_node {
  enum { merkle_leaf, merkle_branch } type;
  unsigned char hash[crypto_generichash_BYTES];
};

struct merkle_leaf {
  struct merkle_node node;
  char *data;
};

struct merkle_branch {
  struct merkle_node node;
  struct merkle_node *left;
  struct merkle_node *right;
};

void print_node(struct merkle_node *node, int level);

void destroy_node(struct merkle_node *node);

void print_level(int level) {
  printf("├─");

  for (int i = 0; i < level - 1; i++) {
    printf("  ");
  }

  if (level) {
    printf("├─");
  }

  printf(" ");
}

struct merkle_leaf *create_leaf(const char *data) {
  struct merkle_leaf *leaf = malloc(sizeof(struct merkle_leaf));

  leaf->node.type = merkle_leaf;
  leaf->data = strdup(data);

  crypto_generichash(leaf->node.hash, sizeof(leaf->node.hash),
                     (unsigned char *)leaf->data, strlen(leaf->data), NULL, 0);

  return leaf;
}

void destroy_leaf(struct merkle_leaf *leaf) {
  free(leaf->data);
  free(leaf);
}

void print_leaf(struct merkle_leaf *leaf, int level) {
  print_level(level);

  for (int i = 0; i < sizeof(leaf->node.hash); i++) {
    printf("%02x", leaf->node.hash[i]);
  }

  printf(" (%s)", leaf->data);
  printf("\n");
}

struct merkle_branch *create_branch(struct merkle_node *left,
                                    struct merkle_node *right) {
  struct merkle_branch *branch = malloc(sizeof(struct merkle_branch));

  branch->node.type = merkle_branch;
  branch->left = left;
  branch->right = right;

  crypto_generichash_state state;

  crypto_generichash_init(&state, NULL, 0, sizeof(branch->node.hash));

  crypto_generichash_update(&state, left->hash, sizeof(left->hash));
  crypto_generichash_update(&state, right->hash, sizeof(right->hash));

  crypto_generichash_final(&state, branch->node.hash,
                           sizeof(branch->node.hash));

  return branch;
}

void destroy_branch(struct merkle_branch *branch) {
  destroy_node(branch->left);
  destroy_node(branch->right);

  free(branch);
}

void print_branch(struct merkle_branch *branch, int level) {
  print_level(level);

  for (int i = 0; i < sizeof(branch->node.hash); i++) {
    printf("%02x", branch->node.hash[i]);
  }

  printf("\n");
  print_node(branch->left, level + 1);
  print_node(branch->right, level + 1);
}

void print_node(struct merkle_node *node, int level) {
  switch (node->type) {
  case merkle_leaf:
    print_leaf((struct merkle_leaf *)node, level);
    break;
  case merkle_branch:
    print_branch((struct merkle_branch *)node, level);
    break;
  }
}

void destroy_node(struct merkle_node *node) {
  switch (node->type) {
  case merkle_leaf:
    destroy_leaf((struct merkle_leaf *)node);
    break;
  case merkle_branch:
    destroy_branch((struct merkle_branch *)node);
    break;
  }
}

int game3_merkletree(void) {
  if (sodium_init() < 0) {
    return 1;
  }

  struct merkle_leaf *kasper = create_leaf("Kasper");
  struct merkle_leaf *sigrid = create_leaf("Sigrid");
  struct merkle_leaf *mafintosh = create_leaf("Mafintosh");
  struct merkle_leaf *anna = create_leaf("Anna");
  struct merkle_leaf *sean = create_leaf("Sean");
  struct merkle_leaf *rafa = create_leaf("Rafa");
  struct merkle_leaf *blahah = create_leaf("Blahah");
  struct merkle_leaf *andrewosh = create_leaf("Andrewosh");

  struct merkle_branch *branch_1 = create_branch(
      (struct merkle_node *)kasper, (struct merkle_node *)andrewosh);

  struct merkle_branch *branch_2 =
      create_branch((struct merkle_node *)sean, (struct merkle_node *)rafa);

  struct merkle_branch *root = create_branch((struct merkle_node *)branch_1,
                                             (struct merkle_node *)branch_1);

  printf(
      "Welcome to the Merkle Tree Challenge!\n\n"
      "You are given a Merkle root and a list of names.\n"
      "Your task is to figure out which four names are actually included\n"
      "in the tree — and in what order they appear.\n\n"
      "Once you've made your guess, you can use the names to rebuild the tree\n"
      "and verify whether the root you’ve calculated matches the one you were "
      "\n"
      "given.\n\n");

  printf("The root is here:");

  for (int i = 0; i < crypto_generichash_BYTES; i++) {
    printf("%02x", root->node.hash[i]);
  }
  printf("\n\n");

  printf("The list of possible names is here: \nSean \nRafa \nKasper"
         "\nMafintosh \nBlahah \nAnna \nSigrid \nAndrewosh\n");

  printf("\nEnter your guess below.\n");
  printf("Please write one name at a time, starting with a capital letter.\n");
  printf("Press 'Enter' after each name.\n\n");

  struct merkle_leaf *name[] = {kasper, andrewosh, sean, rafa};

  char guesses[4][100];
  for (int i = 0; i < 4; i++) {
    printf("Guess name %d: ", i + 1);
    scanf("%99s", guesses[i]);
  }

  int correct = 1;
  for (int i = 0; i < 4; i++) {
    if (strcmp(name[i]->data, guesses[i]) != 0) {
      correct = 0;
      break;
    }
  }

  if (correct) {
    printf("✅ You guessed the correct order - you uncovered the tree structure! 🥳\n");
    return 1;
  } else {
    printf("❌ Wrong tree! \n");
    return 0;
  }
}
