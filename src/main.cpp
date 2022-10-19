#include "headers/main.h"
#include "contrib/skinny-plus/headers/internal-skinny-plus.h"

int main() {
    unsigned char key[48] = {};
    auto* tks = new skinny_plus_key_schedule_t();

    skinny_plus_init(tks, key);
    return 0;
}
