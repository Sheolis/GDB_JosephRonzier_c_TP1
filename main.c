#include <stdio.h>
#include <stdlib.h>

int main() {
  int hp_player=30;
  int hp_moob=30;
  int dmg_sword=8;
  int dmg_bite=12;
  int action_player;
  int action_moob;
  int i; int j; int k;


  while (hp_moob>0 && hp_player>0) {
    action_moob=rand()%2;
    printf("Basilic life points: %d\n",hp_moob);
    printf("Your life points : %d\n",hp_player);
    printf("Attack{1} or defend{0} ?\n");
    scanf("%d",&action_player);
    if (action_player==1){
      if (action_moob==0) {
        printf("The basilic defends itself!\n");
        printf("Sword slash inflict %d life points to the basilic.\n",dmg_sword/4);
        hp_moob-=dmg_sword/4;
      }
      else{
        printf("Sword slash inflict %d life points to the basilic.\n",dmg_sword);
        hp_moob-=dmg_sword;
      }

    }
    if(action_moob==1){
      if (action_player==0) {
        printf("The basilic bites you, inflicting %d life points\n",dmg_bite/4);
        hp_player-=dmg_bite/4;
      }
      else {
        printf("The basilic bites you, inflicting %d life points\n",dmg_bite);
        hp_player-=dmg_bite;
      }
    }
  }
  return 0;
}
