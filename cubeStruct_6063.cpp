#include <cstdio>
#pragma GCC optimize("unroll-loops")
using namespace std;
class Cube{
public:
    char cube[6][3][3];
private:
    const char* color="wyrogb";
    const int ord[4]={4,2,5,3};
public:
    void read_cube(){
        for(int i=0;i<3;i++)for(int j=0;j<3;j++)scanf(" %c",&cube[0][i][j]);
        for(int i=0;i<3;i++){
            for(int k=0;k<4;k++){
                int f=ord[k];
                for(int j=0;j<3;j++)scanf(" %c",&cube[f][i][j]);
            }
        }
        for(int i=0;i<3;i++)for(int j=0;j<3;j++)scanf(" %c",&cube[1][i][j]);
    }
    void pretty_print(){
        for(int i=0;i<3;i++){
            printf("      ");
            for(int j=0;j<3;j++)printf("%c ",cube[0][i][j]);
            puts("");
        }
        for(int i=0;i<3;i++){
            for(int k=0;k<4;k++){
                int f=ord[k];
                for(int j=0;j<3;j++)printf("%c ",cube[f][i][j]);
            }
            puts("");
        }
        for(int i=0;i<3;i++){
            printf("      ");
            for(int j=0;j<3;j++)printf("%c ",cube[1][i][j]);
            puts("");
        }
        puts("");
    }
private:
    void rotate_face(int f){
        int C[3][3];
        for(int i=0;i<3;i++)for(int j=0;j<3;j++)C[i][j]=cube[f][i][j];
        for(int i=0;i<3;i++)for(int j=0;j<3;j++)cube[f][j][2-i]=C[i][j];
    }
public:
    void turn(int s,int d){
        if(d!=1){
            turn(s,1);turn(s,1);
            if(d==-1)turn(s,1);
            return;
        }
        rotate_face((34644>>(s*3))&7);
        if(s==4||s==5){
            int ts[]={2,9-s,3,s},a=(s-4)*2;
            for(int i=0,t;i<3;i++){
                t=cube[ts[0]][a][i];
                for(int j=0;j<3;j++)cube[ts[j]][a][i]=cube[ts[j+1]][a][i];
                cube[ts[3]][a][i]=t;
            }
        }
        if(s==1||s==3){
            int C[3];const int tmp[]={s-1,(s+7)>>1};
            for(int i=0;i<3;i++)
                C[i]=cube[0][2-tmp[0]][i];
            for(int i=0;i<3;i++)
                cube[0][2-tmp[0]][i]=cube[tmp[1]][s==1?2-i:i][2];
            for(int i=0;i<3;i++)
                cube[tmp[1]][i][2]=cube[1][tmp[0]][s==1?i:2-i];
            for(int i=0;i<3;i++)
                cube[1][tmp[0]][s==1?i:2-i]=cube[9-tmp[1]][2-i][0];
            for(int i=0;i<3;i++)
                cube[9-tmp[1]][s==1?i:2-i][0]=C[i];
        }
        if(s==0){
            int C[3];
            for(int i=0;i<3;i++)
                C[i]=cube[0][i][0];
            for(int i=0;i<3;i++)
                cube[0][i][0]=cube[3][2-i][2];
            for(int i=0;i<3;i++)
                cube[3][i][2]=cube[1][2-i][0];
            for(int i=0;i<3;i++)
                cube[1][i][0]=cube[2][i][0];
            for(int i=0;i<3;i++)
                cube[2][i][0]=C[i];
        }
        if(s==2){
            int C[3];
            for(int i=0;i<3;i++)
                C[i]=cube[0][i][2];
            for(int i=0;i<3;i++)
                cube[0][i][2]=cube[2][i][2];
            for(int i=0;i<3;i++)
                cube[2][i][2]=cube[1][i][2];
            for(int i=0;i<3;i++)
                cube[1][i][2]=cube[3][2-i][0];
            for(int i=0;i<3;i++)
                cube[3][2-i][0]=C[i];
        }
    }
};
int main(){
    int T;
    scanf("%d",&T);
    for(int tc=1;tc<=T;tc++){
        Cube C;
        C.read_cube();
        int q;
        scanf("%d",&q);
        while(q--){
            int s,d;
            scanf("%d%d",&s,&d);
            C.turn(s,d);
        }
        printf("Scenario #%d:\n",tc);
        C.pretty_print();
    }
    return 0;
}