#include <stdio.h>

#define MAX 105
#define INF 4000000000000000000LL
#define NEG_INF -4000000000000000000LL
#define NO_EDGE 999999999LL

long long a[MAX][MAX];
long long d[MAX][MAX];
int next[MAX][MAX];
int n, s, t;

int docFile() {
    FILE *fin;
    int i, j;

    fin = fopen("input.txt", "r");
    if (fin == NULL) {
        printf("Khong mo duoc file input.txt\n");
        return 0;
    }

    fscanf(fin, "%d", &n);
    fscanf(fin, "%d%d", &s, &t);

    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            fscanf(fin, "%lld", &a[i][j]);
        }
    }

    fclose(fin);
    return 1;
}

void khoiTao() {
    int i, j;

    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            if (i == j) {
                if (a[i][j] == NO_EDGE) d[i][j] = 0;
                else d[i][j] = a[i][j];
                next[i][j] = j;
            }
            else if (a[i][j] == NO_EDGE) {
                d[i][j] = INF;
                next[i][j] = -1;
            }
            else {
                d[i][j] = a[i][j];
                next[i][j] = j;
            }
        }
    }
}

void floyd() {
    int i, j, k;

    for (k = 1; k <= n; k++) {
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= n; j++) {
                if (d[i][k] != INF && d[k][j] != INF &&
                    d[i][k] != NEG_INF && d[k][j] != NEG_INF) {
                    if (d[i][j] > d[i][k] + d[k][j]) {
                        d[i][j] = d[i][k] + d[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }
    for (k = 1; k <= n; k++) {
        if (d[k][k] < 0) {
            for (i = 1; i <= n; i++) {
                for (j = 1; j <= n; j++) {
                    if (d[i][k] != INF && d[k][j] != INF) {
                        d[i][j] = NEG_INF;
                        next[i][j] = -1;
                    }
                }
            }
        }
    }
}

void ghiDuongDi(FILE *fout) {
    int u;

    if (d[s][t] == INF) {
        fprintf(fout, "Khong co duong di\n");
        return;
    }

    if (d[s][t] == NEG_INF) {
        fprintf(fout, "Khong the truy vet duong di vi bi anh huong boi chu trinh am\n");
        return;
    }

    if (next[s][t] == -1) {
        fprintf(fout, "Khong co duong di\n");
        return;
    }

    u = s;
    fprintf(fout, "%d", u);

    while (u != t) {
        u = next[u][t];
        fprintf(fout, " -> %d", u);
    }

    fprintf(fout, "\n");
}

void ghiKetQua() {
    FILE *fout;

    fout = fopen("output.txt", "w");
    if (fout == NULL) {
        printf("Khong tao duoc file output.txt\n");
        return;
    }

    if (d[s][t] == INF) {
        fprintf(fout, "Khong co duong di tu %d den %d\n", s, t);
    }
    else if (d[s][t] == NEG_INF) {
        fprintf(fout, "Duong di tu %d den %d bi anh huong boi chu trinh am, khong ton tai duong di ngan nhat huu han\n", s, t);
    }
    else {
        fprintf(fout, "Khoang cach ngan nhat tu %d den %d la: %lld\n", s, t, d[s][t]);
        fprintf(fout, "Duong di: ");
        ghiDuongDi(fout);
    }

    fclose(fout);
}

int main() {
    if (!docFile()) return 1;
    khoiTao();
    floyd();
    ghiKetQua();

    printf("Da ghi ket qua vao file output.txt\n");
    return 0;
}
