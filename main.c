#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"

// MAIN
int main(void) {
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
