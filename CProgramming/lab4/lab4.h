
#define QUATER_FINAL_ROUNDS 8
#define SEMI_FINAL 2
struct Team
{
    char team_name[25];
    int handicap;
};

struct Team *league[8],T[8],result;

struct Team game(struct Team *team1, struct Team *team2);

struct Team tournament(struct Team *team[]);

struct Team game_handicap(struct Team *team1,struct Team *team2);

struct Team tournament_handicap(struct Team *team[]);
