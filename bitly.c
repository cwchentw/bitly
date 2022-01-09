#define _GNU_SOURCE  /* For asprintf(3). */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<curl/curl.h>
#include<json-c/json.h>

#define BASEURL "https://api-ssl.bitly.com"
#define DOMAIN "bit.ly"


int get_json(char const *outfile, char const *long_url, char const *token);
int get_url(char const *file, char *short_url);

int main(int argc, char *argv[])
{
    char *token = getenv("BITLY_TOKEN");
    if (!token) {
        fprintf(stderr, "No Bitly Token\n");
        return 1;
    }

    if (argc < 2) {
        fprintf(stderr, "No URL\n");
        return 1;
    }
    
    char *outfile = "/tmp/output.json";
    char *long_url = NULL;
    char short_url[256];

    asprintf(&long_url, "%s", argv[1]);

    if (get_json(outfile, long_url, token)) {
        fprintf(stderr, "Failed connection\n");
        goto ERROR;
    }

    if (get_url(outfile, short_url)) {
        fprintf(stderr, "Unable to get url\n");
        goto ERROR;
    }

    printf("%s\n", short_url);

    if(remove(outfile)) {
        fprintf(stderr, "Unable to delete %s\n", outfile);
        goto ERROR;
    }

    free(long_url);

    return 0;

ERROR:
    if (long_url)
      free(long_url);

    return 1;
}

int get_json(char const *outfile, char const *long_url, char const *token)
{
    FILE *json_file = NULL;
    CURL *curl = NULL;
    char *query_url = NULL;
  
    json_file = fopen(outfile, "w");
    if (!json_file)
        goto ERROR;

    if (curl_global_init(CURL_GLOBAL_DEFAULT))
        goto ERROR;

    curl = curl_easy_init();
    if (!curl)
        goto ERROR;

    asprintf(
        &query_url,
        "%s/v3/shorten?access_token=%s&longUrl=%s&domain=%s",
	      BASEURL, token, long_url, DOMAIN
    );

    curl_easy_setopt(curl, CURLOPT_URL, query_url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, json_file);

    CURLcode res = curl_easy_perform(curl);
    if (res)
      goto ERROR;

    free(query_url);
    curl_easy_cleanup(curl);
    fclose(json_file);

    curl_global_cleanup();

    return 0;

ERROR:
    if (query_url)
        free(query_url);

    if (curl)
        curl_easy_cleanup(curl);

    if (json_file)
        fclose(json_file);

    curl_global_cleanup();

    return 1;
}

int get_url(char const *file, char *short_url)
{
    json_object *json = NULL;
    json = json_object_from_file(file);
    if (!json)
        return 1;

    json_object * json_status = json_object_object_get(json, "status_code");
    const char *status_code = json_object_get_string(json_status);
    int status = atoi(status_code);
    if (status != 200)
        goto ERROR;

    json_object *json_data = json_object_object_get(json, "data");
    json_object *json_url = json_object_object_get(json_data, "url");
    const char *url = json_object_get_string(json_url);
    strcpy(short_url, url);

    json_object_put(json);

    return 0;

ERROR:
    if (json)
        json_object_put(json);

    return 1;
}
