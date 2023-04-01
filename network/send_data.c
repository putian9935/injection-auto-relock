#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

double data[512];
char databuf[4096];

int main() {
    CURL *curl = curl_easy_init();
    CURLcode res;
    int cnt = 0;

    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, "Expect:");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 4096);
    curl_easy_setopt(curl, CURLOPT_LOCALPORT, 59152L);
    curl_easy_setopt(curl, CURLOPT_LOCALPORTRANGE, 20L);
    curl_easy_setopt(curl, CURLOPT_URL, "http://172.31.20.147:65432/");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, databuf);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    if (curl) {
        while (1) {
            /* pass pointer that gets passed in to the
                CURLOPT_READFUNCTION callback */
            int i;
            for (i = 0; i < 512; ++i)
                data[i] = ((double)rand() / (double)RAND_MAX);
            memcpy(databuf, data, sizeof data);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
            usleep(20000u);
        }
    }
    return 0;
}