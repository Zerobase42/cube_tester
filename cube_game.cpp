#include<iostream>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#include<Windows.h>
#include<vector>
using namespace std;

void gotoxy(int x,int y){
    COORD Pos;
    Pos.X=2*x;
    Pos.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Pos);
}
int GetKeyDown(){
    if(_kbhit()!=0)
        return _getch();
    return 0;
}
void SetConsoleView(){
    system("mode con:cols=50 lines=20");
    system("title [CubeGame] by.0B42");
}
void DrawReadyGame(){
    system("cls");
    gotoxy(5,2);
    cout<<"==============================";
    gotoxy(5,3);
    cout<<"===== 3Blind Helper Game =====";
    gotoxy(5,4);
    cout<<"==============================";
    gotoxy(7,6);
    cout<<"Start : Press Space";
    gotoxy(7,7);
    cout<<"Quit : Press Q";
    gotoxy(9,14);
    cout<<"by. 0B42";
}
bool ReadyGame(){
    DrawReadyGame();
    while(1){
        int key=GetKeyDown();
        if(key==' ')return true;
        else if(key=='q'||key=='Q')break;
    }
    return false;
}

void StartGame(){
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