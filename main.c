#include <stdio.h>

int initialisation() {
  return 30;
}
int main() {
  int hp_player;
  int hp_moob;
  int dmg_sword=7;
  int dmg_bite=10;
  int i; int j; int k;

  hp_player=initialisation();
  hp_moob=initialisation();
  while (hp_moob>0 && hp_player>0) {
    printf("Basilic life points: %d\n",hp_moob);
    printf("Sword slash inflict %d life points to the monster.\n",dmg_sword);
    hp_moob-=dmg_sword;
    printf("Basilic life points: %d\n",hp_moob);
    printf("Your life points : %d\n",hp_player);
    printf("The monster bite you, inflicting %d life points\n",dmg_bite);
    hp_player-=dmg_bite;
    printf("Your life points : %d\n",hp_player);
  }

  return 0;
}
