#include <stdio.h>
void printer(int, const char *, int);
void slicer(int, int, int, int *, int *);
//String s with length n
//two numbers p,q are given
int main() {
    int n, p, q;
    scanf("%d", &n);
    scanf("%d", &p);
    scanf("%d", &q);
 //reads data from stdin
 //writes the result into the given arguments
    char str[100];
    scanf("%s", str);
    int Pcount = 0;
    int Qcount = 0;
    slicer(n, p, q, &Pcount, &Qcount);

    if (Pcount + Qcount == -1) {
        printf("%d", -1);
        return 0;
    }
    //n, p and q in the first line 
    //s in the second line
    printf("%d\n", Pcount + Qcount);
    int counting = 0;
    for (int z = 0; z < Pcount; z++) {
        printer(p, str, counting);
    }
    for (int h = 0; h < Qcount; h++) {
        printer(q, str, counting);
    }
    return 0;
}
//In the first line, print the number of substrings
void printer(int partLength, const char *string, int pointer) {
    for (int i = 0; i < partLength; i++) {
        printf("%c", string[pointer + i]);
    }
    printf("\n");
    pointer += partLength;
}
//number of line =(k)
//(k) Print next line of substrings
void slicer(int length, int part1, int part2, int *part1Length, int *part2Legth) {
    if (length % part1 != 0) {
        if (length % part2 == 0)
            (*part2Legth) += length / part2;
        else {
            length -= (part1 + part2);
            (*part1Length)++;
            (*part2Legth)++;
            if (length > 0) {
                slicer(length, part1, part2, part1Length, part2Legth);
            } else if (length < 0) {
                (*part1Length) = 0;
                (*part2Legth) = -1;
            }
            //If  not possible print -1
        }
    } else (*part1Length) += length / part1;
}