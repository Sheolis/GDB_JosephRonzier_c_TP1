#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum action_type { ANTIDOTE=3, POISON=2, ATTACK=1, DEFENSE=0 } action_type_t;
typedef enum status_type { DEAD=0, HEALTHY=1, POISONNED=2 } status_type_t;
typedef struct entity {
  char* name;
  char* attack;
  int hp_max;
  int pm_max;
  int hp;
  int pm;
  int dmg;
  action_type_t action;
  status_type_t status;
  spell_s spell;
} entity_t;


typedef struct spell {
  int def;
  int dmg;
  int hp;
  int cost_pm;
} spell_s;

//Fonction pour actualiser l'affichage dans la console
void clrscreen()
{
    system("@cls||clear");
}

void dessin() {
  printf("                           \\    /                         \n                          ( o  o )                        \n                            v  v                  / \\     \n                           (     )                | |     \n                            (     )               | |     \n                            (      )              | |     \n                      ()    (      )              | |     \n                       ()  (       )              | |     \n       /\\              ( )(        )              | |     \n       \\ \\             ( (          )             | |     \n       /\\ \\   |\\      ( (            )            | |     \n       \\ \\ \\__| |      (              )      ( )---O---( )\n       |\\       |                              |      _|  \n        \\       |                               \\     |   \n          \\      \\                              /   __|   \n            \\                                             \n");
}
// Dmande du nom
void setup_player(entity_t *player) {
  clrscreen();
  char name[1024];
  printf("Entrez votre nom\n");
  scanf("%[^\n]s",name); //permet d'enregister tout jusqu'au premier \n, dont les espaces. Peut générer un crash si le joueur entre un nom plus gros que 1024 octets.
  player->name=strdup(name);
  clrscreen();
  dessin();
}

//Initialisation du round
void round_start(entity_t *player, entity_t *mob) {
  printf("\n<<<<<<<<<<<<<< NEW ROUND >>>>>>>>>>>>>>>\n\n");
  int round_step=1;
  if (player->pm<player->pm_max){ player->pm++; }
  //if (mob->pm<5) { mob->pm++; } //On peut ajouter cette ligne si on veut que le monstre régénère des pm
  if (mob->pm>=5) { mob->action=rand()%3; }
  else { mob->action=rand()%2; }
  printf("%s life points: %d\n", mob->name, mob->hp);
  printf("Your life points : %d\n",player->hp);
  printf("Your pm : %d\n",player->pm);
  while(round_step==1){
    printf("{1}Attack,{0}defend, {2}poison-spell(cost 5pm) or {3}Antidote(cost 3pm) ?\n");
    scanf("%u",&(player->action));
    if (player->action==POISON & player->pm<5){ printf("Not enough mana points to cast \"poison\"\n");  }
    else if (player->action==ANTIDOTE & player->pm<3){ printf("Not enough mana points to cast \"antidote\"\n");  }
    else { round_step=2; clrscreen();}
  }
  printf("\n");
}

//Resolution de l'attaque d'une entitée sur une autre
void attack(entity_t *assaillant, entity_t *target) {
  printf("-------------- %s --------------\n", assaillant->name);
  if (assaillant->action==ATTACK) {
    if (target->action==DEFENSE) {
      printf("%s DEFENDS!\n",target->name);
      printf("%s use %s and inflict %d life points to %s.\n",assaillant->name,assaillant->attack,assaillant->dmg/4,target->name);
      target->hp-=assaillant->dmg/4;
    }
    else{
      printf("The attack %s inflict %d life points to %s.\n",assaillant->attack,assaillant->dmg,target->name);
      target->hp-=assaillant->dmg;
    }
  }
  else if(assaillant->action==POISON) {
    printf("%s POISONS %s.\n",assaillant->name,target->name);
    target->status=2;
    assaillant->pm-=5;
  }
  else if(assaillant->action==ANTIDOTE) {
    printf("%s use ANTIDOTE\n",assaillant->name);
    printf("%s is no longer poisonned\n",assaillant->name);
    assaillant->pm-=3;
    assaillant->status=1;
  }
  printf("\n");
  if(target->hp<=0) {
    target->status=0;
  }
}

//Resolutions spéciales en fonction du statut de l'entitée
void status_resume(entity_t *entity) {
  if (entity->status==POISONNED) {
    entity->hp-=1;
    printf("%s is poisonned and lost 1 hp\n",entity->name);
  }
  if (entity->status==DEAD) {
    printf("%s dies.\n",entity->name);
  }
}



// MAIN
int main() {
  srand(time(NULL));
  spell_s soin={0,0,5,3}; 
  entity_t player={"Player","SWORD SLASH",30, 5, 30, 5, 12, 0, 1};
  entity_t healer={"Healer","HEALING",20, 5, 20, 5, 0, 0, 1, soin};
  entity_t mob={"Orc","BITE", 20, 5, 20, 5, 5, 0, 1};
  setup_player(&player);
  int i; int j; int k;

  while (mob.hp>0 && player.hp>0) {
    round_start(&player,&mob); //screen cleared after that line
    dessin();
    attack(&player,&mob);
    attack(&mob,&player);
    status_resume(&mob);
    status_resume(&player);
  }
  return 0;
}
