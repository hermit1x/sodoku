#include <iostream>
#include <cstdio>
#include <cstring>

#define maxn (250000)

using namespace std;


// DLX矩阵一共729行、324列

struct sudoku{
    int u[maxn], d[maxn], l[maxn], r[maxn]; // 上 下 左 右
    int row[maxn], col[maxn], h[maxn], s[maxn], ansk[maxn], cnt; // 行 列 行头 列重 答案记录

    sudoku() {
        for (int i = 0; i <= 324; ++i) {
            r[i] = i + 1; l[i] = i - 1;
            u[i] = d[i] = i;
        }
        r[324] = 0; l[0] = 324;
        memset(h, -1, sizeof(h));
        memset(s, 0, sizeof(s));
        cnt = 324 + 1;
    }

    int read() {
        char c = getchar();
        while (!(c == '.' || (c >= '0' && c <= '9'))) c = getchar();
        return c == '.' ? -1 : c - '1';
    }

    void link(int R, int C) {
        R++; C++;
        s[C]++;
        row[cnt] = R; col[cnt] = C;
        u[cnt] = C; d[cnt] = d[C];
        u[d[C]] = cnt; d[C] = cnt;
        if (h[R] == -1) 
            h[R] = r[cnt] = l[cnt] = cnt;
        else {
            r[cnt] = h[R]; l[cnt] = l[h[R]];
            r[l[h[R]]] = cnt; l[h[R]] = cnt;
        }
        cnt++;
    }

    void remove(int C) {
	    r[l[C]] = r[C]; l[r[C]] = l[C];
        for (int i = d[C]; i != C; i = d[i])
            for (int j = r[i]; j != i; j = r[j]) {
                u[d[j]] = u[j];
                d[u[j]] = d[j];
                s[col[j]]--;
            }
    }

    void resume(int C) {
        for (int i = u[C]; i != C; i = u[i])
            for (int j = l[i]; j != i; j = l[j]) {
                u[d[j]] = j; d[u[j]] = j;
                s[col[j]]++;
            }
        r[l[C]] = C; l[r[C]] = C;
    }

    #define torow(x) ((x - 1) / 81)
    #define tocol(x) (((x - 1) / 9) % 9)
    #define tonum(x) (((x - 1) % 9) + 1)

    void print(int deep) {
        int ans[10][10] = {0};
        for (int i = 0; i < deep; ++i) {
            int x = ansk[i];
            ans[torow(x)][tocol(x)] = tonum(x);
        }
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) 
                printf("%d ", ans[i][j]);
            printf("\n");
        }
        printf("\n");
    }

    void dance(int deep) {
        if (r[0] == 0) {
            print(deep);
            return ;
        }
        int c = r[0];
        for (int i = r[0]; i != 0; i = r[i]) 
            if (s[i] < s[c]) c = i;
        remove(c);
        for (int i = d[c]; i != c; i = d[i]) {
            ansk[deep] = row[i];
            for (int j = r[i]; j != i; j = r[j]) remove(col[j]);
            dance(deep + 1);
            for (int j = l[i]; j != i; j = l[j]) resume(col[j]); 
        }
        resume(c);
    }

    void init() {
        int c;
        #define num (9 * i + j)
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j) {
                c = read();
                if (c == -1) {
                    for (int k = 0; k < 9; ++k) {
                        link(num * 9 + k, num);
                        link(num * 9 + k, 81 + i * 9 + k);
                        link(num * 9 + k, 162 + j * 9 + k);
                        link(num * 9 + k, 243 + (i / 3 * 3 + j / 3) * 9 + k);
                    }
                }
                else {
                    int k = c;
                    link(num * 9 + k, num);
                    link(num * 9 + k, 81 + i * 9 + k);
                    link(num * 9 + k, 162 + j * 9 + k);
                    link(num * 9 + k, 243 + (i / 3 * 3 + j / 3) * 9 + k);
                }
            }
        #undef num
    }
    void solve() {
        init();
        dance(0);
    }

}s;

int main() {
    s.solve();
    return 0;
}