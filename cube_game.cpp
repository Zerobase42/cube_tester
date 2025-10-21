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

HANDLE g_hBuffer[2];
int g_nScreenIndex = 0;

void InitDoubleBuffer() {
    g_hBuffer[0] = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    g_hBuffer[1] = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    SetConsoleActiveScreenBuffer(g_hBuffer[0]);
    CONSOLE_CURSOR_INFO cursorInfo = { 0, FALSE };
    SetConsoleCursorInfo(g_hBuffer[0], &cursorInfo);
    SetConsoleCursorInfo(g_hBuffer[1], &cursorInfo);
}
void gotoxy(int x, int y) {
    COORD Pos = { SHORT(2 * x), SHORT(y) };
    SetConsoleCursorPosition(g_hBuffer[g_nScreenIndex], Pos);
}
void gotoprt(int x, int y, const string &s) {
    gotoxy(x, y);
    DWORD dw;
    WriteConsoleA(g_hBuffer[g_nScreenIndex], s.c_str(), s.size(), &dw, NULL);
}
void textcolor(int colorNum) {
    SetConsoleTextAttribute(g_hBuffer[g_nScreenIndex], colorNum);
}
void FlipBuffer(){
    SetConsoleActiveScreenBuffer(g_hBuffer[g_nScreenIndex]);
    g_nScreenIndex = !g_nScreenIndex;
    COORD coord = {0,0};
    DWORD dw;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(g_hBuffer[g_nScreenIndex], &csbi);
    FillConsoleOutputCharacter(g_hBuffer[g_nScreenIndex], ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &dw);
    FillConsoleOutputAttribute(g_hBuffer[g_nScreenIndex], csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &dw);
    SetConsoleCursorPosition(g_hBuffer[g_nScreenIndex], coord);
}


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
    FlipBuffer();
    gotoprt(5,1,"=============================================");
    gotoprt(5,2,"============= 3Blind Helper Game ============");
    gotoprt(5,3,"=============================================");
    gotoprt(7,6,"Start : Press Space");
    gotoprt(7,7,"Quit : Press Q");
    gotoprt(5,10,"=============================================");
    gotoprt(9,12,"by. 0B42");
}
bool ReadyGame(){
    while(1){
        DrawReadyGame();
        int key=GetKeyDown();
        if(key==SPACE)return true;
        else if(key=='q'||key=='Q')break;
    }
    return false;
}

void DrawMenuLetter(){
    FlipBuffer();
    gotoprt(0,1,"=============================================");
    string whatToPrint="";
    for(int i=0;i<3;i++){
        whatToPrint="";
        for(int j=0;j<3;j++){
            if(selectLetteringScheme)whatToPrint+=SPEFFZ[0][i][j]+' ';
            else whatToPrint+=CHINESE[0][i][j]+' ';
            whatToPrint+=' ';
        }
        gotoprt(8,3+i,whatToPrint);
    }
    for(int i=0;i<3;i++){
        whatToPrint="";
        gotoxy(5,6+i);
        for(int j=0;j<4;j++){
            for(int k=0;k<3;k++){
                if(selectLetteringScheme) whatToPrint+=SPEFFZ[j][i][k]+' ';
                else whatToPrint+=CHINESE[j][i][k]+' ';
                whatToPrint+=' ';
            }
        }
        gotoprt(5,6+i,whatToPrint);
    }
    for(int i=0;i<3;i++){
        gotoxy(8,9+i);
        whatToPrint="";
        for(int j=0;j<3;j++){
            if(selectLetteringScheme) whatToPrint+=SPEFFZ[5][i][j]+' ';
            else whatToPrint+=CHINESE[5][i][j]+' ';
            whatToPrint+=' ';
        }
        gotoprt(8,9+i,whatToPrint);
    }
    whatToPrint="< ";
    if(selectLetteringScheme) whatToPrint+="Speffz";
    else whatToPrint+="Chinese";
    whatToPrint+=" >";
    gotoprt(8,14,whatToPrint);
    gotoprt(6,16,"Quit this menu : Press Q");

    gotoprt(0,18,"=============================================");
    gotoprt(9,19,"by. 0B42");
}
void MenuLetter(){
    while(1){
        DrawMenuLetter();
        int key=GetKeyDown();
        if(key==LEFT_KEY||key==RIGHT_KEY)
            selectLetteringScheme^=1;
        else if(key=='q'||key=='Q')
            return;
    }
}

