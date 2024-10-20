/*write at the front:
 *after I finish the project have I noted that "the position of monster and princess is random"
 *however, the original project has more fun than the rules required by the question.
 *in that way, I choose to save the original project as "mode 0", and the test project as "mode 1"
 *if you want to play mode 0, please read README.txt in the file to get the rule of the game.
 *if you just want to check if the program satisfies the requirement, then just choose "mode 0"
 *the important comments have been edited in the header file.
 */
#include <bits/stdc++.h>
#include "lab3.h"

using namespace std;

void character::set_character()
{
    life = 1;
    attack = 1;//set the attack of character
    position = 0;//the start room(lobby) is room 0
    withprincess = false;
    trueend = false;
    withsign = false;
    fail = false;
}

void character::change_position(int i)
{
    position = i;
}

void character::player_strengthen()
{
    attack++;
}

int character::put_attack()
{
    return attack;
}

void character::set_withsign()
{
    withsign = true;
}

bool character::put_withsign()
{
    return withsign;
}

void character::set_trueend()
{
    trueend = true;
}

void character::player_event()
{
    string state = map_data[level][position].put_state();//check the state of the room
    string next;//save the next command
    int exit = map_data[level][position].put_exit();//get the exit of the room
    int sword;
    int choice;
    cout << "you are in the " << state << " room..." << endl;//the first line in the room
    if (state == "monster")//monster room
    {
        if (mode == 1)//test mode: directly game over
        {
            cout << "game over!" << endl;
            fail = true;
            return;
        }
        else
        {
            cout << "the attack of the monster is: " << map_data[level][position].put_monster() << endl;
            if (warrior.put_attack() > map_data[level][position].put_monster())
            {
                if (map_data[level][position].put_monster() == 9)
                {
                    cout << "the warrior use the sword of devil to kill the monster.(input ENTER to continue)" << endl;
                    set_echo();
                    set_echo();
                    cout << "however, before the warrior is able to approach the room of princess, " << endl;
                    set_echo();
                    cout << "the soul of devil eventually occupys the mind of warrior." << endl;
                    set_echo();
                    if (warrior.put_withsign())
                    {
                        cout << "before your mind is lost, the white crystal in your pocket is shining." << endl;
                        set_echo();
                        cout << "crystal: I'm the bravery and persistence of all the former warriors." << endl;
                        set_echo();
                        cout << "crystal: We will protect you from the destruction of the devil." << endl;
                        set_echo();
                        cout << "crystal: please with our faith, save the princess!" << endl;
                        map_data[2][3].set_exit(4, 2, 6, 1);
                        map_data[2][3].set_state("space");
                    }
                    else
                    {
                        cout << "some days later, a new warrior get into the castle." << endl;
                        set_echo();
                        cout << "on the top of the castle, a strong \"monster\" stands..." << endl;
                        fail = true;
                        return;
                    }
                }
                else 
                {
                    cout << "you defeat the monster!" << endl;
                    map_data[level][position].set_state("space");
                }

            }
            else if (map_data[level][position].put_monster() == 9)
            {
                cout << "why the monster is so strong?(input ENTER to continue)" << endl;
                set_echo();
                set_echo();
                cout << "the princess is must behind it, but how can I defeat it?" << endl;
                set_echo();
                cout << "a voice out of the screen: at the end of the east, lies a huge dragon." << endl;
                set_echo();
                cout << "brave warrior, find the dragon and get the strength to defeat the monster!" << endl;
            } 
            else 
            {
                cout << "game over!" << endl;
                fail = true;
                return;
            }
        }
    }
    if (state == "princess")
    {
        cout << "princess: it is so brave that you come to save me, please go to the lobby to leave here!" << endl;
        withprincess = true;
    }
    if (state == "space")
    {
    }
    if (state == "crystal")
    {
        cout << "you get the rubia crystal, you are stronger!" << endl;
        warrior.player_strengthen();
        map_data[level][position].set_state("space");
    }
    if (state == "dragon")
    {
        cout << "dragon: young warrior, I know that you comes here to find the strength.(input ENTER to continue)" << endl;
        set_echo();
        set_echo();
        cout << "dragon: here is a sword of devil, which can destroy any monster." << endl;
        set_echo();
        cout << "dragon: however, the one who takes it may be affected by the soul in the sword." << endl;
        set_echo();
        cout << "dragon: even though it is possibly affect your mind, will you take it?" << endl;
        set_echo();
        cout << "choice: take it: 1 ; leave: 2" << endl;
        cin >> sword;
        while (sword != 1 && sword != 2)
        {
            cout << "invalid choice" << endl;
            cin >> sword;
        }
        cout << "dragon: OK, I will respect your choice." << endl;
        if (sword == 2)
        {
            cout << "the warrior leave the castle, with the rubia crystals, he soon becomes the best warrior.(input ENTER to continue)" << endl;
            set_echo();
            set_echo();
            cout << "he punish the one who is bad, do things to help the poor." << endl;
            set_echo();
            cout << "he falls in love with a beautiful girl, and stay with her to death." << endl;
            warrior.set_trueend();
            return;
        }
        else if (sword == 1)
        {
            cout << "you get the sword of devil, you feel that your attack hugely increases." << endl;
            for (sword = 1; sword < 100; sword++)
                warrior.player_strengthen();
        }
        map_data[level][position].set_state("space");
    }
    if (state == "white crystal")
    {
        cout << "you come to the crystal, unlike other rubia crystals, this crystal is shining pure white." << endl;
        warrior.player_strengthen();
        warrior.set_withsign();
        map_data[level][position].set_state("space");
    }
    cout << "now your attack is: " << warrior.put_attack() << endl;
    cout << "There are " << exit << " exits:";
    map_data[level][position].put_room();
    cout << "Enter your command:" << endl;
    cin >> next >> next;
    while (map_data[level][position].put_direction(next) == -1)
    {
        cout << "this is an invalid command, please enter like: go xxxx" << endl;
        cout << "or confirm that you are able to arrive that direction" << endl;
        cin >> next >> next;
    }
    choice = map_data[level][position].put_direction(next);
    cout << endl;
    warrior.change_position(choice);
}

