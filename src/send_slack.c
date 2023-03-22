#include "send_slack.h"

#ifdef USE_SLACK
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

CURL *curl;
CURLcode res;

void init_send_slack() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed: %s\naborting\n",
                curl_easy_strerror(res));
        curl_global_cleanup();
    }
}

void send_message(char *msg) {
    char *post = malloc(strlen(msg) + 13);
    memset(post, 0, strlen(msg) + 13);
    curl_easy_setopt(curl, CURLOPT_URL, SLACK_WEBHOOK_URL);
    strcat(post, "{'text':'");
    strcat(post, msg);
    strcat(post, "'}");
    // printf("%s\n", post);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
}
#endif
