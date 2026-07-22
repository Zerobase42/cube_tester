#define DEBUG 1
/*
         00 01 02
         03 04 05
         06 07 08
36 37 38 09 10 11 18 19 20 27 28 29
39 40 41 12 13 14 21 22 23 30 31 32
42 43 44 15 16 17 24 25 26 33 34 35
         45 46 47
         48 49 50
         51 52 53
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx,avx2,fma")
#if DEBUG==1
#define debug(format,args...)fprintf(stderr,format,##args)
#else
#define debug(format,args...)
#endif
typedef struct{
    int*buf;
    int front;
    int rear;
    int size;
}queue;
static inline queue*initQ(int size){
    queue*q=(queue*)malloc(sizeof(queue));
    q->buf=(int*)malloc(sizeof(int)*size);
    q->front=0;
    q->rear=0;
    q->size=size;
    return q;
}
static inline void delQ(queue*q){
    free(q->buf);
    free(q);
}
static inline int emptyQ(const queue*q){
    return q->front==q->rear;
}
static inline void pushQ(queue *q,int x){
    if(q->rear>=q->size){
        fprintf(stderr,
            "[QUEUE ERROR] push overflow (rear=%d size=%d)\n",
            q->rear,q->size);
        _Exit(-1);
    }
    q->buf[q->rear++]=x;
}
static inline int popQ(queue *q){
    if(q->front>=q->rear){
        fprintf(stderr,
            "[QUEUE ERROR] pop underflow (front=%d rear=%d)\n",
            q->front,q->rear);
        _Exit(-1);
    }
    return q->buf[q->front++];
}
static inline int frontQ(const queue *q){
    if(q->front>=q->rear){
        fprintf(stderr,
            "[QUEUE ERROR] front on empty queue (front=%d rear=%d)\n",
            q->front,q->rear);
        _Exit(-1);
    }
    return q->buf[q->front];
}
#define byte unsigned char
#define swapByte(a,b){register byte t=(a);a=(b);b=t;}
const byte __ord[54]={0,1,2,3,4,5,6,7,8,36,37,38,9,10,11,18,19,20,27,28,29,39,40,41,12,13,14,21,22,23,30,31,32,42,43,44,15,16,17,24,25,26,33,34,35,45,46,47,48,49,50,51,52,53};
typedef struct{byte arr[54];}stCube;
static inline void __cw_turn(stCube*cube,const byte*pos){
    for(int i=0;i<20;i+=4){
        register byte t=cube->arr[pos[i]];
        cube->arr[pos[i]]=cube->arr[pos[i+3]];
        cube->arr[pos[i+3]]=cube->arr[pos[i+2]];
        cube->arr[pos[i+2]]=cube->arr[pos[i+1]];
        cube->arr[pos[i+1]]=t;
    }
};
static inline void __ccw_turn(stCube*cube,const byte*pos){
    for(int i=0;i<20;i+=4){
        register byte t=cube->arr[pos[i]];
        cube->arr[pos[i]]=cube->arr[pos[i+1]];
        cube->arr[pos[i+1]]=cube->arr[pos[i+2]];
        cube->arr[pos[i+2]]=cube->arr[pos[i+3]];
        cube->arr[pos[i+3]]=t;
    }
}
static inline void __half_turn(stCube*cube,const byte*pos){
    for(int i=0;i<20;i+=4){
        swapByte(cube->arr[pos[i]],cube->arr[pos[i+2]]);
        swapByte(cube->arr[pos[i+1]],cube->arr[pos[i+3]]);
    }
}
static inline void turnCube(stCube*cube,byte x){
    byte q=x/3,r=x%3;
    const byte*il;
    static const byte u0[20]={0,2,8,6,3,1,5,7,9,36,27,18,10,37,28,19,11,38,29,20};
    static const byte u1[20]={9,11,17,15,12,10,14,16,6,18,47,44,7,21,46,41,8,24,45,38};
    static const byte u2[20]={18,20,26,24,21,19,23,25,2,33,47,11,5,30,50,14,8,27,53,17};
    static const byte u3[20]={27,29,35,33,30,28,32,34,0,42,53,20,1,39,52,23,2,36,51,26};
    static const byte u4[20]={36,38,44,42,39,37,41,43,0,9,45,35,3,12,48,32,6,15,51,29};
    static const byte u5[20]={45,47,53,51,48,46,50,52,42,15,24,33,43,16,25,34,44,17,26,35};
    switch(q){
        case 0:il=u0;break;
        case 1:il=u1;break;
        case 2:il=u2;break;
        case 3:il=u3;break;
        case 4:il=u4;break;
        case 5:il=u5;break;
        default:return;
    }
    switch(r){
        case 0:__cw_turn(cube,il);break;
        case 1:__half_turn(cube,il);break;
        case 2:__ccw_turn(cube,il);break;
    }
}
void readCube(stCube*cube){
    for(int i=0,c;i<54;i++){
        scanf("%d",&c);
        cube->arr[__ord[i]]=c;
    }
}
static inline void printCube(const stCube*cube){
    static char wbuf[144];
    memset(wbuf,' ',sizeof(wbuf));
    byte i=0,p=6;
    for(int a=0;a<54;a++){
        wbuf[p++]=cube->arr[__ord[a]];
        ++i;p++;
        if(i==3||i==6||i==45||i==48||i==51)wbuf[p++]='\n',p+=6;
        if(i==9||i==21||i==33)wbuf[p++]='\n';
    }
#ifdef __linux__
    write(1,wbuf,144);
#else
    fwrite(wbuf,1,144,stdout);
#endif
}

static byte next_permutation_int(int*arr,int n){
    if(n<=1)return 0;
    int i=n-1,i1,i2,t;
    while(1){
        i1=i;
        if(arr[--i]<arr[i1]){
            i2=n;
            while(arr[i]>=arr[--i2]);
            t=arr[i];
            arr[i]=arr[i2];
            arr[i2]=t;
            for(int l=i1,r=n-1;l<r;++l,--r){
                t=arr[l];
                arr[l]=arr[r];
                arr[r]=t;
            }//reverse
            return 1;
        }
        if(i==0){
            for(int l=0,r=n-1;l<r;++l,--r){
                int t=arr[l];
                arr[l]=arr[r];
                arr[r]=t;
            }// reverse
            return 0;
        }
    }
}

unsigned short p1_back[2048];
const byte p1_edge[11][2]={{1,28},{37,3},{19,5},{7,10},{41,12},{21,14},{23,30},{39,32},{46,16},{43,48},{25,50}};
void p1_precalc(){
    queue*q=initQ(2048);
    pushQ(q,0);
    p1_back[0]=-1;
    while(!emptyQ(q)){
        int f=popQ(q);
        stCube cube;
        memset(&cube,0,sizeof(cube));
        int sum=0;
        for(int j=0;j<11;++j){
            byte a=p1_edge[10-j][0],b=p1_edge[10-j][1];
            int v=(f>>j)&1;
            cube.arr[v?a:b]=1;
            sum^=v;
        }
        cube.arr[sum?34:52]=1;
        for(int i=0;i<6;++i){
            for(int j=0;j<3;++j){
                turnCube(&cube,i*3);
                int nv=0;
                for(int i=0;i<11;i++){
                    nv=(nv<<1)|!cube.arr[p1_edge[i][0]];
                }
                if(!p1_back[nv]){
                    p1_back[nv]=((i*3+3-j)<<11)|f;
                    pushQ(q,nv);
                }
            }
            turnCube(&cube,i*3);
        }
    }
    delQ(q);
}
int p1_solve(const stCube*cube,int*res){
    int r=0;
    for(int i=0;i<11;i++){
        byte a=p1_edge[i][0],b=p1_edge[i][1];
        int color=(1<<(cube->arr[b]))|(1<<(cube->arr[a]));
        r<<=1;
        if((cube->arr[b]==3||cube->arr[b]==5)
            ||(!(color&40)&&(cube->arr[b]==1||cube->arr[b]==6)))
            r|=1;
    }
    int cnt=0;
    while(r){
        int val=p1_back[r];
        res[cnt++]=(val>>11)-1;
        r=val&2047;
    }
    return cnt;
}
const byte p2_op[14]={1,3,4,5,6,7,8,9,10,11,12,13,14,16};
const byte p2_corner[7][3]={{18,8,11},{20,27,2},{24,17,47},{26,53,33},{36,0,29},{38,9,6},{42,35,51}};
const byte p2_edge[12][2]={{37,3},{19,5},{41,12},{21,14},{23,30},{39,32},{43,48},{25,50},{1,28},{7,10},{46,16},{52,34}};
unsigned short p2_cs[16384],p2_cl[2187],p2_oTble[2187][14],p2_es[4096],p2_el[495],p2_eTble[495][14];
unsigned int p2_back[1119744];
void p2_precalc_c(){
    {
        int c=0;
        for(int i=0;i<2187;){
            int f=(c&(c>>1))&0x55555555;
            if((f&=-f))c+=f;
            else{
                p2_cs[c]=i;
                p2_cl[i]=c;
                ++c,++i;
            }
        }
    }
    for(int i=0;i<2187;++i){
        int f=p2_cl[i];
        stCube nc;
        memset(&nc,0,sizeof(nc));
        int sum=0;
        for(int j=0;j<7;++j){
            int stat=(f>>(j<<1))&3;
            byte a=p2_corner[6-j][0],b=p2_corner[6-j][1],c=p2_corner[6-j][2];
            nc.arr[stat==0?a:stat==1?b:c]=1;
            sum+=stat;
        }
        sum%=3;
        nc.arr[sum==0?44:sum==1?15:45]=1;
        if(!p2_oTble[i][0]){
            turnCube(&nc,1);
            int r=0;
            for(int i=0;i<7;i++){
                byte a=p2_corner[i][0],b=p2_corner[i][1];
                r=(r<<2)|(nc.arr[a]?0:nc.arr[b]?1:2);
            }
            int f=p2_cs[r];
            p2_oTble[i][0]=f;
            p2_oTble[f][0]=i;
            turnCube(&nc,1);
        }
        for(int j=1;j<5;++j){
            for(int k=0;k<3;++k){
                turnCube(&nc,j*3);
                int up=j*3+k-2;
                if(!p2_oTble[i][up]){
                    int r=0;
                    for(int i=0;i<7;i++){
                        byte a=p2_corner[i][0],b=p2_corner[i][1];
                        r=(r<<2)|(nc.arr[a]?0:nc.arr[b]?1:2);
                    }
                    int f=p2_cs[r];
                    p2_oTble[i][up]=f;
                    p2_oTble[f][j*3-k]=i;
                }
            }
            turnCube(&nc,j*3);
        }
        if(!p2_oTble[i][13]){
            turnCube(&nc,16);
            int r=0;
            for(int i=0;i<7;i++){
                byte a=p2_corner[i][0],b=p2_corner[i][1];
                r=(r<<2)|(nc.arr[a]?0:nc.arr[b]?1:2);
            }
            int f=p2_cs[r];
            p2_oTble[i][13]=f;
            p2_oTble[f][13]=i;
            turnCube(&nc,16);
        }
    }
}
void p2_precalc_e(){
    byte arr[4]={0,1,2,3};
    for(int i=0;i<495;++i){
        int v=(1<<arr[0])|(1<<arr[1])|(1<<arr[2])|(1<<arr[3]);
        p2_es[v]=i;
        p2_el[i]=v;
        if(arr[3]==11){
            if(arr[2]==10){
                if(arr[1]==9){
                    ++arr[0];
                    arr[1]=arr[0]+1;
                }else++arr[1];
                arr[2]=arr[1]+1;
            }else++arr[2];
            arr[3]=arr[2]+1;
        }else++arr[3];
    }
    for(int i=0;i<495;++i){
        int f=p2_el[i];
        stCube nc;
        memset(&nc,0,sizeof(nc));
        for(int j=0;j<12;++j){
            byte idx=p2_edge[11-j][0];
            nc.arr[idx]=(f>>j)&1;
        }
        if(!p2_eTble[i][0]){
            turnCube(&nc,1);
            int r=0;
            for(int i=0;i<12;i++){
                r=(r<<1)|(!!nc.arr[p2_edge[i][0]]);
            }
            int f=p2_es[r];
            p2_eTble[i][0]=f;
            p2_eTble[f][0]=i;
            turnCube(&nc,1);
        }
        for(int j=1;j<5;++j){
            for(int k=0;k<3;++k){
                turnCube(&nc,j*3);
                int up=j*3+k-2;
                if(!p2_eTble[i][up]){
                    int r=0;
                    for(int i=0;i<12;i++){
                        r=(r<<1)|(!!nc.arr[p2_edge[i][0]]);
                    }
                    int f=p2_es[r];
                    p2_eTble[i][up]=f;
                    p2_eTble[f][j*3-k]=i;
                }
            }
            turnCube(&nc,j*3);
        }
        if(!p2_eTble[i][13]){
            turnCube(&nc,16);
            int r=0;
            for(int i=0;i<12;i++){
                r=(r<<1)|(!!nc.arr[p2_edge[i][0]]);
            }
            int f=p2_es[r];
            p2_eTble[i][13]=f;
            p2_eTble[f][13]=i;
            turnCube(&nc,16);
        }
    }
}
void p2_precalc(){ // TODO : inline up 2 func
    p2_precalc_c();
    p2_precalc_e();
    queue*q=initQ(1119744);
    pushQ(q,0);
    p2_back[0]=-1;
    const int op_sz=14;
    while(!emptyQ(q)){
        const int f=popQ(q);
        int c=f>>9,e=f&511;//c=(f>>9)&4095
        for(int i=0;i<op_sz;++i){
            const int op=p2_op[i];
            int nc=p2_oTble[c][i],ne=p2_eTble[e][i];
            int v=(nc<<9)|ne;
            if(!p2_back[v]){
                int qq=op/3,r=op%3;
                p2_back[v]=((qq*3+2-r)<<21)|f;
                pushQ(q,v);
            }
        }
    }
    delQ(q);
}
int p2_solve(stCube*cube,int*res){
    int cr=0;
    for(int i=0;i<7;i++){
        if((1<<cube->arr[p2_corner[i][0]])&40)cr<<=2;
        else if((1<<cube->arr[p2_corner[i][1]])&40)cr=(cr<<2)|1;
        else if((1<<cube->arr[p2_corner[i][2]])&40)cr=(cr<<2)|2;
    }
    cr=p2_cs[cr];
    int eg=0;
    for(int i=0;i<12;i++){
        int v=(1<<cube->arr[p2_edge[i][0]])|(1<<cube->arr[p2_edge[i][1]]);
        if(v&40)eg<<=1;
        else eg=(eg<<1)|1;
    }
    eg=p2_es[eg];
    int r=(cr<<9)|eg,cnt=0;
    while(r){
        int val=p2_back[r];
        res[cnt++]=val>>21;
        r=val&2097151;
    }
    return cnt;
}
const byte p3_op[10]={1,4,6,7,8,10,12,13,14,16};
const byte p34_corner[8][3]={{18,8,11},{26,53,33},{36,0,29},{44,45,15},{20,27,2},{24,17,47},{38,9,6},{42,35,51}};
const byte p3_edge[8][2]={{41,12},{21,14},{23,30},{39,32},{37,3},{19,5},{43,48},{25,50}};
unsigned short p3_ps[256],p3_pl[70],p3_prs[524288],p3_cTble[40320][10],p3_eTble[70][10];
unsigned int p3_prl[40320],p3_back[2822400];
void p3_precalc_pi(){
    if(p3_pl[0]==15)return;
    int arr[4]={0,1,2,3};
    for(int i=0;i<70;++i){
        int v=(1<<arr[0])|(1<<arr[1])|(1<<arr[2])|(1<<arr[3]);
        p3_ps[v]=i;
        p3_pl[i]=v;
        if(arr[3]==7){
            if(arr[2]==6){
                if(arr[1]==5){
                    ++arr[0];
                    arr[1]=arr[0]+1;
                }else++arr[1];
                arr[2]=arr[1]+1;
            }else++arr[2];
            arr[3]=arr[2]+1;
        }else++arr[3];
    }
}
void p3_precalc_pr(){
    p3_precalc_pi();
    int permutation[8]={0,1,2,3,4,5,6,7};
    int c=0;
    do{
        int perm_val=0;
        for(int i=0;i<6;++i){
            perm_val=(perm_val<<3)|permutation[i];
        }
        p3_prs[(perm_val<<1)|(permutation[6]>permutation[7])]=c;
        p3_prl[c]=(perm_val<<6)|(permutation[6]<<3)|permutation[7]; // RTE
        ++c;
    }while(next_permutation_int(permutation,8));
}
void p3_preclac_c(){
    p3_precalc_pr();
    for(int i=0;i<40320;++i){
        int f=p3_prl[i];
        stCube nc;
        memset(&nc,0,sizeof(nc));
        for(int j=0;j<8;++j){
            nc.arr[p34_corner[7-j][0]]=(f>>(j*3))&7;
        }
        int oidx=0;
        for(int j=0;j<6;++j){
            if((1<<j)&20){
                for(int k=0;k<3;++k){
                    turnCube(&nc,j*3);
                    if(!p3_cTble[i][oidx]){
                        int r=0;
                        for(int i=0;i<6;++i){
                            r=(r<<3)|nc.arr[p34_corner[i][0]];
                        }
                        r=(r<<1)|(nc.arr[p34_corner[6][0]]>nc.arr[p34_corner[7][0]]);
                        int f=p3_prs[r];
                        p3_cTble[i][oidx]=f;
                        p3_cTble[f][oidx&1?oidx:((oidx-2)^2)+2]=i;
                    }
                    ++oidx;
                }
                turnCube(&nc,j*3);
            }else{
                if(!p3_cTble[i][oidx]){
                    turnCube(&nc,j*3+1);
                    int r=0;
                    for(int i=0;i<6;++i){
                        r=(r<<3)|nc.arr[p34_corner[i][0]];
                    }
                    r=(r<<1)|(nc.arr[p34_corner[6][0]]>nc.arr[p34_corner[7][0]]);
                    int f=p3_prs[r];
                    p3_cTble[i][oidx]=f;
                    p3_cTble[f][oidx]=i;
                    turnCube(&nc,j*3+1);
                }
                ++oidx;
            }
        }
    }
}
void p3_precalc_e(){
    p3_precalc_pi();
    for(int i=0;i<70;++i){
        int f=p3_pl[i];
        stCube nc;
        memset(&nc,0,sizeof(nc));
        for(int j=0;j<8;++j){
            nc.arr[p3_edge[7-j][0]]=(f>>j)&1;
        }
        int oidx=0;
        for(int j=0;j<6;++j){
            if((1<<j)&20){
                for(int k=0;k<3;++k){
                    turnCube(&nc,j*3);
                    if(!p3_eTble[i][oidx]){
                        int r=0;
                        for(int i=0;i<8;i++){
                            r=(r<<1)|(!!nc.arr[p3_edge[i][0]]);
                        }
                        int f=p3_ps[r];
                        p3_eTble[i][oidx]=f;
                        p3_eTble[f][oidx&1?oidx:((oidx-2)^2)+2]=i;
                    }
                    ++oidx;
                }
                turnCube(&nc,j*3);
            }else{
                if(!p3_eTble[i][oidx]){
                    turnCube(&nc,j*3+1);
                    int r=0;
                    for(int i=0;i<8;i++){
                        r=(r<<1)|(!!nc.arr[p3_edge[i][0]]);
                    }
                    int f=p3_ps[r];
                    p3_eTble[i][oidx]=f;
                    p3_eTble[f][oidx]=i;
                    turnCube(&nc,j*3+1);
                }
                ++oidx;
            }
        }
    }
}
void p3_precalc(){
    p3_preclac_c();
    debug("-- p3 corner calculated\n");
    p3_precalc_e();
    debug("-- p3 edge calculated\n");
    queue*q=initQ(2822400);
    {
        int inch[40320];
        memset(inch,0,sizeof(inch));
        queue*q0=initQ(40320);
        pushQ(q,0);
        pushQ(q0,0);
        p3_back[0]=-1;
        inch[0]=1;
        while(!emptyQ(q0)){
            int f=frontQ(q0);
            popQ(q0);
            byte perm[8];
            {
                int x=p3_prl[f];
                for(int i=7;i>=0;--i){
                    perm[i]=x&7;
                    x>>=3;
                }
            }
            static const short e[4]={101,686,1271,1253};
            for(int i=0;i<4;i++){
                const byte f=(e[i]>>6),s=e[i]&63;
                swapByte(perm[f>>3],perm[f&7]);
                swapByte(perm[s>>3],perm[s&7]);
                int v=0;
                for(int i=0;i<6;++i){
                    v=(v<<3)|perm[i];
                }
                int nf=p3_ps[(v<<1)|(perm[6]>perm[7])];
                if(!inch[nf]){
                    pushQ(q0,nf);
                    pushQ(q,nf*70);
                    p3_back[nf*70]=-1;
                    inch[nf]=1;
                }
                swapByte(perm[f>>3],perm[f&7]);
                swapByte(perm[s>>3],perm[s&7]);
            }
        }
        delQ(q0);
    }
    const int op_sz=10;
    while(!emptyQ(q)){
        const int f=frontQ(q);
        popQ(q);
        int c=f/70,e=f%70;
        for(int i=0;i<op_sz;++i){
            const int op=p3_op[i];
            int nc=p3_cTble[c][i],ne=p3_eTble[e][i];
            int v=nc*70+ne;
            if(!p3_back[v]){
                int qq=op/3,r=op%3;
                p3_back[v]=((qq*3+2-r)<<22)|f;
                pushQ(q,v);
            }
        }
    }
    delQ(q);
}
int p3_solve(stCube*cube,int*res){
    int c=0;
    int co[8];
    for(int i=0;i<8;i++){
        const byte a=p34_corner[i][0],b=p34_corner[i][1],c=p34_corner[i][2];
        int ccol=(1<<cube->arr[a])|(1<<cube->arr[b])|(1<<cube->arr[c]);
        int r=0;
        if(ccol&64)r|=1;
        if(ccol&32)r|=2;
        if(((ccol>>4)&1)!=(__builtin_popcount(r)&1))r|=4;
        co[i]=r;
    }
    int p=0;
    for(int i=0;i<6;++i){
        p=(p<<3)|co[i];
    }
    c=p3_prs[(p<<1)|(co[6]>co[7])];
    int e=0;
    for(int i=0;i<8;i++){
        int v=(1<<cube->arr[p3_edge[i][0]])|(1<<cube->arr[p3_edge[i][1]]);
        e<<=1;
        if(!(v&20))e|=1;
    }
    e=p3_ps[e];
    int r=c*70+e,cnt=0;
    while(~p3_back[r]){
        int v=p3_back[r];
        res[cnt++]=v>>22;
        r=v&4194303;
    }
    return cnt;
}

byte p4_ps[256],p4_pl[24],p4_cTble[96][6],p4_color[64];
const byte p4_edge[12][2]={{1,28},{7,10},{46,16},{52,34},{21,14},{23,30},{39,32},{41,12},{19,5},{25,50},{43,48},{37,3}};
unsigned short p4_cl[96],p4_eTble[6912][6];
unsigned int p4_back[663552];
void p4_precalc_pr(){
    if(p4_pl[0]==27){
        return;
    }
    int perm[4]={0,1,2,3},c=0;
    do{
        int r=0;
        for(int i=0;i<4;i++){
            r=(r<<2)|perm[i];
        }
        p4_ps[r]=c;
        p4_pl[c]=r;
        ++c;
    }while(next_permutation_int(perm,4));
}
void p4_precalc_c(){
    p4_precalc_pr();
    queue*q=initQ(96);
    pushQ(q,0);
    p4_cl[0]=6939;
    while(!emptyQ(q)){
        int f=frontQ(q);
        popQ(q);
        stCube nc;
        memset(&nc,0,sizeof(nc));
        for(int i=0;i<8;++i){
            nc.arr[p34_corner[7-i][0]]=(p4_cl[f]>>(i<<1))&3;
        }
        for(int i=0;i<6;++i){
            if(!p4_cTble[f][i]){
                turnCube(&nc,i*3+1);
                int lp=0;
                for(int i=0;i<8;++i){
                    lp=(lp<<2)|nc.arr[p34_corner[i][0]];
                }
                int sp=(p4_ps[lp>>8]<<2)|((lp>>6)&3);
                if(!p4_cl[sp]){
                    p4_cl[sp]=lp;
                    pushQ(q,sp);
                }
                p4_cTble[f][i]=sp;
                p4_cTble[sp][i]=f;
                turnCube(&nc,i*3+1);
            }
        }
    }
    delQ(q);
}
void p4_precalc_e(){
    p4_precalc_pr();
    unsigned int el[6912];
    memset(el,0,sizeof(el));
    queue*q=initQ(6912);
    pushQ(q,0);
    el[0]=1776411;
    while(!emptyQ(q)){
        int f=frontQ(q);
        popQ(q);
        stCube nc;
        memset(&nc,0,sizeof(nc));
        for(int i=0;i<12;++i){
            nc.arr[p4_edge[11-i][0]]=(el[f]>>(i<<1))&3;
        }
        for(int i=0;i<6;++i){
            if(!p4_eTble[f][i]){
                turnCube(&nc,i*3+1);
                int lp=0,llp=0;
                for(int j=0;j<3;++j){
                    int r=0;
                    for(int k=0;k<4;++k){
                        r=(r<<2)|nc.arr[p4_edge[(j<<2)|k][0]];
                    }
                    llp=(llp<<8)|r;
                    lp=lp*24+p4_ps[r];
                }
                const int nf=lp>>1;
                if(!el[nf]){
                    el[nf]=llp;
                    pushQ(q,nf);
                }
                p4_eTble[f][i]=nf;
                p4_eTble[nf][i]=f;
                turnCube(&nc,i*3+1);
            }
        }
    }
    delQ(q);
}
void p4_precalc(){
    p4_precalc_c();
    p4_precalc_e();
    {
        int i=0;
        for(;i<8;++i){
            int v=(1<<(p34_corner[i][0]/9))|(1<<(p34_corner[i][1]/9))|(1<<(p34_corner[i][2]/9));
            p4_color[v]=i&3;
        }
        for(int j=0;j<12;++i,++j){
            int v=(1<<(p4_edge[j][0]/9))|(1<<(p4_edge[j][1]/9));
            p4_color[v]=i&3;
        }
    }
    queue*q=initQ(663552);
    p4_back[0]=-1;
    pushQ(q,0);
    while(!emptyQ(q)){
        int f=frontQ(q);
        popQ(q);
        int e=f/96,c=f%96;
        for(int i=0;i<6;++i){
            int ne=p4_eTble[e][i],nc=p4_cTble[c][i];
            int nf=ne*96+nc;
            if(!p4_back[nf]){
                p4_back[nf]=((i+1)<<20)|f;
                pushQ(q,nf);
            }
        }
    }
    delQ(q);
}
int p4_solve(stCube*cube,int*res){
    int c=0;
    for(int i=0;i<8;++i){
        c=(c<<2)|p4_color[((1<<cube->arr[p34_corner[i][0]]))|((1<<cube->arr[p34_corner[i][1]]))|((1<<cube->arr[p34_corner[i][2]]))];
    }
    c=(p4_ps[c>>8]<<2)|((c>>6)&3);
    int e=0;
    for(int i=0;i<3;++i){
        int r=0;
        for(int j=0;j<4;++j){
            r=(r<<2)|p4_color[((1<<cube->arr[p4_edge[(i<<2)|j][0]])|(1<<cube->arr[p4_edge[(i<<2)|j][1]]))>>1];
        }
        e=(e*24)+p4_ps[r];
    }
    e>>=1;
    int r=e*96+c,cnt=0;
    while(~p4_back[r]){
        int val=p4_back[r];
        res[cnt++]=(val>>20)*3-2;
        r=val&1048575;
    }
    return cnt;
}
int main(){
    p1_precalc();
    debug("p1 precalculated\n");
    p2_precalc();
    debug("p2 precalculated\n");
    p3_precalc();
    debug("p3 precalculated\n");
    p4_precalc();
    debug("p4 precalculated\n");
    stCube cube;
    int code[64];
    int len=0,n,T;
    scanf("%d",&T);
    while(T--){
        readCube(&cube);
        n=p1_solve(&cube,code+len);
        for(int i=0;i<n;i++)
            turnCube(&cube,code[len+i]);
        len+=n;
        n=p2_solve(&cube,code+len);
        for(int i=0;i<n;i++)
            turnCube(&cube,code[len+i]);
        len+=n;
        n=p3_solve(&cube,code+len);
        for(int i=0;i<n;i++)
            turnCube(&cube,code[len+i]);
        len+=n;
        n=p4_solve(&cube,code+len);
        for(int i=0;i<n;i++)
            turnCube(&cube,code[len+i]);
        len+=n;
        for(int i=0;i<len;i++){
            int op=code[i];
            if(op==-1)printf("/ ");
            else{
                putchar("UFRBLD"[op/3]);
                int r=op%3;
                if(r)putchar(" 2'"[r]);
                putchar(' ');
            }
        }
        putchar('\n');
    }
    return 0;
}