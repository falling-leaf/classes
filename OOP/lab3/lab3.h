#ifndef LAB3_H
#define LAB3_H

#include <bits/stdc++.h>

using namespace std;

class character//the class the represents the character
{
private:
    string name;//name of the character
    int life;//life of the character
    int attack;//attack of the character
    int position;//position of the character
    bool withprincess;//whether the character is taking the princess
    bool withsign;//whether the character is taking the white cyrstal
    bool trueend;//one of the end of the game
    bool fail;//one of the end of the game
public:
    void set_withsign();//set the value of withsign
    void set_character();//set the value of character
    void set_trueend();//set the value of trueend
    void player_event();//the event of the player
    void player_strengthen();//reset the value of attack
    int put_attack();//output the attack
    void change_position(int i);//change the position of the character
    bool victory();//judge if satisfies the victory
    bool failure();//judge if satisfies the failure
    bool put_withsign();//output the value of withsign
};

class room//the class of the room
{
private:
    string state;//the state of the room
    int exit;//the exit of the room
    int monster_attack;//the attack of the monster
    int north_room;//if no exit, is NULL.
    int south_room;
    int east_room;
    int west_room;
public:
    void set_state(string str);//set the state of the room
    void set_exit(int east, int west, int south, int north);//set the exit of the room
    void set_monster(int attack);//set the monster of the room
    int put_monster();//output the monster of the room
    string put_state();//output the state of the room
    int put_exit();//output the exit of the room
    void put_room();//output the room
    int put_direction(string str);//give the direction of the input
};

int mode;//the mode of the game
int level;//the level of the game
room map_data[3][20];//the map data
character warrior;//the character
void set_room();//initialize the room
void set_echo();//initialize the echo
#endif