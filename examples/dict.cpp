#include <cstdlib>
#include <cstdio>
#include "HashMasm.h"
#include "helpers/timingTools.h"
#include "helpers/leoTestsHelpers.h"

const size_t bufferSize = 150;


int main(int argc, const char **argv) {
    if (argc != 2) {
        printf("Wrong command line args. Expected one argument - file to process.\n");
        return EXIT_FAILURE;
    }
    char *content = readFile(argv);
    if (!content)
        return EXIT_FAILURE;

    size_t allLines = countFileLines(content);

    HashMasm<char *> htable = {};
    htable.init();

    char *token = content;
    size_t i = 0;
    TIME({
             while (token > (char *) 2) {
                 printf("\b\r%d%%", int(float(i * 100) / float(allLines)));
                 fflush(stdout);
                 char *endLine = strchr(token, '\n');
                 if (endLine)
                     *endLine = 0;
                 token = flattenString(token);
                 char *sep = strchr(token, '=');
                 if (!sep)
                     break;
                 *sep = 0;
                 htable.set(token, sep + 1, false);
                 token = endLine + 1;
                 i++;
             }
             printf("\r100%% (%zu words)\nTranslator is ready\n", htable.getSize());
         }, "Build time");

    auto *buffer = (char *) (calloc(bufferSize, 1));
    while (true) {
        scanf("%99s", buffer);
        if (strcmp("end", buffer) == 0)
            break;
        else if (strcmp("all", buffer) == 0) {
            for (const auto &elem : htable)
                printf("%s -> %s\n", elem.key, elem.value);
            continue;
        }
        char **found = htable.get(buffer);
        if (found) {
            printf("`%s` -> `%s`\n", buffer, *found);
        } else {
            printf("There is no `%s` in dictionary\n", buffer);
        }
    }

    htable.dest();
    free(content);
    return 0;
}

