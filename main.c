#include <stdio.h>

int main() {
  int hp_player=30;
  int hp_moob=30;
  int dmg_sword=8;
  int dmg_bite=12;
  int action_player;
  int action_moob;
  int i; int j; int k;


  while (hp_moob>0 && hp_player>0) {
    printf("Attack{1} or defend{0} ?\n");
    scanf("%d",&action_player);
    if (action_player==1){
      printf("Basilic life points: %d\n",hp_moob);
      printf("Sword slash inflict %d life points to the monster.\n",dmg_sword);
      hp_moob-=dmg_sword;
      printf("Basilic life points: %d\n",hp_moob);
    }
    printf("Your life points : %d\n",hp_player);

    if (action_player==0) {
      printf("The monster bite you, inflicting %d life points\n",dmg_bite/4);
      hp_player-=dmg_bite/4;
    }
    else {
      printf("The monster bite you, inflicting %d life points\n",dmg_bite);
      hp_player-=dmg_bite;
    }
    printf("Your life points : %d\n",hp_player);
  }
  return 0;
}
