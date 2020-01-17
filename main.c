#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum action_type { POISON=2, ATTACK=1, DEFENSE=0 } action_type_t;

typedef struct entity
{
  char* name;
  char* attack;
  int hp;
  int dmg;
  int pm;
  action_type_t action;
  int status;
} entity_t;


void round_start(entity_t *player, entity_t *mob) {
  int round_step=1;
  if (player->pm<5){ player->pm++; }
  if (mob->pm<5) { mob->pm++; }
  if (mob->pm>=5) { mob->action=rand()%3; }
  else { mob->action=rand()%2; }
  while(round_step==1){
    printf("%s life points: %d\n", mob->name, mob->hp);
    printf("Your life points : %d\n",player->hp);
    printf("Your pm : %d\n",player->pm);
    printf("Attack{1}, defend{0} or poison spell{2}(cost 5pm) ?\n");
    scanf("%u",&(player->action));
    if (player->action==POISON & player->pm<5){ printf("Not enough mana points to cast \"poison\"\n");  }
    else { round_step=2; }
  }
}

void attack(entity_t *assaillant, entity_t *target) {
  if (assaillant->action==ATTACK) {
    if (target->action==DEFENSE) {
      printf("%s defends!\n",target->name);
      printf("The attack %s inflict %d life points to %s.\n",assaillant->attack,assaillant->dmg/4,target->name);
      target->hp-=assaillant->dmg/4;
    }
    else{
      printf("The attack %s inflict %d life points to %s.\n",assaillant->attack,assaillant->dmg,target->name);
      target->hp-=assaillant->dmg;
    }
  }
}

void setup_player(entity_t *player) {
  char name[1024];
  printf("Entrez votre nom\n");
  scanf("%[^\n]s",name);
  player->name=strdup(name);
}

int main() {
  entity_t player={"Player","sword slash",30, 8, 5, 0, 0};
  entity_t mob={"Basilic","bite",30, 12, 5, 0, 0};
  setup_player(&player);
  int i; int j; int k;

  while (mob.hp>0 && player.hp>0) {
    round_start(&player,&mob);
    attack(&player,&mob);
    attack(&mob,&player);
  }
  return 0;
}
