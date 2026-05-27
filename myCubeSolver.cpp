#include <cstdint>
#include <cstdio>
#include <queue>
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx,avx2,fma")
using namespace std;
class Cube {
#define C(f, r, c) cube[(f) * 9 + (r) * 3 + (c)]
   private:
    const char* color = "wyrogb";
    static constexpr int ord[4] = { 4, 2, 5, 3 };
    void rotate_face(int f) {
        char t;
        t = C(f, 0, 0);
        C(f, 0, 0) = C(f, 2, 0);
        C(f, 2, 0) = C(f, 2, 2);
        C(f, 2, 2) = C(f, 0, 2);
        C(f, 0, 2) = t;
        t = C(f, 0, 1);
        C(f, 0, 1) = C(f, 1, 0);
        C(f, 1, 0) = C(f, 2, 1);
        C(f, 2, 1) = C(f, 1, 2);
        C(f, 1, 2) = t;
    }

   public:
    alignas(32) uint8_t cube[54];
    void read_cube() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) scanf(" %c", &C(0, i, j));
        for (int i = 0; i < 3; i++) {
            for (int k = 0; k < 4; k++) {
                int f = ord[k];
                for (int j = 0; j < 3; j++) scanf(" %c", &C(f, i, j));
            }
        }
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) scanf(" %c", &C(1, i, j));
    }
    void pretty_print() {
        for (int i = 0; i < 3; i++) {
            printf("      ");
            for (int j = 0; j < 3; j++) printf("%c ", C(0, i, j));
            puts("");
        }
        for (int i = 0; i < 3; i++) {
            for (int k = 0; k < 4; k++) {
                int f = ord[k];
                for (int j = 0; j < 3; j++) printf("%c ", C(f, i, j));
            }
            puts("");
        }
        for (int i = 0; i < 3; i++) {
            printf("      ");
            for (int j = 0; j < 3; j++) printf("%c ", C(1, i, j));
            puts("");
        }
        puts("");
    }
    void turn(int s, int d) {
        static const int ri[2][3] = { { 0, 1, 2 }, { 2, 1, 0 } };
        const int *p, *q;
        int cnt = (d == 2) ? 2 : (d == -1 ? 3 : 1);
        while (cnt--) {
            rotate_face((34644 >> (s * 3)) & 7);
            switch (s) {
                case 4:
                case 5: {
                    int a = (s - 4) << 1;
                    const int ts[2][4] = { { 2, 5, 3, 4 }, { 2, 4, 3, 5 } };
                    p = ts[s - 4];
                    for (int i = 0, t; i < 3; i++) {
                        t = C(p[0], a, i);
                        for (int j = 0; j < 3; j++) C(p[j], a, i) = C(p[j + 1], a, i);
                        C(p[3], a, i) = t;
                    }
                } break;
                case 1:
                case 3: {
                    int c[3], t = s >> 1;
                    static constexpr int t1[2] = { 0, 2 }, t2[2] = { 4, 5 };
                    p = ri[t];
                    q = ri[t ^ 1];
                    for (int i = 0; i < 3; i++) {
                        c[i] = C(0, t1[t ^ 1], i);
                        C(0, t1[t ^ 1], i) = C(t2[t], q[i], 2);
                    }
                    for (int i = 0; i < 3; i++) {
                        C(t2[t], i, 2) = C(1, t1[t], p[i]);
                        C(1, t1[t], p[i]) = C(t2[t ^ 1], 2 - i, 0);
                    }
                    for (int i = 0; i < 3; i++)
                        C(t2[t ^ 1], p[i], 0) = c[i];
                } break;
                case 0:
                case 2: {
                    int c[3], t = s >> 1;
                    static constexpr int t3[] = { 2, 3 };
                    p = ri[t];
                    q = ri[t ^ 1];
                    for (int i = 0; i < 3; i++) {
                        c[i] = C(0, i, s);
                        C(0, i, s) = C(t3[t ^ 1], q[i], 2);
                    }
                    for (int i = 0; i < 3; i++) C(t3[t ^ 1], i, 2) = C(1, q[i], s);
                    for (int i = 0; i < 3; i++) {
                        C(1, i, s) = C(t3[t], p[i], 0);
                        C(t3[t], p[i], 0) = c[i];
                    }
                } break;
            }
        }
    }
};
namespace phase_1 {
uint8_t backprop[2048];
const int eleven_edge[11][2] = {
    { 1, 28 },
    { 37, 3 },
    { 19, 5 },
    { 7, 10 },
    { 41, 12 },
    { 21, 14 },
    { 23, 30 },
    { 39, 32 },
    { 46, 16 },
    { 43, 48 },
    { 25, 50 }
};
int cube_value(const Cube& c) {
    int r = 0;
    for (const auto& [idx, _] : eleven_edge) {
        r = (r << 1) | !cube.cube[idx];
    }
    return r;
}
void precalc() {
    std::queue<int> q;
    q.push(0);
    backprop[0] = -1;
    while (!q.empty()) {
        int f = q.front();
        q.pop();
        Cube c;
        int sum_edge = 0;
        for (int i = 0; i < 11; i++) {
            const auto& [now, flipped] = eleven_edge[10 - j];
            int v = (f >> j) & 1;
            c.cube[v ? flipped : now] = 1;
            sum_edge ^= v;
        }
        c.cube[sum_edge ? 34 : 52] = 1;
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 3; ++j) {
                c.turn(i, 1);
                int nv = cube_value(c);
                if (!backprop[nv]) {
                    backprop[nv] = ((i * 3 + 3 - j) << 11) | f;
                    q.push(nv);
                }
            }
            c.turn(i, 1);
        }
    }
    void solve(const Cube& cube, std::vector<int> result) {
        int r = 0;
    }
}