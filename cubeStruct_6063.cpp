#include <bits/stdc++.h>
#pragma GCC optimize("unroll-loops")
using namespace std;
class Cube {
   public:
    char cube[6][3][3];
    void read_cube() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) scanf(" %c", &cube[0][i][j]);
        int ord[4] = { 4, 2, 5, 3 };
        for (int i = 0; i < 3; i++) {
            for (int k = 0; k < 4; k++) {
                int f = ord[k];
                for (int j = 0; j < 3; j++) scanf(" %c", &cube[f][i][j]);
            }
        }
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) scanf(" %c", &cube[1][i][j]);
    }
    void pretty_print() {
        for (int i = 0; i < 3; i++) {
            printf("      ");
            for (int j = 0; j < 3; j++) printf("%c ", cube[0][i][j]);
            puts("");
        }
        int ord[4] = { 4, 2, 5, 3 };
        for (int i = 0; i < 3; i++) {
            for (int k = 0; k < 4; k++) {
                int f = ord[k];
                for (int j = 0; j < 3; j++) printf("%c ", cube[f][i][j]);
            }
            puts("");
        }
        for (int i = 0; i < 3; i++) {
            printf("      ");
            for (int j = 0; j < 3; j++) printf("%c ", cube[1][i][j]);
            puts("");
        }
        puts("");
    }

   private:
    void rotate_face(int f) {
        int C[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) C[i][j] = cube[f][i][j];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) cube[f][j][2 - i] = C[i][j];
    }

   public:
    void turn(int s, int d) {
        if (d != 1) {
            for (int i = 0; i < (8 - d) / 3; i++) turn(s, 1);
            return;
        }
        rotate_face((34644 >> (s * 3)) & 7);
        if (s == 4 || s == 5) {
            int tmp[] = { 2, 9 - s, 3, s };
            for (int i = 0; i < 3; i++) {
                int t = cube[tmp[0]][tf][i];
                for (int j = 0; j < 3; j++) cube[tmp[j]][tf][i] = cube[tmp[j + 1]][tf][i];
                cube[tmp[3]][tf][i] = t;
            }
        }
        if (s == 1) {
            int C[3];
            for (int i = 0; i < 3; i++) C[i] = cube[0][2][i];

            for (int i = 0; i < 3; i++) {
                cube[0][2][i] = cube[4][2 - i][2];
                cube[4][i][2] = cube[1][0][i];
                cube[1][0][i] = cube[5][2 - i][0];
                cube[5][i][0] = C[2 - i];
            }
        }

        if (s == 3) {
            int C[3];
            for (int i = 0; i < 3; i++) C[i] = cube[0][0][i];

            for (int i = 0; i < 3; i++) {
                cube[0][0][i] = cube[5][i][2];
                cube[5][i][2] = cube[1][2][2 - i];
                cube[1][2][i] = cube[4][i][0];
                cube[4][2 - i][0] = C[i];
            }
        }
        if (s == 0) {
            int C[3];
            for (int i = 0; i < 3; i++)
                C[i] = cube[0][i][0];
            cube[0][2][0] = cube[3][0][2];
            cube[0][1][0] = cube[3][1][2];
            cube[0][0][0] = cube[3][2][2];
            cube[3][0][2] = cube[1][2][0];
            cube[3][1][2] = cube[1][1][0];
            cube[3][2][2] = cube[1][0][0];
            cube[1][0][0] = cube[2][0][0];
            cube[1][1][0] = cube[2][1][0];
            cube[1][2][0] = cube[2][2][0];
            for (int i = 0; i < 3; i++) cube[2][i][0] = C[i];
        }
        if (s == 2) {
            int C[3];
            for (int i = 0; i < 3; i++)
                C[i] = cube[0][i][2];
            for (int i = 0; i < 3; i++)
                cube[0][i][2] = cube[2][i][2];
            for (int i = 0; i < 3; i++)
                cube[2][i][2] = cube[1][i][2];
            for (int i = 0; i < 3; i++)
                cube[1][i][2] = cube[3][2 - i][0];
            for (int i = 0; i < 3; i++)
                cube[3][i][0] = C[2 - i];
        }
    }
};
int main() {
    int T;
    scanf("%d", &T);
    for (int tc = 1; tc <= T; tc++) {
        Cube C;
        C.read_cube();
        int q;
        scanf("%d", &q);
        while (q--) {
            int s, d;
            scanf("%d%d", &s, &d);
            C.turn(s, d);
        }
        printf("Scenario #%d:\n", tc);
        C.pretty_print();
    }
    return 0;
}