#include "include/requests.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    Response* response = (Response*)userp;
    response->content = realloc(response->content, strlen(response->content) + total_size + 1);
    strcat(response->content, contents);
    return total_size;
}

Response* requests_get(const char* url) {
    CURL* curl = curl_easy_init();
    Response* response = malloc(sizeof(Response));
    response->content = calloc(1, sizeof(char));

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->status_code);
        curl_easy_cleanup(curl);
    }
    return response;
}
