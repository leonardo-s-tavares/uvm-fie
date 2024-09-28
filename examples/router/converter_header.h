#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void str_replace(char *target, const char *needle, const char *replacement)
{
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *p = strstr(tmp, needle);

        // walked past last occurrence of needle; copy remaining part
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        // copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // adjust pointers, move on
        tmp = p + needle_len;
    }

    // write altered string back to target
    strcpy(target, buffer);
}

void clean_matrix(char matrix[100][100]) {
    int i, j;
    
    for(i = 0; i < 100; i++) {
        for(j = 0; j < 100; j++) {
            matrix[i][j] = '\0';
        }
    }
}

char *extract(const char *const string, const char *const left, const char *const right)
{
    char  *head;
    char  *tail;
    size_t length;
    char  *result;

    if ((string == NULL) || (left == NULL) || (right == NULL))
        return NULL;
    length = strlen(left);
    head   = strstr(string, left);
    if (head == NULL)
        return NULL;
    head += length;
    tail  = strstr(head, right);
    if (tail == NULL)
        return tail;
    length = tail - head;
    result = malloc(1 + length);
    if (result == NULL)
        return NULL;
    result[length] = '\0';

    memcpy(result, head, length);
    return result;
}

int count_occurrences(const char *str, const char *substr) {
    int count = 0;
    const char *p = str;

    while ((p = strstr(p, substr)) != NULL) {
        count++;
        p += strlen(substr);
    }

    return count;
}
