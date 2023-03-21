#include <stdlib.h>
#include <unistd.h>

#include "read.h"
#include "rp.h"
#include "write.h"

int main(int argc, char **argv) {
    if (rp_Init() != RP_OK) {
        exit(-1);
    }

    write_prepare();
    read_prepare();

    write_ch2(0.5);
    while (1) {
        for (float i = 0; i < 1; i += 1. / 512) {
            write_ch1(i);
            usleep(2);
        }
    }

    rp_Release();
    return 0;
}
