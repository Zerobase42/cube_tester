#include<iostream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#include<Windows.h>
#include<vector>
#include<fstream>
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}