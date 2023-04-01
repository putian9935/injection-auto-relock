#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "read.h"
#include "rp.h"
#include "write.h"

double data[512];
char databuf[512 * 8];

int main(int argc, char **argv) {
    if (rp_Init() != RP_OK) {
        exit(-1);
    }

    CURL *curl = curl_easy_init();
    CURLcode res;
    if (!curl) {
        fprintf(stderr, "curl initializtion failed!\n");
        return -1;
    }
    
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, "Expect:");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, sizeof databuf);
    curl_easy_setopt(curl, CURLOPT_URL, HOST_ADDR);
    curl_easy_setopt(curl, CURLOPT_LOCALPORT, 59152L);
    curl_easy_setopt(curl, CURLOPT_LOCALPORTRANGE, 20L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, databuf);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    write_prepare();
    read_prepare();

    write_ch2(0.5);
    while (1) {
        for (int i = 0; i < 512; ++i) {
            write_ch1((float)i / (float)512.);
            data[i] = read_ch1_avg();
        }

        memcpy(databuf, data, sizeof data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
    }

    rp_Release();
    return 0;
}
