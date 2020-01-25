#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"

//Fonction pour actualiser l'affichage dans la console
void clrscreen()
{
    system("@cls||clear");
}

//void dessin() {
  //printf("                           \\    /                         \n                          ( o  o )                        \n                            v  v                  / \\     \n                           (     )                | |     \n                            (     )               | |     \n                            (      )              | |     \n                      ()    (      )              | |     \n                       ()  (       )              | |     \n       /\\              ( )(        )              | |     \n       \\ \\             ( (          )             | |     \n       /\\ \\   |\\      ( (            )            | |     \n       \\ \\ \\__| |      (              )      ( )---O---( )\n       |\\       |                              |      _|  \n        \\       |                               \\     |   \n          \\      \\                              /   __|   \n            \\                                             \n");
//}

// Dmande du nom
void setup_player(entity_s *player)
{
  clrscreen();
  char name[1024];
  printf("You are a member of the \"Fellowship of the light\"\n");
  printf("Please indicate your name :\n");
  scanf("%[^\n]s",name); //permet d'enregister tout jusqu'au premier \n, dont les espaces. Peut générer un crash si le joueur entre un nom plus gros que 1024 octets.
  player->name=strdup(name);
  clrscreen();
  //dessin();
}

//Initialisation du round
void round_start_ia(entity_s *mob, team_s *opponents)
{
  if (mob->pm<mob->pm_max) { mob->pm++; } //On peut ajouter cette ligne si on veut que le monstre régénère des pm
  if (mob->pm>=5) { mob->action=rand()%3; }
  else { mob->action=rand()%2; }
  mob->target=opponents->members[rand()%4];
  printf("%s life points: %d\n", mob->name, mob->hp);
}

void choice_target (entity_s *character,team_s *targetted_team)
{
  int i;
  printf("Which entity ? :{1}[%s] {2}[%s] {3}[%s] {4}[%s]\n",targetted_team->members[0]->name,targetted_team->members[1]->name,targetted_team->members[2]->name,targetted_team->members[3]->name);
  scanf("%d",&i);
  character->target=targetted_team->members[i-1];
}

void round_start_character(entity_s *character, team_s *allies, team_s *ennemies)
{
  int targetted_team=0;
  int round_step=1;
  if (character->pm<character->pm_max){ character->pm++; }
  printf("%s life points: %d\n", character->name, character->hp);
  printf("%s pm : %d\n",character->name, character->pm);
  while(round_step==1){
    printf("{1}Attack,{0}defend, {2}poison-spell(cost 5pm) or {3}Antidote(cost 3pm) ?\n");
    scanf("%u",&(character->action));
    if (character->action==POISON & character->pm<5){ printf("Not enough mana points to cast \"poison\"\n");  }
    else if (character->action==ANTIDOTE & character->pm<3){ printf("Not enough mana points to cast \"antidote\"\n");  }
    else { round_step++;}
  }
  if (character->action!=DEFENSE){
    while(round_step==2) {
      printf("Choose Target :\n");
      printf("Which team ? : {0}[%s] {1}[%s]\n",allies->name, ennemies->name);
      scanf("%d",&targetted_team);
      if (targetted_team==0) {
        choice_target(character, allies);
        round_step++;
      }
      else if (targetted_team==1) {
        choice_target(character, ennemies);
        round_step++;
      }
      else{
        printf(">!>!>!Wrong input!<!<!<\n");
      }
    }
  }
  printf("\n");
}

