#include <stdio.h>

int stringEqual(const char *, const char *);

int copy_array(const char *, char *, int);

int main() {
    int lines;
    char names[100][100];
    int points[100];
    int count = 0, index = 0;
    int winnerScore = -100;
    scanf("%d", &lines);
    for (int line = 0; line < lines; line++) {
        char name[100];
        scanf("%s", name);
        int score;
        scanf("%d", &score);
        for (int i = 0; i < count; i++)
            if (stringEqual(name, names[i]) == 0) {
                points[i] += score;
                if (points[i] <= winnerScore) {
                    break;
                } else {
                    winnerScore = points[i];
                    index = i;
                }
                break;
            }

        copy_array(name, names[count], 100);
        points[count] = score;
        if (winnerScore < points[count]) {
            winnerScore = points[count];
            index = count;
        }
        count++;
    }
    printf("%s", names[index]);
    return 0;
}

int stringEqual(const char *a, const char *b) {
    int flag = 0, i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            flag = 1;
            break;
        }
        i++;
    }
    if (flag == 0)
        return 0;
    else
        return 1;
}

int copy_array(const char *a, char *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        b[i] = a[i];
    }
}
