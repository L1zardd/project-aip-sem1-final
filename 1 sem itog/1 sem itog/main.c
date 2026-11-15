#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LEN 20

char word1[MAX_LEN], word2[MAX_LEN], word3[MAX_LEN];

char letters[17];
int uniq_letters = 0;

int val[256];

bool used[16];

bool not_zero[256];

void reverse_str(char *s){
    int len = strlen(s);
    for(int i = 0; i < len/2; i++){
        char t = s[i];
        s[i] = s[len - 1 + i];
        s[len - 1 + i] = t;
    }
}



int main(void) {
    scanf("%19s %19s %19s", word1, word2, word3);
    
    reverse_str(word1); reverse_str(word2); reverse_str(word3);
    
    bool seen[256] = {false};
    
    int len1 = strlen(word1), len2 = strlen(word2), len3 = strlen(word3);
    
    int max_len = len1;
    if (len2 > max_len) max_len = len2;
    if (len3 > max_len) max_len = len3;
    
    for(int col = 0; col < max_len; col++){
        char current_chars[3] = {
            (col < len1) ? word1[col] : 0,
            (col < len2) ? word2[col] : 0,
            (col < len3) ? word3[col] : 0
        };
        for(int i = 0; i < 3; i++){
            unsigned char c = current_chars[i];
            if (c != 0 && !seen[c]) {
                seen[c] = true;
                letters[uniq_letters++] = c;
                    if (uniq_letters > 16) {
                        printf("Решений нет\n");
                        return 0;
                }
            }
        }
        
    }
    printf("%s", letters);
    
    return 0;
}
