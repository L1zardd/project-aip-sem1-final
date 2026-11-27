#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


#define MAX_LEN 20
#define BASE 16

char word1[MAX_LEN], word2[MAX_LEN], word3[MAX_LEN];
int len1, len2, len3;
int val[256];
bool used[BASE];
bool not_zero[256];

void reverse_str(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; i++) {
        char t = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = t;
    }
}

void print_solution(void) {
    for (int i = len1 - 1; i >= 0; i--) printf("%X", val[(unsigned char)word1[i]]);
    printf(" + ");
    for (int i = len2 - 1; i >= 0; i--) printf("%X", val[(unsigned char)word2[i]]);
    printf(" = ");
    for (int i = len3 - 1; i >= 0; i--) printf("%X", val[(unsigned char)word3[i]]);
    printf("\n");
}

bool solve(int col, int row, int carry) {
    if (col == len3) {
        if (carry == 0) {
            print_solution();
            return true;
        }
        return false;
    }

    if (row == 0) {
        if (col >= len1) {
            return solve(col, 1, carry);
        }
        unsigned char ch = (unsigned char)word1[col];
        if (val[ch] != -1) {
            return solve(col, 1, carry);
        }
        for (int d = 0; d < BASE; d++) {
            if (used[d]) continue;
            if (d == 0 && not_zero[ch]) continue;

            val[ch] = d;
            used[d] = true;

            if (solve(col, 1, carry)) {
                return true;
            }

            val[ch] = -1;
            used[d] = false;
        }
        return false;
    }

    if (row == 1) {
        if (col >= len2) {
            return solve(col, 2, carry);
        }
        unsigned char ch = (unsigned char)word2[col];
        if (val[ch] != -1) {
            return solve(col, 2, carry);
        }
        for (int d = 0; d < BASE; d++) {
            if (used[d]) continue;
            if (d == 0 && not_zero[ch]) continue;

            val[ch] = d;
            used[d] = true;

            if (solve(col, 2, carry)) {
                return true;
            }

            val[ch] = -1;
            used[d] = false;
        }
        return false;
    }
    
    int d1 = (col < len1) ? val[(unsigned char)word1[col]] : 0;
    int d2 = (col < len2) ? val[(unsigned char)word2[col]] : 0;
    int sum = d1 + d2 + carry;
    int target = sum % BASE;
    int next_carry = sum / BASE;

    unsigned char ch3 = (unsigned char)word3[col];

    if (val[ch3] != -1) {
        if (val[ch3] != target) {
            return false;
        }
        return solve(col + 1, 0, next_carry);
    }

    if (used[target]) return false;
    if (target == 0 && not_zero[ch3]) return false;

    val[ch3] = target;
    used[target] = true;

    if (solve(col + 1, 0, next_carry)) {
        return true;
    }

    val[ch3] = -1;
    used[target] = false;
    return false;
}

int main(void) {
    struct timespec start, end;
    
    
    if (scanf("%19s %19s %19s", word1, word2, word3) != 3) {
        return 0;
    }
    clock_gettime(CLOCK_MONOTONIC, &start);

    reverse_str(word1);
    reverse_str(word2);
    reverse_str(word3);

    len1 = strlen(word1);
    len2 = strlen(word2);
    len3 = strlen(word3);

    int max_addend = (len1 > len2) ? len1 : len2;
    if (len3 < max_addend || len3 > max_addend + 1) {
        printf("Решений нет\n");
        return 0;
    }

    bool seen[256] = {false};
    int uniq_letters = 0;
    char *words[3] = {word1, word2, word3};
    int lens[3] = {len1, len2, len3};

    for (int i = 0; i < 256; i++) {
        val[i] = -1;
    }

    for (int w = 0; w < 3; w++) {
        for (int i = 0; i < lens[w]; i++) {
            unsigned char c = (unsigned char)words[w][i];
            if (!seen[c]) {
                seen[c] = true;
                uniq_letters++;
                if (uniq_letters > BASE) {
                    printf("Решений нет\n");
                    return 0;
                }
            }
        }
    }

    if (len1 > 1) not_zero[(unsigned char)word1[len1 - 1]] = true;
    if (len2 > 1) not_zero[(unsigned char)word2[len2 - 1]] = true;
    if (len3 > 1) not_zero[(unsigned char)word3[len3 - 1]] = true;

    if (!solve(0, 0, 0)) {
        printf("Решений нет\n");
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Время выполнения: %f секунд\n", time_taken);
    return 0;
}
