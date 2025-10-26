#include <stdio.h>

void end(int *, unsigned long long int *, int *, int *);

void loop(int *, unsigned long long int *, int *, int *);

int main() {
    int count;
    int sets[100];
    unsigned long long sumSets = 1;
    unsigned long long result = 0;
    scanf("%d", &count);
    int loopCount, set1 = 0;
    for (int j = 0; j < count; j++) {
        char cmd[3];
        scanf("%s", cmd);
        if (set1 != 0) {
            sets[0] = set1;
        }
        switch (cmd[0]) {
            case 'a':
                result += sumSets;
                break;

            case 'e':
                end(sets, &sumSets, &loopCount, &set1);
                break;

            case 'f': {
                loop(sets, &sumSets, &loopCount, &set1);
                break;
            }
        }
    }
    if (result > 4294967295) { printf("OVERFLOW!!!"); }
    else { printf("%llu", result); }
    return 0;
}

void loop(int *sets, unsigned long long int *sumSets, int *loopCount, int *set1) {
    int set;
    scanf("%d", &set);
    sets[(*loopCount)] = set;
    (*sumSets) *= set;
    if ((*loopCount) == 0) {
        (*set1) = set;
    }
    (*loopCount)++;
}

void end(int *sets, unsigned long long int *sumSets, int *loopCount, int *set1) {
    (*loopCount)--;
    (*sumSets) /= sets[(*loopCount)];
    sets[(*loopCount)] = 0;
    if ((*loopCount) == 0) {
        (*set1) = 0;
    }
}
