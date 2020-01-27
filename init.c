#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"

void clrscreen()
{
    system("@cls||clear");
}

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

void add_to_team(team_s *team, entity_s *member)
{
    team->members = realloc(team->members, (team->size + 1) * sizeof(entity_s *));
    team->members[team->size] = member;
    team->size++;
}
