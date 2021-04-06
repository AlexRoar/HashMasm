#include <cstdlib>
#include <cstdio>
#include "HashMasm.h"
#include "helpers/timingTools.h"
#include "helpers/leoTestsHelpers.h"

#define SIZE_MODIF (/10);

const size_t maxWords = -1;

int main(int argc, const char **argv) {

    if (argc != 2) {
        printf("Wrong command line args. Expected one argument - file to process.\n");
        return EXIT_FAILURE;
    }
    char *content = readFile(argv);
    if (!content)
        return EXIT_FAILURE;

    size_t allWhitespaces = countFileWhitespace(content);
    char *savedContent = strdup(content);
    allWhitespaces = min(allWhitespaces, maxWords);

    TIME({
             hashMasmTest(allWhitespaces, content);
         }, "My hash table:");
    strcpy(content, savedContent);
    free(savedContent);
    return 0;
}

