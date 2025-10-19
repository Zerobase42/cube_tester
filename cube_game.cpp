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

int main(){
    return 0;
}