#include <stdio.h>
#include <stdlib.h>
#include "other.h"

void endGame(void){
  printf("You win!\n");
  exit(0);
}

int main(void) {
  int guess, secret, len;
  char buf[12];
  printf("Enter secret number:\n");
  scanf("%12s", buf); //fixed line
  guess = atoi(buf);
  secret=getSecretCode();
  if (guess == secret) {
    printf("You got it right!\n");
  } else {
    printf("You are so wrong!\n");
    return 1;
  }
  printf("Enter the secret string to win:\n");
  scanf("%12s", buf); //fixed line
  guess = calculateValue(buf, strlen(buf));
  if (guess != secret) {
    printf("You lose!\n");
    return 2;
  }
  endGame();
  return 0;
}
