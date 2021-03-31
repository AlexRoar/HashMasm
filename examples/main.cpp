#include <cstdlib>
#include <cstdio>
#include <unordered_map>
#include "../src/HashMasm.h"
#include "timingTools.h"

const size_t maxWords = -1;

char *readFile(const char *const *argv);

size_t countFileWhitespace(const char *content);

char *flattenString(char *token);

template<typename T>
T min(T a, T b);

void hashMasmTest(size_t allWhitespaces, char *content);

void hashStdTest(size_t allWhitespaces, char *content);

int main(int argc, const char **argv) {
    if (argc != 2) {
        printf("Wrong command line args. Expected one argument - file to process.\n");
        return EXIT_FAILURE;
    }
    char *content = readFile(argv);
    char *initialContent = content;
    if (!content)
        return EXIT_FAILURE;

    size_t allWhitespaces = countFileWhitespace(content);
    char *savedContent = strdup(content);
    allWhitespaces = min(allWhitespaces, maxWords);

    TIME({
             hashMasmTest(allWhitespaces, content);
         }, "My hash table:");
    strcpy(content, savedContent);
    TIME({
             hashStdTest(allWhitespaces, content);
         }, "Std unordered map:");

    free(initialContent);
    free(savedContent);
    return 0;
}

void hashMasmTest(size_t allWhitespaces, char *content) {
    HashMasm<unsigned> htable = {};
    htable.init(allWhitespaces);

    char *token = strtok(content, " ");
    size_t i = 0;
    while (token && i < allWhitespaces) {
        printf("\b\r%d%%", int(float(i * 100) / float(allWhitespaces)));
        fflush(stdout);
        token = flattenString(token);
        auto *valuePtr = htable.get(token);
        if (valuePtr == nullptr) {
            htable.set(token, 1, false);
        } else {
            htable.set(token, 1 + *valuePtr, false);
        }
        token = strtok(NULL, " ");
        i++;
    }
    printf("\r100%%\n");
    htable.dest();
}

void hashStdTest(size_t allWhitespaces, char *content) {
    std::unordered_map<const char *, unsigned> htable;
    htable.reserve(allWhitespaces);

    char *token = strtok(content, " ");
    size_t i = 0;
    while (token && i < allWhitespaces) {
        printf("\b\r%d%%", int(float(i * 100) / float(allWhitespaces)));
        fflush(stdout);
        token = flattenString(token);
        auto valuePtr = htable.find(token);
        if (valuePtr == htable.end()) {
            htable[token] = 1;
        } else {
            htable[token] = 1 + valuePtr->second;
        }
        token = strtok(NULL, " ");
        i++;
    }
    printf("\r100%%\n");
}

template<typename T>
T min(const T a, const T b) {
    return a > b ? b : a;
}

char *readFile(const char *const *argv) {
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("File (%s) is not found\n", argv[1]);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);  /* same as rewind(f); */

    auto string = (char *) malloc(fsize + 2);
    fread(string, 1, fsize, file);
    fclose(file);
    string[fsize] = 0;
    return string;
}

char *flattenString(char *token) {
    char *ret = token;
    while (*token == ' ') {
        token++;
        ret = token;
    }
    while (*token != 0) {
        switch (*token) {
            case '\n':
            case '\t':
            case '-':
            case '\r':
                *token = ' ';
        }
        token++;
    }
    token--;
    while (*token == ' ' && token > ret) {
        *token = 0;
        token--;
    }
    return ret;
}

size_t countFileWhitespace(const char *content) {
    size_t res = 0;
    while (*content != 0) {
        if (*content == ' ')
            res++;
        content++;
    }
    return res;
}