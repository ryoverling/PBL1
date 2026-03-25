#include <stdio.h>
#include <stdlib.h>

#define MAXN 100
#define INF 1000000000

int dist[MAXN][MAXN];
int Next[MAXN][MAXN];

void printValue(FILE *out, int x) {
    if (x >= INF / 2) fprintf(out, "INF");
    else fprintf(out, "%d", x);
}

void printMatrix(FILE *out, int n) {
    fprintf(out, "\nMa tran khoang cach hien tai:\n\n");

    fprintf(out, "%8s", "");
    for (int j = 0; j < n; j++) {
        fprintf(out, "%8d", j);
    }
    fprintf(out, "\n");

    for (int i = 0; i < n; i++) {
        fprintf(out, "%8d", i);
        for (int j = 0; j < n; j++) {
            if (dist[i][j] >= INF / 2) fprintf(out, "%8s", "INF");
            else fprintf(out, "%8d", dist[i][j]);
        }
        fprintf(out, "\n");
    }
}

void printPath(FILE *out, int u, int v) {
    if (Next[u][v] == -1) {
        fprintf(out, "Khong ton tai duong di tu %d den %d\n", u, v);
        return;
    }

    fprintf(out, "Duong di ngan nhat tu %d den %d: ", u, v);
    fprintf(out, "%d", u);

    while (u != v) {
        u = Next[u][v];
        fprintf(out, " -> %d", u);
    }
    fprintf(out, "\n");
}

void readInput(const char *filename, int *n, int *start, int *end) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Khong mo duoc file %s\n", filename);
        exit(1);
    }

    fscanf(f, "%d", n);
    fscanf(f, "%d %d", start, end);

    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            int w;
            fscanf(f, "%d", &w);

            if (i == j) {
                dist[i][j] = 0;
                Next[i][j] = j;
            } else if (w == -1) {
                dist[i][j] = INF;
                Next[i][j] = -1;
            } else {
                dist[i][j] = w;
                Next[i][j] = j;
            }
        }
    }

    fclose(f);
}

int hasNegativeCycle(int n) {
    for (int i = 0; i < n; i++) {
        if (dist[i][i] < 0) return 1;
    }
    return 0;
}

void floydWarshall(FILE *out, int n, int start, int end) {
    fprintf(out, "=============================================\n");
    fprintf(out, "GIAI BAI TOAN BANG THUAT TOAN FLOYD-WARSHALL\n");
    fprintf(out, "So dinh       : %d\n", n);
    fprintf(out, "Dinh bat dau  : %d\n", start);
    fprintf(out, "Dinh ket thuc : %d\n", end);
    fprintf(out, "=============================================\n");

    fprintf(out, "\nTrang thai ban dau:\n");
    printMatrix(out, n);

    for (int k = 0; k < n; k++) {
        fprintf(out, "\n====================================================\n");
        fprintf(out, "XET DINH TRUNG GIAN k = %d\n", k);
        fprintf(out, "====================================================\n");

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(out, "\nXet duong di %d -> %d qua %d\n", i, j, k);

                if (dist[i][k] >= INF / 2 || dist[k][j] >= INF / 2) {
                    fprintf(out, "Khong the di qua %d vi ", k);
                    if (dist[i][k] >= INF / 2)
                        fprintf(out, "dist[%d][%d] = INF ", i, k);
                    if (dist[k][j] >= INF / 2)
                        fprintf(out, "dist[%d][%d] = INF", k, j);
                    fprintf(out, "\n=> KHONG CAP NHAT\n");
                    continue;
                }

                int candidate = dist[i][k] + dist[k][j];

                fprintf(out, "So sanh: dist[%d][%d] = ", i, j);
                printValue(out, dist[i][j]);
                fprintf(out, " voi dist[%d][%d] + dist[%d][%d] = %d + %d = %d\n",
                        i, k, k, j, dist[i][k], dist[k][j], candidate);

                if (candidate < dist[i][j]) {
                    int old = dist[i][j];
                    dist[i][j] = candidate;
                    Next[i][j] = Next[i][k];

                    fprintf(out, "=> CAP NHAT\n");
                    fprintf(out, "   dist[%d][%d]: ", i, j);
                    if (old >= INF / 2) fprintf(out, "INF");
                    else fprintf(out, "%d", old);
                    fprintf(out, " -> %d\n", dist[i][j]);

                    fprintf(out, "   Next[%d][%d] = %d\n", i, j, Next[i][j]);
                } else {
                    fprintf(out, "=> KHONG CAP NHAT\n");
                }
            }
        }

        fprintf(out, "\nKet thuc khi xet k = %d\n", k);
        printMatrix(out, n);
    }

    fprintf(out, "\n=============================================\n");
    fprintf(out, "KET LUAN\n");
    fprintf(out, "=============================================\n");

    if (hasNegativeCycle(n)) {
        fprintf(out, "Do thi co chu trinh am.\n");
        fprintf(out, "Ket qua duong di ngan nhat co the khong xac dinh.\n");
        return;
    }

    fprintf(out, "\nMa tran khoang cach ngan nhat cuoi cung:\n");
    printMatrix(out, n);

    fprintf(out, "\nKhoang cach ngan nhat tu %d den %d: ", start, end);
    if (dist[start][end] >= INF / 2) fprintf(out, "INF\n");
    else fprintf(out, "%d\n", dist[start][end]);

    printPath(out, start, end);
}

int main() {
    int n, start, end;

    readInput("input.txt", &n, &start, &end);

    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Khong mo duoc file output.txt\n");
        return 1;
    }

    floydWarshall(out, n, start, end);

    fclose(out);

    printf("Da ghi ket qua vao file output.txt\n");
    return 0;
}