bool character::victory()
{
    if (withprincess == true && position == 0 || trueend == true)
        return true;
    else return false;
}

bool character::failure()
{
    return fail;
}

void room::set_state(string str)
{
    state = str;
}

string room::put_state()
{
    return state;
}

void room::set_exit(int east, int west, int south, int north)
{
    east_room = east;
    west_room = west;
    south_room = south;
    north_room = north;
    exit = 0;
    if (east != -1)
        exit++;
    if (west != -1)
        exit++;
    if (south != -1)
        exit++;
    if (north != -1)
        exit++;
}

int room::put_exit()
{
    return exit;
}

void room::set_monster(int attack)
{
    monster_attack = attack;
}

int room::put_monster()
{
    return monster_attack;
}

void room::put_room()
{
    int i = 1;
    if (east_room != -1)
    {
        if (i != exit)
            cout << "east, ";
        else cout << "east.";
        i++;
    }
    if (west_room != -1)
    {
        if (i != exit)
            cout << "west, ";
        else cout << "west.";
        i++;
    }
    if (south_room != -1)
    {
        if (i != exit)
            cout << "south, ";
        else cout << "south.";
        i++;
    }
    if (north_room != -1)
    {
        if (i != exit)
            cout << "north, ";
        else cout << "north.";
    }
    cout << endl;
}

int room::put_direction(string str)
{
    if (str == "west")
        return west_room;
    if (str == "east")
        return east_room;
    if (str == "south")
        return south_room;
    if (str == "north")
        return north_room;
    else return -1;
}

void set_echo()
{
    string c;
    getline(cin, c);
}


