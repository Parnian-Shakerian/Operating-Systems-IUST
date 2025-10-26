#include <stdio.h>
//The input consists of two lines
int goodFinder(int, const int *);
//in the first line n
//in the second line an array of length n
int main() {
    int count;
    int days[100];
    scanf("%d", &count);
    for (int i = 0; i < count; i++) {
        scanf("%d", &days[i]);
    }
    int good = goodFinder(count, days);

    printf("%d", good);

    return 0;
}
int goodFinder(int count, const int *days) {
    int good = 0;
    //The output should contain 1 line
    //number of good days out of these n days.
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (days[j] < days[i]) {
                good++;
                break;
            }
        }
    }
    return good;
}