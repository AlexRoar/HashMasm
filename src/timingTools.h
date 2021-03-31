//
// Created by Александр Дремов on 31.03.2021.
//

#ifndef HASHMASM_TIMINGTOOLS_H
#define HASHMASM_TIMINGTOOLS_H
#include <sys/time.h>

#define TIME(code, msg) \
do { \
struct timeval tval_before = {}, tval_after = {}, tval_result = {}; \
bool ended = false; \
gettimeofday(&tval_before, NULL);\
{code} \
if (!ended){ \
gettimeofday(&tval_after, NULL);\
timersub(&tval_after, &tval_before, &tval_result); \
}printf("%s elapsed: %ld.%06ld \n", msg, (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);\
} while(0)

#define SET_TIME do { \
gettimeofday(&tval_after, NULL);\
timersub(&tval_after, &tval_before, &tval_result);\
ended = true;\
} while(0)

#endif //HASHMASM_TIMINGTOOLS_H