//Resolution de l'attaque d'une entitée sur une autre
void attack(entity_s *assaillant, entity_s *target)
{
  printf("-------------- %s --------------\n", assaillant->name);
  if (assaillant->action==ATTACK) {
    if (target->action==DEFENSE) {
        printf("%s DEFENDS!\n",target->name);
      }
    printf("%s use %s and inflict %d life points to %s.\n",assaillant->name,assaillant->attack,assaillant->dmg/target->def,target->name);
    target->hp-=assaillant->dmg/target->def;
  }
  else if (assaillant->action==DEFENSE) {
    assaillant->def+=4;
    printf("%s DEFENDS!\n",assaillant->name);
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
void status_resume(entity_s *entity)
{
  if (entity->status==POISONNED) {
    entity->hp-=1;
    printf("%s is poisonned and lost 1 hp\n",entity->name);
  }
  if (entity->status==DEAD) {
    printf("%s dies.\n",entity->name);
  }
  entity->def=entity->def_init;
}

void add_to_team(team_s *team, entity_s *member)
{
  team->members = realloc(team->members, (team->size + 1) * sizeof(entity_s *));
  team->members[team->size] = member;
  team->size++;
}



// MAIN
int main() {
  srand(time(NULL));
  spell_s soin={0, 0, 5, 3};
  spell_s strike={0, 2, 0, 3};
  spell_s shield={2, 0, 0, 3};
  entity_s player= {"Player","SWORD SLASH",30, 5, 30, 5, 12, 0, 0, 1, NULL, 1, 1};
  entity_s healer= {"Healer","HEALING",20, 5, 20, 5, 0, 0, 0, 1, &soin, 1, 1};
  entity_s warrior={"Warrior","STAGGERING STRIKE",20, 5, 20, 5, 5, 0, 0, 1, &strike, 1, 1};
  entity_s templar={"Templar","SHIELD WALL",25, 5, 25, 5, 3, 0, 0, 1, &shield, 1, 1};
  entity_s chief=  {"Chief Superior Orc Smallthumbdunnowhich","DEADLY PUNCH", 20, 5, 20, 5, 5, 0, 0, 1, NULL, 1, 1};
  entity_s orc=    {"Bored Orc","BEARLY STADING AXE STROKE", 20, 5, 20, 5, 5, 0, 0, 1, NULL, 1, 1};
  entity_s gob=    {"Toothless Gobelin","SNEAKY HIT", 20, 5, 20, 5, 5, 0, 0, 1, NULL, 1, 1};
  entity_s squig=  {"Slightly angry Squig","BITE", 20, 5, 20, 5, 5, 0, 0, 1, NULL, 1, 1};
  team_s allies={"fellowship of light", 0, NULL};
  team_s opponents={"Smallthumbdunnowich crew", 0, NULL};
  add_to_team(&allies,&player); add_to_team(&allies,&healer); add_to_team(&allies,&warrior); add_to_team(&allies,&templar);
  add_to_team(&opponents,&chief); add_to_team(&opponents,&orc); add_to_team(&opponents,&gob); add_to_team(&opponents,&squig);
  setup_player(&player);
  int i; int j; int k;

  while (chief.hp>0 && player.hp>0) {
    printf("\n<<<<<<<<<<<<<< NEW ROUND >>>>>>>>>>>>>>>\n\n");
    round_start_ia(&chief,&allies);
    round_start_ia(&orc,&allies);
    round_start_ia(&gob,&allies);
    round_start_ia(&squig,&allies);
      printf("\n");
    round_start_character(&player,&allies,&opponents); //screen cleared after that line
    round_start_character(&healer,&allies,&opponents);
    round_start_character(&warrior,&allies,&opponents);
    round_start_character(&templar,&allies,&opponents);
    attack(&player,player.target);
    attack(&healer,healer.target);
    attack(&warrior,warrior.target);
    attack(&templar,templar.target);
    attack(&chief,chief.target);
    attack(&orc,orc.target);
    attack(&gob,gob.target);
    attack(&squig,squig.target);
    printf("----------Status resolution----------\n");
    status_resume(&player);
    status_resume(&healer);
    status_resume(&warrior);
    status_resume(&templar);
    status_resume(&chief);
    status_resume(&orc);
    status_resume(&gob);
    status_resume(&squig);
  }
  return 0;
}
