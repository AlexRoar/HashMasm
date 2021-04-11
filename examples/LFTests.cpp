#include <cstdlib>
#include <cstdio>
#include "HashMasm.h"
#include "helpers/timingTools.h"
#include "helpers/leoTestsHelpers.h"

#define SIZE_MODIF * 10
const size_t maxWords = -1;

int main(int argc, const char **argv) {
    if (argc != 2) {
        printf("Wrong command line args. Expected one argument - file to process.\n");
        return EXIT_FAILURE;
    }
    char *content = readFile(argv);
    char* contentCopy = strdup(content);
    if (!content)
        return EXIT_FAILURE;

    size_t allLines = countFileLines(content);
    size_t i = 0;

    FILE *data = fopen("testres.csv", "wb");
    for (int lr = 5; lr < 150; lr += 5) {
        char *token = content;
        TIMENOPRINTF({
                         HashMasm<char *> htable = {};
                         htable.init(0, true, lr);
                         while (token > (char *) 2) {
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

                         htable.dest();

                         struct timeval res = {};
                         GETTIME(&res);
                         fprintf(data, "%ld.%06ld, ", (long int) res.tv_sec, (long int) res.tv_usec);
                     });
        fflush(stdout);
        strcpy(content, contentCopy);
    }
    fclose(data);
    free(content);
    free(contentCopy);
    return 0;
}