int ColortoInt(int color){
    int ColorList[6]={WHITE,DARK_YELLOW,GREEN,RED,BLUE,YELLOW};
    return ColorList[color];
}
void DrawSelectColorUF(int arrow){
    FlipBuffer();

    gotoprt(0,1,"=============================================");
    gotoprt(5,2,"[ FACE COLOR SELECT ]");
    gotoprt(0,3,"=============================================");

    gotoprt(3,5,"Use < > or 0~5 to change color");
    gotoprt(3,6,"Press [U] or [F] to switch target");
    gotoprt(3,7,"Quit : [Q]");

    // UP / FRONT 제목
    gotoprt(2,9,"UP FACE");
    gotoprt(11,9,"FRONT FACE");

    // 선택 화살표
    if(arrow==1) gotoprt(0,9,">");
    else gotoprt(10,9,">");

    // 색 박스 크기 상수
    int boxWidth = 13;
    int boxHeight = 3;

    // ── UP FACE 박스
    gotoprt(2,11,"+-------------+");
    gotoprt(2,12,"|             |");
    gotoprt(2,13,"+-------------+");
    // 색 글자 중앙 정렬
    string upColor = cubeColor[UP_COLOR];
    while(upColor.size() < 11) upColor += " ";
    textcolor(ColortoInt(UP_COLOR));
    gotoprt(4,12,upColor);
    textcolor(WHITE);

    // ── FRONT FACE 박스
    gotoprt(12,11,"+-------------+");
    gotoprt(12,12,"|             |");
    gotoprt(12,13,"+-------------+");
    string frColor = cubeColor[FRONT_COLOR];
    while(frColor.size() < 11) frColor += " ";
    textcolor(ColortoInt(FRONT_COLOR));
    gotoprt(14,12,frColor);
    textcolor(WHITE);

    // 하단 색 번호 안내
    gotoprt(2,16,"[0]WHITE  [1]ORANGE  [2]GREEN");
    gotoprt(4,17,"[3]RED  [4]BLUE  [5]YELLOW");

    gotoprt(0,18,"=============================================");
    gotoprt(9,19,"by. 0B42");
}

void OverflowCheck(){
    if(UP_COLOR<0)UP_COLOR=5;
    if(FRONT_COLOR<0)FRONT_COLOR=5;
    if(UP_COLOR>5)UP_COLOR=0;
    if(FRONT_COLOR>5)FRONT_COLOR=0;
}
int oppositeFace(int color){
    int opp[6]={5,3,4,1,2,0};
    return opp[color];
}
void SelectColorUF(){
    int arrow=0; // 1 = UP selected, 0 = FRONT selected
    while(1){
        DrawSelectColorUF(arrow);
        int key=GetKeyDown();
        if(key=='q'||key=='Q')
            break;

        if(key=='u'||key=='U'){
            arrow=1;
            continue;
        }
        if(key=='f'||key=='F'){
            arrow=0;
            continue;
        }

        // 방향키 처리 (연속 스킵 포함)
        if(key==LEFT_KEY || key==RIGHT_KEY){
            int *target = arrow ? &UP_COLOR : &FRONT_COLOR;
            int dir = (key==LEFT_KEY ? -1 : 1);

            // 한 칸 이동
            *target += dir;
            OverflowCheck();

            // 유효한 조합(같거나 반대면 안됨)이 될 때까지 계속 이동
            // (만약 전체 6색이 모두 불가능한 경우는 이 루프가 도는 것을 방지해야
            // 하지만 UP vs FRONT라는 한 쌍에선 항상 적어도 4개의 유효색이 존재)
            while (UP_COLOR == FRONT_COLOR || oppositeFace(UP_COLOR) == FRONT_COLOR) {
                *target += dir;
                OverflowCheck();
            }
            continue;
        }

        // 숫자키 직접 지정 (0~5)
        if(key >= '0' && key <= '5'){
            int newColor = key - '0';
            if(arrow){
                // UP 선택 상태에서 숫자 입력
                UP_COLOR = newColor;
                // 만약 현재 FRONT와 같거나 반대면, newColor에서 한 칸 앞으로(우측) 이동하여 유효 색 찾음
                if(UP_COLOR == FRONT_COLOR || oppositeFace(UP_COLOR) == FRONT_COLOR){
                    // 시계방향(+)으로 한 칸씩 이동하여 유효색을 찾음
                    int tryColor = (newColor + 1) % 6;
                    while (tryColor == FRONT_COLOR || oppositeFace(tryColor) == FRONT_COLOR){
                        tryColor = (tryColor + 1) % 6;
                    }
                    UP_COLOR = tryColor;
                }
            } else {
                // FRONT 선택 상태에서 숫자 입력
                FRONT_COLOR = newColor;
                if(UP_COLOR == FRONT_COLOR || oppositeFace(UP_COLOR) == FRONT_COLOR){
                    int tryColor = (newColor + 1) % 6;
                    while (UP_COLOR == tryColor || oppositeFace(UP_COLOR) == tryColor){
                        tryColor = (tryColor + 1) % 6;
                    }
                    FRONT_COLOR = tryColor;
                }
            }
            continue;
        }

        // 그 외 키는 무시하고 다시 루프
    }
}

void DrawMenu(){
    FlipBuffer();
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
    while(1){
        DrawMenu();
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
    InitDoubleBuffer();
    bool isStart=false;
    while(true){
        isStart=ReadyGame();
        if(isStart)StartGame();
        else break;
    }
    return 0;
}