#include <stdio.h>

int initialisation() {
  return 30;
}
int main() {
  int hp_player;
  int hp_moob;
  int dmg_sword=10;
  int i; int j; int k;

  hp_player=initialisation();
  hp_moob=initialisation();

  printf("life points: %d\n",hp_moob);
  printf("Sword slash inflict %d life points to the monster.\n",dmg_sword);
  hp_moob-=dmg_sword;
  printf("life points: %d\n",hp_moob);
  return 0;
}
