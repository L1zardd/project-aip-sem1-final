#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LEN 20
#define BASE 16

char word1[MAX_LEN], word2[MAX_LEN], word3[MAX_LEN];
int len1, len2, len3, max_len;
char letters[17];
int uniq_letters = 0;
int val[256];
bool used[16];
bool not_zero[256];

void reverse_str(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; i++) {
        char t = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = t;
    }
}

bool is_valid(void) {
    if (len1 > 1 && val[(unsigned char)word1[len1 - 1]] == 0) return false;
    if (len2 > 1 && val[(unsigned char)word2[len2 - 1]] == 0) return false;
    if (len3 > 1 && val[(unsigned char)word3[len3 - 1]] == 0) return false;

    int carry = 0;
    for (int i = 0; i < len3; i++) {
        int d1 = (i < len1) ? val[(unsigned char)word1[i]] : 0;
        int d2 = (i < len2) ? val[(unsigned char)word2[i]] : 0;
        int d3 = val[(unsigned char)word3[i]];

        int sum = d1 + d2 + carry;
        if (sum % BASE != d3) {
            return false;
        }
        carry = sum / BASE;
    }

    return carry == 0;
}

void print_solution(void) {
    for (int i = len1 - 1; i >= 0; i--) printf("%X", val[(unsigned char)word1[i]]);
    printf(" + ");
    for (int i = len2 - 1; i >= 0; i--) printf("%X", val[(unsigned char)word2[i]]);
    printf(" = ");
    for (int i = len3 - 1; i >= 0; i--) printf("%X", val[(unsigned char)word3[i]]);
    printf("\n");
}

bool solve(int n) {
    if (n == uniq_letters) {
        if (is_valid()) {
            print_solution();
            return true;
        }
        return false;
    }

    unsigned char ch = (unsigned char)letters[n];
    for (int d = 0; d < BASE; d++) {
        if (used[d]) continue;
        if (d == 0 && not_zero[ch]) continue;

        val[ch] = d;
        used[d] = true;

        if (solve(n + 1)) {
            return true;
        }

        used[d] = false;
    }

    return false;
}

int main(void) {
    if (scanf("%19s %19s %19s", word1, word2, word3) != 3) {
        return 0;
    }

    reverse_str(word1);
    reverse_str(word2);
    reverse_str(word3);

    len1 = strlen(word1);
    len2 = strlen(word2);
    len3 = strlen(word3);

    if (len3 < len1 || len3 < len2) {
        printf("Решений нет\n");
        return 0;
    }

    if (len1 > 1) not_zero[(unsigned char)word1[len1 - 1]] = true;
    if (len2 > 1) not_zero[(unsigned char)word2[len2 - 1]] = true;
    if (len3 > 1) not_zero[(unsigned char)word3[len3 - 1]] = true;

    max_len = len3;
    bool seen[256] = {false};

    for (int col = 0; col < max_len; col++) {
        char current_chars[3] = {
            (col < len1) ? word1[col] : 0,
            (col < len2) ? word2[col] : 0,
            (col < len3) ? word3[col] : 0
        };
        for (int i = 0; i < 3; i++) {
            unsigned char c = (unsigned char)current_chars[i];
            if (c != 0 && !seen[c]) {
                if (uniq_letters == 16) {
                    printf("Решений нет\n");
                    return 0;
                }
                seen[c] = true;
                letters[uniq_letters++] = c;
            }
        }
    }

    if (!solve(0)) {
        printf("Решений нет\n");
    }

    return 0;
}

/*
A B C
A A B
A A A
A B CD
AA BB CDC
ABC DEF GHIJ
AB C AB
AB CD EF
AB BA CC
ABC D ABE
A BCD BEF
a b c
A a B
TWO TWO FOUR
SEND MORE MONEY
ABC D EF
A B CDEF
AA AA AA
AA BB CCC
ABCDEFGH IJKLMNOP QRSTUVW
*/
