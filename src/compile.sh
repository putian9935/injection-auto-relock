gcc write.c read.c peak_detect.c length_lock.c current_lock.c main.c\
    -std=gnu11 -Wall  \
    -I/opt/redpitaya/include \
    -DZ10 \
    -L/opt/redpitaya/lib -static -lrp -lrp-hw -lm -lstdc++ -lpthread \
    -o main

gcc write.c read.c get_error_signal.c\
    -std=gnu11 -Wall  \
    -I/opt/redpitaya/include \
    -DZ10 \
    -L/opt/redpitaya/lib -static -lrp -lrp-hw -lm -lstdc++ -lpthread \
    -o get_error_signal    

# gcc write.c read.c sweep.c \
#     -std=gnu11 -Wall  \
#     -I/opt/redpitaya/include \
#     -DZ10 \
#     -L/opt/redpitaya/lib -static -lrp -lrp-hw -lm -lstdc++ -lpthread \
#     -o sweep
