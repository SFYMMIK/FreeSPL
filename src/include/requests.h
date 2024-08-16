#ifndef REQUESTS_H
#define REQUESTS_H

typedef struct {
    int status_code;
    char* content;
} Response;

Response* requests_get(const char* url);
Response* requests_post(const char* url, const char* data);

#endif
