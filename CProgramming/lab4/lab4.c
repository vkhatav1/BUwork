#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>
#include "lab4.h"
#include "myStrStr.h"


int main()
{
    // Part 1: Strings and Arrays
    char *haystack[]={"apple","orange","blueberry","strawberry","grapefruit"};
    char *needle[]={"app","ge","ueber","erry","terrible"};
    char buffer[15];
    memset(buffer,0, sizeof(buffer));
    int flag;
    srand(time(NULL));
    for (int i = 0; i < 5; ++i)
    {
        flag=myStrStr(haystack[i],needle[i],buffer);
        printf("\nString No : %d",i);
        
        if(flag==1)
            printf("\n Haystack : %s Needle : %s Buffer : %s\n",haystack[i],needle[i],buffer);
        else
            printf("\nString match not Found!!!\n");
        memset(buffer,0,sizeof(buffer));
    }
    
    // Part 2: Tournament
    //Part A:
    // Assigning names to the team
    league[0]=&T[0];
    char team_name1[]="Real Madrid";
    strcpy(league[0]->team_name,team_name1);
    char team_name2[]="Barcelona";
    league[1]=&T[1];
    strcpy(league[1]->team_name,team_name2);
    char team_name3[]="Argentina";
    league[2]=&T[2];
    strcpy(league[2]->team_name,team_name3);
    char team_name4[]="Manchester U";
    league[3]=&T[3];
    strcpy(league[3]->team_name,team_name4);
    char team_name5[]="Arsenal";
    league[4]=&T[4];
    strcpy(league[4]->team_name,team_name5);
    char team_name6[]="Chelsea";
    league[5]=&T[5];
    strcpy(league[5]->team_name,team_name6);
    char team_name7[]="Liverpool";
    league[6]=&T[6];
    strcpy(league[6]->team_name,team_name7);
    char team_name8[]="Milan";
    league[7]=&T[7];
    strcpy(league[7]->team_name,team_name8);
    
    result=game(league[0],league[1]);
    printf("The Winner team is : %s\n",result.team_name);
    
    //Part B:
    printf("\n********FIFA Tournament*******\n");
    tournament(league);
    printf("The Winner of the tournament is : %s\n",result.team_name);
    
    // Part C:
    league[0]->handicap=4;
    league[1]->handicap=2;
    league[2]->handicap=1;
    league[3]->handicap=5;
    league[4]->handicap=2;
    league[5]->handicap=3;
    league[6]->handicap=1;
    league[7]->handicap=4;
    
    printf("\n********FIFA Handicap Tournament*******\n");
    tournament_handicap(league);
    printf("The Winner of the Handicap tournament is : %s\n",result.team_name);
    return 0;
}

struct Team tournament(struct Team *team[])
{
    //round 1:
    struct Team winner[4];
    
    for (int i = 0,j=0; i < 8; i+=2,j++)
    {
        winner[j]=game(team[i],team[i+1]);
        printf("The Winner team is : %s\n",winner[j].team_name);
    }
    //round 2:
    int j=0;
    for (int k = 0; k < 2; ++k)
    {
        winner[j]=game(&winner[k],&winner[k+1]);
        printf("The Winner team is : %s\n",winner[j].team_name);
        j++;
    }
    //round 3:
    result=game(&winner[0],&winner[1]);
    
    return result;
    
}
struct Team game(struct Team *team1,struct Team *team2)
{
    
    int team1_score,team2_score;
    team1_score=rand()%10;
    team2_score=rand()%10;
    
    printf("\nThe participating teams are : Team 1:%s Team 2:%s\n",team1->team_name,team2->team_name);
    printf("The scores after the match are : Team 1: %d Team 2: %d\n",team1_score,team2_score);
    if(team1_score>team2_score)
        return *team1;
    else
        return *team2;
}

struct Team game_handicap(struct Team *team1,struct Team *team2)
{
    
    int team1_score,team2_score;
    team1_score=rand()%10;
    team2_score=rand()%10;
    
    printf("\nThe participating handicap teams are : Team 1:%s Team 2:%s\n",team1->team_name,team2->team_name);
    printf("\nThe real scores before adding handicap weightage are  : Team 1: %d Team 2: %d\n",team1_score,team2_score);
    
    // giving weightage to the team who have greater handicap value
    if(team1->handicap>team2->handicap)
        team1_score+=team1->handicap;
        else
            team2_score+=team2->handicap;
            
            printf("The scores after the weightage are : Team 1: %d Team 2: %d\n",team1_score,team2_score);
            if(team1_score>team2_score)
                return *team1;
            else
                return *team2;
}

struct Team tournament_handicap(struct Team *team[])
{
    //round 1:
    struct Team winner[4];
    
    for (int i = 0,j=0; i < 8; i+=2,j++)
    {
        winner[j]=game_handicap(team[i],team[i+1]);
        printf("The Winner team is : %s\n",winner[j].team_name);
    }
    //round 2:
    int j=0;
    for (int k = 0; k < 2; ++k)
    {
        winner[j]=game_handicap(&winner[k],&winner[k+1]);
        printf("The Winner team is : %s\n",winner[j].team_name);
        j++;
    }
    //round 3:
    result=game_handicap(&winner[0],&winner[1]);
    
    return result;
    
}
