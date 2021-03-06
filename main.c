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
int round_start_ia(entity_s *mob, team_s *opponents)
{
    if (mob->status==DEAD)
    {
        printf("%s life points: dead\n",mob->name);
        return -1;
    }
    if (mob->pm<mob->pm_max) { mob->pm++; } //On peut ajouter cette ligne si on veut que le monstre régénère des pm
    if (mob->spell!=NULL)
    {
        if (mob->pm>=mob->spell->cost_pm) { mob->action=rand()%3; }
    }
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


int round_start_character(entity_s *character, team_s *allies, team_s *ennemies)
{
    if (character->status==DEAD)
    {
        printf("%s is dead and can't play anymore\n",character->name);
        return -1;
    }

    int targetted_team=0;
    int round_step=1;

    if (character->pm<character->pm_max) { character->pm++; }
    printf("%s life points: %d\n", character->name, character->hp);
    printf("%s pm : %d\n",character->name, character->pm);
    while(round_step==1)
    {
        printf("[1]Attack [2]Defend [3]%s spell (cost %d) [4]Antidote(cost 10pm) ?\n",character->spell->name,character->spell->cost_pm);
        scanf("%u",&(character->action));
        if (character->action==ANTIDOTE & character->pm<3){ printf("Not enough mana points to cast \"antidote\"\n");  }
        else { round_step++;}
    }
    if (character->action!=DEFENSE)
    {
        while(round_step==2) {
            printf("Choose Target :\n");
            printf("Which team ? : {0}[%s] {1}[%s]\n",allies->name, ennemies->name);
            scanf("%d",&targetted_team);
            if (targetted_team==0)
            {
                choice_target(character, allies);
                if (character->target->status==DEAD)
                {
                    printf("This target is already dead, you need to choose another one\n");
                }
                else
                {
                  round_step++;
                }
            }
            else if (targetted_team==1)
            {
                choice_target(character, ennemies);
                if (character->target->status==DEAD)
                {
                    printf("This target is already dead, you need to choose another one\n");
                }
                else
                {
                    round_step++;
                }
            }
            else
            {
                printf(">!>!>!Wrong input!<!<!<\n");
            }
        }
    }
    else {
      character->target=character; // Permet d'éviter d'appeler plus tard la fonction attaque avec un target=NULL
    }
    printf("\n");
}

//Resolution de l'attaque d'une entitée sur une autre
int attack(entity_s *assaillant, entity_s *target)
{
    if (assaillant->status==DEAD)
    {
      printf("%s lie dead\n",assaillant->name);
      return -1;
    }

    if (target->status==DEAD)
    {
      printf("The target \"%s\" is already dead.\n",assaillant->name);
      return -1;
    }

    printf("-------------- %s --------------\n", assaillant->name);

    switch (assaillant->action) {

        case ATTACK:

            if (target->action==DEFENSE) {
                printf("%s DEFENDS!\n",target->name);
              }
            printf("%s use %s and inflict %d life points to %s.\n", assaillant->name, assaillant->attack, assaillant->dmg/target->def, target->name);
            target->hp-=assaillant->dmg/target->def;
            break;

        case DEFENSE:

            assaillant->def+=4;
            printf("%s DEFENDS!\n",assaillant->name);
            break;

        case CAST:

            printf("%s cast %s on %s\n", assaillant->name, assaillant->spell->name, target->name);
            spell(assaillant, target);
            break;

        case ANTIDOTE:

            printf("%s use ANTIDOTE\n",assaillant->name);
            printf("%s is no longer poisonned\n",assaillant->name);
            assaillant->pm-=10;
            assaillant->status=1;
            break;

    }

    printf("\n");
    if(target->hp<=0)
    {
        target->status=0;
    }
}

//Resolution du spell des alliés vers une entitée
void spell(entity_s *caster, entity_s *target)
{
    if (caster->spell->def!=0)
    {
        target->def_init+=caster->spell->def;
        printf("%s gain %d point of defense\n",target->name,caster->spell->def);
    }
    if (caster->spell->dmg!=0)
    {
        target->hp+=caster->spell->dmg;
        printf("%s lost %d life points\n",target->name,caster->spell->dmg);
    }
    if (caster->spell->hp!=0)
    {
        target->hp+=caster->spell->dmg;
        printf("%s is healed by %d life points\n",target->name,caster->spell->hp);
    }
    if (caster->spell->status_modif!=(-1))
    {
        target->status=caster->spell->status_modif;
    }
    caster->pm-=caster->spell->cost_pm;
}

//Resolutions spéciales en fonction du statut de l'entitée
void status_resume(entity_s *entity)
{
    switch (entity->status) {

        case POISONNED:
            entity->hp-=5;
            printf("%s is poisonned and lost 5 hp\n",entity->name);
            break;

        case DEAD:
            printf("%s dies.\n",entity->name);
            break;
    }
    entity->def=entity->def_init;
}

void add_to_team(team_s *team, entity_s *member)
{
    team->members = realloc(team->members, (team->size + 1) * sizeof(entity_s *));
    team->members[team->size] = member;
    team->size++;
}

int victory_check(team_s *allies, team_s *opponents)
{
    int v_check=0;
    for (int i = 0; i < 4; i++)
    {
        if (allies->members[i]->status==DEAD)
        {
          v_check++;
        }
    }
    if (v_check==4)
    {
          printf("You have been defeated by the %s\n",opponents->name);
          return 1;
    }
    v_check=0;
    for (int i = 0; i < 4; i++)
    {
        if (opponents->members[i]->status==DEAD)
        {
            v_check++;
        }
    }
    if (v_check==4)
    {
          printf("Victory, the %s is defeated !\n",opponents->name);
          return 1;
    }
    return 0;
}


// MAIN
int main() {
    srand(time(NULL));
    spell_s poison={0, 0, 0, 10, 2, "Poison"};
    spell_s soin={0, 0, 25, 25, -1, "Soin"};
    spell_s strike={0, -50, 0, 25, -1, "Strike"};
    spell_s shield={1, 0, 0, 15, -1, "Shield"};
    entity_s player= {"Player","SWORD SLASH",100, 100, 25, 25, 20, 0, 0, 1, &poison, 1, 1};
    entity_s healer= {"Healer","HEALING",50, 50, 100, 100, 10, 0, 0, 1, &soin, 1, 1};
    entity_s warrior={"Warrior","STAGGERING STRIKE",100, 100, 25, 25, 20, 0, 0, 1, &strike, 1, 1};
    entity_s templar={"Templar","SHIELD WALL",125, 125, 30, 30, 18, 0, 0, 1, &shield, 1, 1};
    entity_s chief=  {"Chief Superior Orc Smallthumbdunnowhich","DEADLY PUNCH", 200, 200, 10, 10, 12, 0, 0, 1, &poison, 1, 1};
    entity_s orc=    {"Bored Orc","BEARLY STADING AXE STROKE", 110, 110, 0, 0, 9, 0, 0, 1, NULL, 1, 1};
    entity_s gob=    {"Toothless Gobelin","SNEAKY HIT", 50, 50, 0, 0, 4, 0, 0, 1, NULL, 1, 1};
    entity_s squig=  {"Slightly angry Squig","BITE", 100, 100, 0, 0, 10, 0, 0, 1, NULL, 1, 1};
    team_s allies={"fellowship of light", 0, NULL};
    team_s opponents={"Smallthumbdunnowich crew", 0, NULL};
    add_to_team(&allies,&player); add_to_team(&allies,&healer); add_to_team(&allies,&warrior); add_to_team(&allies,&templar);
    add_to_team(&opponents,&chief); add_to_team(&opponents,&orc); add_to_team(&opponents,&gob); add_to_team(&opponents,&squig);
    setup_player(&player);

    while (victory_check(&allies, &opponents)==0)
    {
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
        for (int i = 0; i < 4; i++)
        {
            if (allies.members[i]->status!=0)
            {
                attack(allies.members[i],allies.members[i]->target);
            }
        }
        for (int i = 0; i < 4; i++)
        {
            if (opponents.members[i]->status!=0)
            {
                attack(opponents.members[i],opponents.members[i]->target);
            }
        }
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
