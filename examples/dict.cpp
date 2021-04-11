#include <cstdlib>
#include <cstdio>
#include "HashMasmFixed.h"
#include "helpers/timingTools.h"
#include "helpers/leoTestsHelpers.h"

const size_t bufferSize = 150;

void processInput(size_t allLines, HashMasmFixed<char *> &htable, char *token, size_t i);

void processUserInput(HashMasmFixed<char *> &htable, char *buffer);

int main(int argc, const char **argv) {
    if (argc != 2) {
        printf("Wrong command line args. Expected one argument - file to process.\n");
        return EXIT_FAILURE;
    }
    char *content = readFile(argv);
    if (!content)
        return EXIT_FAILURE;

    size_t allLines = countFileLines(content);

    HashMasmFixed<char *> htable = {};
    htable.init(allLines * 2);

    char *token = content;
    size_t i = 0;
    TIME({
             processInput(allLines, htable, token, i);
             printf("\r100%% (%zu words)\nTranslator is ready\n", htable.getSize());
         }, "Build time");

    auto *buffer = (char *) (calloc(bufferSize, 1));
    processUserInput(htable, buffer);

    htable.dest();
    free(content);
    return 0;
}

void processUserInput(HashMasmFixed<char *> &htable, char *buffer) {
    while (true) {
        scanf("%99s", buffer);
        if (strcmp("end", buffer) == 0)
            break;
        else if (strcmp("all", buffer) == 0) {
            for (const auto &elem : htable)
                printf("%s -> %s\n", elem.key, elem.value);
            continue;
        }
        char *tokenFlat = flattenString(buffer);
        __m256i tokenKey = makePacked(tokenFlat);
        char **found = htable.get(tokenKey);
        if (found) {
            printf("`%s` -> `%s`\n", tokenFlat, *found);
        } else {
            printf("There is no `%s` in dictionary\n", tokenFlat);
        }
    }
}

void processInput(size_t allLines, HashMasmFixed<char *> &htable, char *token, size_t i) {
    while (token > (char *) 2) {
        char *endLine = strchr(token, '\n');
        if (endLine)
            *endLine = 0;
        token = flattenString(token);
        char *sep = strchr(token, '=');
        if (!sep)
            break;
        *sep = 0;
        __m256i tokenKey = makePacked(token);
        htable.set(tokenKey, sep + 1);
        token = endLine + 1;
        i++;
    }
}

