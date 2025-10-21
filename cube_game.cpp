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

void CursorView(){
    CONSOLE_CURSOR_INFO cursorInfo={ 0,};
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

enum{
    UP_KEY=272,
    LEFT_KEY=275,
    RIGHT_KEY=277,
    DOWN_KEY=280
};

void textcolor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}
enum{
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_SKYBLUE,
    DARK_RED,
    DARK_VOILET,
    DARK_YELLOW,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    SKYBLUE,
    RED,
    VIOLET,
    YELLOW,
    WHITE
};

int selectLetteringScheme;
int UP_COLOR=0,FRONT_COLOR=2;
const char cubeColor[6][7]={"WHITE ","ORANGE","GREEN ","  RED "," BLUE ","YELLOW"};

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
    if(_kbhit()!=0){
        key=_getch();
        if(key==MAGIC_KEY)return _getch()+200;
        else return key;
    }
    return 0;
}

void SetConsoleView(){
    system("mode con:cols=55 lines=25");
    system("title [CubeGame] by.0B42");
}

void DrawReadyGame(){
    system("cls");
    gotoprt(5,1,"=============================================");
    gotoprt(5,2,"============= 3Blind Helper Game ============");
    gotoprt(5,3,"=============================================");
    gotoprt(7,6,"Start : Press Space");
    gotoprt(7,7,"Quit : Press Q");
    gotoprt(5,10,"=============================================");
    gotoprt(9,12,"by. 0B42");
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

void DrawMenuLetter(){
    selectLetteringScheme^=1;
    system("cls");
    gotoprt(0,1,"=============================================");
    for(int i=0;i<3;i++){
        gotoxy(8,3+i);
        for(int j=0;j<3;j++){
            if(selectLetteringScheme)cout<<SPEFFZ[0][i][j]<<' ';
            else cout<<CHINESE[0][i][j]<<' ';
        }
        cout<<"\n";
    }
    for(int i=0;i<3;i++){
        gotoxy(5,6+i);
        for(int j=0;j<4;j++){
            for(int k=0;k<3;k++){
                if(selectLetteringScheme) cout<<SPEFFZ[j][i][k]<<' ';
                else cout<<CHINESE[j][i][k]<<' ';
            }
        }
        cout<<'\n';
    }
    for(int i=0;i<3;i++){
        gotoxy(8,9+i);
        for(int j=0;j<3;j++){
            if(selectLetteringScheme) cout<<SPEFFZ[5][i][j]<<' ';
            else cout<<CHINESE[5][i][j]<<' ';
        }
        cout<<"\n";
    }
    gotoxy(8,14);
    cout<<"< ";
    if(selectLetteringScheme)
        cout<<"Speffz";
    else
        cout<<"Chinese";
    
    cout<<" >";
    gotoprt(6,16,"Quit this menu : Press Q");

    gotoprt(0,18,"=============================================");
    gotoprt(9,19,"by. 0B42");
}
void MenuLetter(){
    DrawMenuLetter();
    while(1){
        int key=GetKeyDown();
        if(key==LEFT_KEY||key==RIGHT_KEY)
            DrawMenuLetter();
        else if(key=='q'||key=='Q')
            return;
    }
}

int ColortoInt(int color){
    int ColorList[6]={WHITE,DARK_YELLOW,GREEN,RED,BLUE,YELLOW};
    return ColorList[color];
}
void DrawSelectColorUF(int arrow){
    system("cls");
    gotoprt(0,1,"=============================================");
    gotoprt(3,3,"SELECT FACE COLOR POSITION");
    gotoprt(4,8,"UP COLOR          FRONT COLOR");
    gotoxy(3,10);
    cout<<"< ";
    textcolor(ColortoInt(UP_COLOR));
    cout<<cubeColor[UP_COLOR];
    textcolor(WHITE);
    cout<<" >";

    gotoxy(13,10);
    cout<<"< ";
    textcolor(ColortoInt(FRONT_COLOR));
    cout<<cubeColor[FRONT_COLOR];
    textcolor(WHITE);
    cout<<" >";
    gotoprt(arrow?6:16,12,"^");
    gotoprt(6,14,"UP: PRESS U, FRONT: PRESS F");
    gotoprt(6,16,"Quit this menu : Press Q");
    gotoprt(0,18,"=============================================");
    gotoprt(9,19,"by. 0B42");
    return;
}
void OverflowCheck(){
    if(UP_COLOR<0)UP_COLOR=5;
    if(FRONT_COLOR<0)FRONT_COLOR=5;
    if(UP_COLOR>5)UP_COLOR=0;
    if(FRONT_COLOR>5)FRONT_COLOR=0;
}
void SelectColorUF(){
    DrawSelectColorUF(0);
    int arrow=0;
    while(1){
        int key=GetKeyDown();
        if(key=='q'||key=='Q')
            break;
        if(key=='u'||key=='U'){
            arrow=1;
            DrawSelectColorUF(arrow);
        }
        if(key=='f'||key=='F'){
            arrow=0;
            DrawSelectColorUF(arrow);
        }
        if(key==LEFT_KEY){
            if(arrow) UP_COLOR--;
            else FRONT_COLOR--;
            OverflowCheck();
            DrawSelectColorUF(arrow);
        }
        if(key==RIGHT_KEY){
            if(arrow) UP_COLOR++;
            else FRONT_COLOR++;
            OverflowCheck();
            DrawSelectColorUF(arrow);
        }
    }
}
void DrawMenu(){
    system("cls");
    gotoprt(0,1,"=============================================");
    gotoprt(10,3,"Menu");
    gotoprt(5,7,"Start : Press Space");
    gotoprt(5,9,"Select Lettering Scheme : Press S");
    gotoprt(5,11,"Select U&F color : Press C");
    gotoprt(5,13,"Quit : Press Q");
    gotoprt(0,18,"=============================================");
    gotoprt(9,19,"by. 0B42");
}
void StartMenu(){
    DrawMenu();
    while(1){
        int key=GetKeyDown();
        if(key==SPACE)break;
        else if(key=='q'||key=='Q')
            break;
        else if(key=='s'||key=='S'){
            MenuLetter();
            DrawMenu();
        }
        else if(key=='c'||key=='C'){
            SelectColorUF();
            DrawMenu();
        }
    }
}

void StartGame(){
    StartMenu();
    // game code to here
}

int main(){
    CursorView();
    SetConsoleView();
    bool isStart=false;
    while(true){
        isStart=ReadyGame();
        if(isStart)StartGame();
        else break;
    }
    return 0;
}