void set_room()
{
    int temp = 0;
    int tempp = 0;
    cout << "before starting the game, please choose the mode of game:" << endl;
    cout << "original mode: 0(not satisfy the random position of monster/princess, but has more fun)" << endl;
    cout << "test mode: 1(the mode that satisfy the requirement of the question)" << endl;
    cout << "(note: the more information please read the README.txt)" << endl;
    mode = 2;
    cin >> mode;
    while (mode != 1 && mode != 0)
    {
        cout << "please input 0 or 1" << endl;
        cin >> mode;
    }
    if (mode == 0)
    {
        cout << "welcome to the game! please choose the difficuly:" << endl;
        cout << "note: in hard mode, there are some extra plots." << endl;
        cout << "note: there is some long conversations in hard mode, you can input ENTER to continue it." << endl;
        cout << "easy: 0/normal: 1/hard: 2" << endl;
        cin >> level;
        map_data[0][0].set_state("lobby");
        map_data[0][0].set_exit(-1, -1, -1, 1);
        map_data[0][1].set_state("space");
        map_data[0][1].set_exit(2, 3, 0, 4);
        map_data[0][4].set_state("princess");
        map_data[0][4].set_exit(-1, -1, 1, -1);
        map_data[0][2].set_state("monster");
        map_data[0][2].set_exit(-1, 1, -1, -1);
        map_data[0][2].set_monster(1);
        map_data[0][3].set_state("crystal");
        map_data[0][3].set_exit(1, -1, -1, -1);

        map_data[1][0].set_state("lobby");
        map_data[1][0].set_exit(-1, -1, -1, 8);
        map_data[1][1].set_state("monster");
        map_data[1][1].set_exit(2, -1, 4, -1);
        map_data[1][1].set_monster(2);
        map_data[1][2].set_state("princess");
        map_data[1][2].set_exit(3, 1, 5, -1);
        map_data[1][3].set_state("monster");
        map_data[1][3].set_exit(-1, 2, 6, -1);
        map_data[1][3].set_monster(2);
        map_data[1][4].set_state("crystal");
        map_data[1][4].set_exit(5, -1, 7, 1);
        map_data[1][5].set_state("monster");
        map_data[1][5].set_exit(6, 4, 8, 2);
        map_data[1][5].set_monster(2);
        map_data[1][6].set_state("crystal");
        map_data[1][6].set_exit(-1, 5, 9, 3);
        map_data[1][7].set_state("space");
        map_data[1][7].set_exit(8, -1, -1, 4);
        map_data[1][8].set_state("space");
        map_data[1][8].set_exit(9, 7, 0, 5);
        map_data[1][9].set_state("space");
        map_data[1][9].set_exit(-1, 8, -1, 6);

        map_data[2][0].set_state("lobby");
        map_data[2][0].set_exit(-1, -1, -1, 17);
        map_data[2][1].set_state("princess");
        map_data[2][1].set_exit(-1, -1, 3, -1);
        map_data[2][2].set_state("white crystal");
        map_data[2][2].set_exit(3, -1, 5, -1);
        map_data[2][3].set_state("monster");
        map_data[2][3].set_exit(4, 2, 6, -1);
        map_data[2][3].set_monster(9);
        map_data[2][4].set_state("monster");
        map_data[2][4].set_exit(-1, 3, 7, -1);
        map_data[2][4].set_monster(2);
        map_data[2][5].set_state("monster");
        map_data[2][5].set_exit(6, -1, 8, 2);
        map_data[2][5].set_monster(2);
        map_data[2][6].set_state("monster");
        map_data[2][6].set_exit(7, 5, 9, 3);
        map_data[2][6].set_monster(2);
        map_data[2][7].set_state("crystal");
        map_data[2][7].set_exit(-1, 6, 10, 4);
        map_data[2][8].set_state("crystal");
        map_data[2][8].set_exit(9, -1, 13, 5);
        map_data[2][9].set_state("monster");
        map_data[2][9].set_exit(10, 8, 14, 6);
        map_data[2][9].set_monster(1);

        map_data[2][10].set_state("monster");
        map_data[2][10].set_exit(11, 9, 15, 7);
        map_data[2][10].set_monster(4);
        map_data[2][11].set_state("space");
        map_data[2][11].set_exit(12, 10, -1, -1);
        map_data[2][12].set_state("dragon");
        map_data[2][12].set_exit(-1, 11, -1, -1);
        map_data[2][13].set_state("monster");
        map_data[2][13].set_exit(14, -1, 16, 8);
        map_data[2][13].set_monster(2);
        map_data[2][14].set_state("space");
        map_data[2][14].set_exit(15, 13, 17, 9);
        map_data[2][15].set_state("monster");
        map_data[2][15].set_exit(-1, 14, 18, 10);
        map_data[2][15].set_monster(2);
        map_data[2][16].set_state("crystal");
        map_data[2][16].set_exit(17, -1, -1, 13);
        map_data[2][17].set_state("space");
        map_data[2][17].set_exit(18, 16, 0, 14);
        map_data[2][18].set_state("crystal");
        map_data[2][18].set_exit(-1, 17, -1, 15);
    }
    else if (mode == 1)
    {
        level = 2;
        map_data[2][0].set_state("lobby");
        map_data[2][0].set_exit(-1, -1, -1, 17);
        map_data[2][1].set_state("space");
        map_data[2][1].set_exit(-1, -1, 3, -1);
        map_data[2][2].set_state("space");
        map_data[2][2].set_exit(3, -1, 5, -1);
        map_data[2][3].set_state("space");
        map_data[2][3].set_exit(4, 2, 6, -1);
        map_data[2][3].set_monster(9);
        map_data[2][4].set_state("space");
        map_data[2][4].set_exit(-1, 3, 7, -1);
        map_data[2][4].set_monster(2);
        map_data[2][5].set_state("space");
        map_data[2][5].set_exit(6, -1, 8, 2);
        map_data[2][5].set_monster(2);
        map_data[2][6].set_state("space");
        map_data[2][6].set_exit(7, 5, 9, 3);
        map_data[2][6].set_monster(2);
        map_data[2][7].set_state("space");
        map_data[2][7].set_exit(-1, 6, 10, 4);
        map_data[2][8].set_state("space");
        map_data[2][8].set_exit(9, -1, 13, 5);
        map_data[2][9].set_state("space");
        map_data[2][9].set_exit(10, 8, 14, 6);
        map_data[2][9].set_monster(1);

        map_data[2][10].set_state("space");
        map_data[2][10].set_exit(11, 9, 15, 7);
        map_data[2][10].set_monster(4);
        map_data[2][11].set_state("space");
        map_data[2][11].set_exit(12, 10, -1, -1);
        map_data[2][12].set_state("space");
        map_data[2][12].set_exit(-1, 11, -1, -1);
        map_data[2][13].set_state("space");
        map_data[2][13].set_exit(14, -1, 16, 8);
        map_data[2][13].set_monster(2);
        map_data[2][14].set_state("space");
        map_data[2][14].set_exit(15, 13, 17, 9);
        map_data[2][15].set_state("space");
        map_data[2][15].set_exit(-1, 14, 18, 10);
        map_data[2][15].set_monster(2);
        map_data[2][16].set_state("space");
        map_data[2][16].set_exit(17, -1, -1, 13);
        map_data[2][17].set_state("space");
        map_data[2][17].set_exit(18, 16, 0, 14);
        map_data[2][18].set_state("space");
        map_data[2][18].set_exit(-1, 17, -1, 15);
        srand(time(NULL));
        temp = rand()%18 + 1;
        map_data[2][temp].set_state("monster");
        tempp = rand()%18 + 1;
        while (tempp == temp)
            tempp = rand()%18 + 1;
        map_data[2][tempp].set_state("princess");
        
    }
}

int main()
{
    set_room();
    warrior.set_character();
    while (warrior.victory() ==false && warrior.failure() == false)
        warrior.player_event();
    if (warrior.victory() == true)
        cout << "you save the princess!" << endl;
    else if (warrior.failure() == true)
        cout << "you lose!" << endl;
}