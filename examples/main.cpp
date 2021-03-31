#include <cstdlib>
#include <cstdio>
#include <HashMasm.h>

int main() {
    HashMasm<int> htable = {};
    htable.init(4);

    auto val = htable.get("absd");

    htable.dest();
    return 0;
}
