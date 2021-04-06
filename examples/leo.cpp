#include <cstdlib>
#include <cstdio>
#include "HashMasm.h"
#include "helpers/timingTools.h"
#include "helpers/leoTestsHelpers.h"

const size_t maxWords = -1;
const size_t bufferSize = 150;

int main(int argc, const char **argv) {
    if (argc != 2) {
        printf("Wrong command line args. Expected one argument - file to process.\n");
        return EXIT_FAILURE;
    }
    char *content = readFile(argv);
    if (!content)
        return EXIT_FAILURE;

    size_t allWhitespaces = countFileWhitespace(content);
    allWhitespaces = min(allWhitespaces, maxWords);

    HashMasm<unsigned> htable = {};
    htable.init();

    char *token = strtok(content, " ");
    size_t i = 0;
    TIME({
             while (token && i < allWhitespaces) {
                 printf("\b\r%d%%", int(float(i * 100) / float(allWhitespaces)));
                 fflush(stdout);
                 token = flattenString(token);
                 auto *valuePtr = htable.get(token);
                 if (valuePtr == nullptr)
                     htable.set(token, 1, false);
                 else
                     htable.set(token, 1 + *valuePtr, false);
                 token = strtok(NULL, " ");
                 i++;
             }
             printf("\r100%% (%zu words)\nLeo git is ready\n", htable.getSize());
         }, "Loaded 5 `War and Peace`");

    auto *buffer = (char *) (calloc(bufferSize, 1));
    while (true) {
        scanf("%99s", buffer);
        if (strcmp("end", buffer) == 0)
            break;
        else if (strcmp("all", buffer) == 0) {
            for (const auto &elem : htable)
                printf("%s -> %u\n", elem.key, elem.value);
            continue;
        }
        unsigned *found = htable.get(buffer);
        if (found) {
            printf("There's %u `%s` in `War And Peace`\n", *found, buffer);
        } else {
            printf("There is no `%s` in `War And Peace`\n", buffer);
        }
    }

    htable.dest();

    free(content);
    return 0;
}

