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

char Speffz[6][3][3]={{
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
    },
};



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
        if(key==MAGIC_KEY)return _getch();
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

void StartGame(){
    DrawMenu();
    return;
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