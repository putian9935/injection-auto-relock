#include <stdlib.h>

#include "rp.h"
#include "read.h"
#include "write.h"
#include "length_lock.h"
#include "current_lock.h"

#include <unistd.h>
int main(int argc, char **argv)
{

    if (rp_Init() != RP_OK)
    {
        exit(-1);
    }

    // initialize read and write
    write_prepare();
    read_prepare();

    // put current to lock point and wait
    write_ch2(.5);
    usleep(20000);

    // start length lock for some time
    // peak_detect will also set current setpoint 
    init_length_lock(0.0002, 0.0001, 0.00, peak_detect());
    for(int i = 0; i < 200; ++i)
        update_length_lock();

    // start current lock 
    init_current_lock(.2, .2, 0.1, .5);
    while(1)
    {
        for(int i = 0; i < 250; ++i)
            update_length_lock();
        update_current_lock();
    }

    write_ch2(0.5);
    rp_Release();
    return 0;
}
