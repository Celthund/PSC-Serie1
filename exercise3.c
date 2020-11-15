#include <stdio.h>
#include <ctype.h>
#include <memory.h>

int isseperator(char c){
    return isspace(c) | (c == ',');
}

int string_split(char *text, char *words[], size_t words_size){
    char *begin_word = &text[0], *current_char = &text[0];
    int found_words = 0;
    while(*current_char != '\0' && found_words < words_size){
        if (isseperator(*current_char) > 0){
            if (isseperator(*begin_word) == 0) {
                words[found_words] = begin_word;
                found_words++;
            }
            *current_char++ = '\0';
            begin_word = current_char;
        } else {
            current_char++;
        }
    }
    return found_words;
}

int main (){
    char text[] = "Teste, um dois tres \n era uma vez \t.";
    char *words[10] = {0};
    printf("%d", string_split(text, words, sizeof(words) / sizeof(words[0])));
    return 0;
}