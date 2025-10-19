#include<iostream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#include<Windows.h>
#include<vector>
using namespace std;
#define MAGIC_KEY 224
#define SPACE 32

enum KEYBOARD{
    UP=72,
    LEFT=75,
    RIGHT=77,
    DOWN=80
};

int selectLetteringScheme;

const char Speffz[6][3][3]={{
        {'A','B','C'},
        {'D',' ','B'},
        {'D','C','C'}
    },{
        {'E','E','F'},
        {'H',' ','F'},
        {'H','G','G'}
    },{
        {'I','I','J'},
        {'L',' ','J'},
        {'L','K','K'}
    },{
        {'M','M','N'},
        {'P',' ','N'},
        {'P','O','O'}
    },{
        {'Q','Q','R'},
        {'T',' ','R'},
        {'T','S','S'}
    },{
        {'U','U','V'},
        {'X',' ','V'},
        {'X','W','W'}
    }
};

const char Chinese[6][3][3]={{
        {'D','E','G'},
        {'C',' ','G'},
        {'A','A','J'}
    },{
        {'E','D','C'},
        {'X',' ','T'},
        {'Q','L','M'}
    },{
        {'B','B','L'},
        {'S',' ','Q'},
        {'N','J','Y'}
    },{
        {'K','H','I'},
        {'R',' ','Z'},
        {'Z','P','S'}
    },{
        {'H','F','F'},
        {'Y',' ','W'},
        {'T','N','P'}
    },{
        {'W','I','X'},
        {'K',' ','O'},
        {'O','M','R'}
    }
};
/*
const char Korean[6][3][3]={{
        {'D','E','G'},
        {'C',' ','G'},
        {'A','A','J'}
    },{
        {'E','D','C'},
        {'X',' ','T'},
        {'Q','L','M'}
    },{
        {'B','B','L'},
        {'S',' ','Q'},
        {'N','J','Y'}
    },{
        {'K','H','I'},
        {'R',' ','Z'},
        {'Z','P','S'}
    },{
        {'H','F','F'},
        {'Y',' ','W'},
        {'T','N','P'}
    },{
        {'W','I','X'},
        {'K',' ','O'},
        {'O','M','R'}
    }
};
*/

void gotoxy(int x,int y){
    COORD Pos;
    Pos.X=2*x;
    Pos.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Pos);
}

void gotoprt(int x,int y,string s){
    gotoxy(x,y);
    cout<<s;
}

int GetKeyDown(){
    int key;
    if(_kbhit()!=0)
        key=_getch();
        if(key==MAGIC_KEY)return _getch()+200;
        else return key;
    return 0;
}
void SetConsoleView(){
    system("mode con:cols=35 lines=20");
    system("title [CubeGame] by.0B42");
}
void DrawReadyGame(){
    system("cls");
    gotoprt(5,2,"==============================");
    gotoprt(5,3,"===== 3Blind Helper Game =====");
    gotoprt(5,4,"==============================");
    gotoprt(7,6,"Start : Press Space");
    gotoprt(7,7,"Quit : Press Q");
    gotoprt(9,14,"by. 0B42");
}

void DrawMenu(){
    system("cls");
    gotoprt(5,2,"===================================");
    gotoprt(10,4,"Menu");
    gotoprt(7,6,"Start : Press Space");
    gotoprt(7,8,"Select Lettering Scheme : Press S");
    gotoprt(7,10,"Select U&F color : Press C");
    gotoprt(5,12,"===================================");
    gotoprt(9,14,"by. 0B42");
}

bool ReadyGame(){
    DrawReadyGame();
    while(1){
        int key=GetKeyDown();
        if(key==SPACE)return true;
        else if(key=='q'||key=='Q')break;
    }
    return false;
}

void Menu_Letter(){
    return;
}

void StartMenu(){
    DrawMenu();
    while(1){
        int key=GetKeyDown();
        if(key==SPACE)break;
        else if(key=='s'||key=='S'){
            Menu_Letter();
            DrawMenu();
        }
    }
}

void StartGame(){
    StartMenu();
}

int main(){
    SetConsoleView();
    bool isStart=false;
    while(1){
        isStart=ReadyGame();
        if(isStart)StartGame();
        else break;
    }
    return 0;
}