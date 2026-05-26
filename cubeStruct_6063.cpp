#include <cstdio>
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx,avx2,fma")
using namespace std;
class Cube{
#define C(f,r,c) cube[f*9+r*3+c]
public:
    char cube[6][3][3];
private:
    const char* color="wyrogb";
    static constexpr int ord[4]={4,2,5,3};
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
        char t;
        t=cube[f][0][0];
        cube[f][0][0]=cube[f][2][0];
        cube[f][2][0]=cube[f][2][2];
        cube[f][2][2]=cube[f][0][2];
        cube[f][0][2]=t;
        t=cube[f][0][1];
        cube[f][0][1]=cube[f][1][0];
        cube[f][1][0]=cube[f][2][1];
        cube[f][2][1]=cube[f][1][2];
        cube[f][1][2]=t;
    }
public:
    void turn(int s,int d){
        static const int ri[2][3]={{0,1,2},{2,1,0}};
        const int *p,*q;
        int cnt=(d==2)?2:(d==-1?3:1);
        while(cnt--){
            rotate_face((34644>>(s*3))&7);
            switch(s){
            case 4:case 5:{
                int a=(s-4)<<1;
                const int ts[2][4]={{2,5,3,4},{2,4,3,5}};
                p=ts[s-4];
                for(int i=0,t;i<3;i++){
                    t=cube[p[0]][a][i];
                    for(int j=0;j<3;j++)cube[p[j]][a][i]=cube[p[j+1]][a][i];
                    cube[p[3]][a][i]=t;
                }
            }break;
            case 1:case 3:{
                int C[3],t=s>>1;
                static constexpr int t1[2]={0,2},t2[2]={4,5};
                p=ri[t];q=ri[t^1];
                for(int i=0;i<3;i++){
                    C[i]=cube[0][t1[t^1]][i];
                    cube[0][t1[t^1]][i]=cube[t2[t]][q[i]][2];
                }
                for(int i=0;i<3;i++){
                    cube[t2[t]][i][2]=cube[1][t1[t]][p[i]];
                    cube[1][t1[t]][p[i]]=cube[t2[t^1]][2-i][0];
                }
                for(int i=0;i<3;i++)
                    cube[t2[t^1]][p[i]][0]=C[i];
            }break;
            case 0:case 2:{
                int C[3],t=s>>1;
                static constexpr int t3[]={2,3};
                p=ri[t];q=ri[t^1];
                for(int i=0;i<3;i++){
                    C[i]=cube[0][i][s];
                    cube[0][i][s]=cube[t3[t^1]][q[i]][2];
                }
                for(int i=0;i<3;i++)cube[t3[t^1]][i][2]=cube[1][q[i]][s];
                for(int i=0;i<3;i++){
                    cube[1][i][s]=cube[t3[t]][p[i]][0];
                    cube[t3[t]][p[i]][0]=C[i];
                }
            }break;
            }
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