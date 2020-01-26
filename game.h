#ifndef __GAME_H_ //permet d'éviter un appel multiple du dossier game.h
#define __GAME_H_

typedef enum action_type { ANTIDOTE=4, CAST=3, DEFENSE=2, ATTACK=1} action_type_e;
typedef enum status_type { DEAD=0, HEALTHY=1, POISONNED=2 } status_type_e;
typedef struct entity entity_s;
typedef struct team team_s;
typedef struct spell spell_s;

void setup_player(entity_s *player);
int round_start_ia(entity_s *mob, team_s *opponents);
void choice_target (entity_s *character,team_s *targetted_team);
int round_start_character(entity_s *character, team_s *allies, team_s *ennemies);
int attack(entity_s *assaillant, entity_s *target);
void status_resume(entity_s *entity);
void add_to_team(team_s *team, entity_s *member);
int victory_check(team_s *allies, team_s *opponents);
void spell(entity_s *caster, entity_s *target);

struct spell
{
    int def;
    int dmg;
    int hp;
    int cost_pm;
    int status_modif;
    char *name;
};

struct entity
{
    char* name;
    char* attack;
    int hp_max;
    int hp;
    int pm_max;
    int pm;
    int dmg;
    entity_s *target;
    action_type_e action;
    status_type_e status;
    spell_s *spell;
    int def_init;
    int def;
};

struct team
{
    char* name;
    int size;
    entity_s **members;
};

#endif /* end of include guard: __GAME_H_ */
