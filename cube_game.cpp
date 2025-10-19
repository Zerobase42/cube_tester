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

enum{
    UP_KEY=272,
    LEFT_KEY=275,
    RIGHT_KEY=277,
    DOWN_KEY=280
};

int selectLetteringScheme=1;

const char SPEFFZ[6][3][3]={{
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

const char CHINESE[6][3][3]={{
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
    gotoprt(5,16,"===================================");
    gotoprt(9,18,"by. 0B42");
}

void DrawMenu(){
    system("cls");
    gotoprt(0,2,"===================================");
    gotoprt(10,4,"Menu");
    gotoprt(7,6,"Start : Press Space");
    gotoprt(7,8,"Select Lettering Scheme : Press S");
    gotoprt(7,10,"Select U&F color : Press C");
    gotoprt(0,16,"===================================");
    gotoprt(9,18,"by. 0B42");
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

void drawMenuLetter(int letteringScheme){
    gotoprt(0,2,"===================================");
    for(int i=0;i<3;i++){
        gotoxy(12,4+i);
        for(int j=0;j<3;j++){
            if(letteringScheme) cout<<SPEFFZ[0][i][j]<<' ';
            else cout<<CHINESE[0][i][j]<<' ';
        }
        cout<<"\n";
    }
    for(int i=0;i<3;i++){
        gotoxy(5,7+i);
        for(int j=0;j<4;j++){
            for(int k=0;k<3;k++){
                if(letteringScheme) cout<<SPEFFZ[j][i][k]<<' ';
                else cout<<CHINESE[j][i][k]<<' ';
            }
        }
        cout<<'\n';
    }
    for(int i=0;i<3;i++){
        gotoxy(12,11+i);
        for(int j=0;j<3;j++){
            if(letteringScheme) cout<<SPEFFZ[5][i][j]<<' ';
            else cout<<CHINESE[5][i][j]<<' ';
        }
        cout<<"\n";
    }
    gotoxy(6,14);
    cout<<'< ';
    if(letteringScheme)
        cout<<"Speffz";
    else
        cout<<"Chinese";
    
    cout<<' >';
    gotoprt(6,15,"Quit this menu : Press Q");

    gotoprt(0,16,"===================================");
    gotoprt(9,18,"by. 0B42");
}

void Menu_Letter(){
    drawMenuLetter(selectLetteringScheme);
    while(1){
        int key=GetKeyDown();
        if(key==LEFT_KEY||key==RIGHT_KEY)
            drawMenuLetter(selectLetteringScheme^=1);
        else if(key=='q'||key=='Q')
            return;
    }
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