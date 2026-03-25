#include <stdio.h>
#include <stdlib.h>
// n  : so dinh
// start: dinh xuat phat
//edgeCount : so canh do thi
//edge[]: mang chua ds cac canh

#define MAXN 100
#define INF 1000000000

typedef struct {
    int u, v, w;
} Edge;

int distArr[MAXN];
int parentArr[MAXN];

void printState(FILE *out, int n) {
    fprintf(out, "\n%-10s %-10s %-10s\n", "Dinh", "dist", "parent");
    fprintf(out, "--------------------------------\n");
    for (int i = 0; i < n; i++) {
        if (distArr[i] >= INF / 2)
            fprintf(out, "%-10d %-10s ", i, "INF");
        else
            fprintf(out, "%-10d %-10d ", i, distArr[i]);

        if (parentArr[i] == -1)
            fprintf(out, "%-10s\n", "null");
        else
            fprintf(out, "%-10d\n", parentArr[i]);
    }
}

void printPath(FILE *out, int start, int end) {
    if (distArr[end] >= INF / 2) {
        fprintf(out, "\nKhong ton tai duong di tu %d den %d\n", start, end);
        return;
    }

    int path[MAXN];
    int len = 0;
    int cur = end;

    while (cur != -1) {
        path[len++] = cur;
        if (cur == start) break;
        cur = parentArr[cur];
    }

    if (path[len - 1] != start) {
        fprintf(out, "\nKhong truy vet duoc duong di.\n");
        return;
    }

    fprintf(out, "\nDuong di ngan nhat tu %d den %d: ", start, end);
    for (int i = len - 1; i >= 0; i--) {
        fprintf(out, "%d", path[i]);
        if (i > 0) fprintf(out, " -> ");
    }
    fprintf(out, "\nTong trong so: %d\n", distArr[end]);
}

int hasNegativeCycle(int edgeCount, Edge edges[]) {
    for (int i = 0; i < edgeCount; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].w;

        if (distArr[u] < INF / 2 && distArr[u] + w < distArr[v]) {
            return 1;
        }
    }
    return 0;
}

void bellmanFord(FILE *out, int n, int start, int end, int edgeCount, Edge edges[]) {
    for (int i = 0; i < n; i++) {
        distArr[i] = INF;   
        parentArr[i] = -1;
    }
    distArr[start] = 0;

    fprintf(out, "========================================\n");
    fprintf(out, "THUAT TOAN BELLMAN-FORD\n");
    fprintf(out, "So dinh: %d\n", n);
    fprintf(out, "Dinh xuat phat: %d\n", start);
    fprintf(out, "Dinh ket thuc : %d\n", end);
    fprintf(out, "So canh: %d\n", edgeCount);
    fprintf(out, "========================================\n");

    fprintf(out, "\nTrang thai khoi tao:\n");
    printState(out, n);

    for (int round = 1; round <= n - 1; round++) {
        int updated = 0;

        fprintf(out, "\n========================================\n");
        fprintf(out, "VONG %d\n", round);
        fprintf(out, "========================================\n");

        for (int step = 0; step < edgeCount; step++) {
            int u = edges[step].u;
            int v = edges[step].v;
            int w = edges[step].w;

            fprintf(out, "\nBuoc %d: Xet canh %d -> %d (w = %d)\n", step + 1, u, v, w);

            if (distArr[u] >= INF / 2) {
                fprintf(out, "dist[%d] = INF => bo qua\n", u);
                continue;
            }

            int candidate = distArr[u] + w;

            if (distArr[v] >= INF / 2)
                fprintf(out, "So sanh: dist[%d] + %d = %d < INF ?\n", u, w, candidate);
            else
                fprintf(out, "So sanh: dist[%d] + %d = %d < dist[%d] = %d ?\n",
                        u, w, candidate, v, distArr[v]);

            if (candidate < distArr[v]) {
                int oldDist = distArr[v];
                int oldParent = parentArr[v];

                distArr[v] = candidate;
                parentArr[v] = u;
                updated = 1;

                fprintf(out, "=> CAP NHAT\n");
                if (oldDist >= INF / 2)
                    fprintf(out, "   dist[%d]: INF -> %d\n", v, distArr[v]);
                else
                    fprintf(out, "   dist[%d]: %d -> %d\n", v, oldDist, distArr[v]);

                if (oldParent == -1)
                    fprintf(out, "   parent[%d]: null -> %d\n", v, u);
                else
                    fprintf(out, "   parent[%d]: %d -> %d\n", v, oldParent, u);
            } else {
                fprintf(out, "=> KHONG CAP NHAT\n");
            }
        }

        fprintf(out, "\nKet thuc vong %d:\n", round);
        printState(out, n);

        if (!updated) {
            fprintf(out, "\nKhong con cap nhat nao, dung som.\n");
            break;
        }
    }

    fprintf(out, "\n========================================\n");
    fprintf(out, "KET LUAN\n");
    fprintf(out, "========================================\n");

    if (hasNegativeCycle(edgeCount, edges)) {
        fprintf(out, "Do thi co chu trinh am reachable tu dinh %d\n", start);
    } else {
        printState(out, n);
        printPath(out, start, end);
    }
}

void readInput(const char *filename, int *n, int *start, int *end, int *edgeCount, Edge edges[]) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Khong mo duoc file %s\n", filename);
        exit(1);
    }

    fscanf(f, "%d", n);
    fscanf(f, "%d %d", start, end);

    int matrix[MAXN][MAXN];
    *edgeCount = 0;

    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            fscanf(f, "%d", &matrix[i][j]);
            if (matrix[i][j] != -1 && i != j) {
                edges[*edgeCount].u = i;
                edges[*edgeCount].v = j;
                edges[*edgeCount].w = matrix[i][j];
                (*edgeCount)++;
            }
        }
    }

    fclose(f);
}

int main() {
    int n, start, end, edgeCount;
    Edge edges[MAXN * MAXN];

    readInput("input.txt", &n, &start, &end, &edgeCount, edges);

    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Khong mo duoc file output.txt\n");
        return 1;
    }

    bellmanFord(out, n, start, end, edgeCount, edges);

    fclose(out);

    printf("Da ghi ket qua vao file output.txt\n");
    return 0;
}