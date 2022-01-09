#define _GNU_SOURCE  /* For asprintf(3). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <tchar.h>
#endif

#include <curl/curl.h>
#include <json-c/json.h>

#define BASEURL "https://api-ssl.bitly.com"
#define DOMAIN "bit.ly"


int get_json(char const *outfile, char const *long_url, char const *token);
int get_url(char const *file, char *short_url);

int main(int argc, char *argv[])
{
    /* Check whether a Bitly token is available. */
    char *token = getenv("BITLY_TOKEN");
    if (!token) {
        fprintf(stderr, "No Bitly Token\n");
        return 1;
    }

    /* Check whether an URL is available. */
    if (argc < 2) {
        fprintf(stderr, "No URL\n");
        return 1;
    }

#if _WIN32
    /* FIXME: Create a random temporary file name. */
    char *outfile = NULL;

    char *tempPath = getenv("TEMP");
    if (!tempPath) {
        fprintf(stderr, "Unable to get temporary directory\n");
        return 1;
    }

    asprintf(&outfile, "%s\\%s", tempPath, "output.json");
#else
    /* FIXME: Create a random temporary file name. */
    char *outfile = "/tmp/output.json";
#endif
    char *long_url = NULL;
    char short_url[256];

    asprintf(&long_url, "%s", argv[1]);

    /* Query Bitly to receive a JSON data. */
    if (get_json(outfile, long_url, token)) {
        fprintf(stderr, "Failed connection\n");
        goto ERROR_MAIN;
    }

    /* Parse the JSON data to get a shortened URL. */
    if (get_url(outfile, short_url)) {
        fprintf(stderr, "Unable to get url\n");
        goto ERROR_MAIN;
    }

    printf("%s\n", short_url);

    /* Remove the temporary file. */
    if(remove(outfile)) {
        fprintf(stderr, "Unable to delete %s\n", outfile);
        goto ERROR_MAIN;
    }

    free(long_url);

    return 0;

ERROR_MAIN:
    if (long_url)
      free(long_url);

    if(remove(outfile)) {
        fprintf(stderr, "Unable to delete %s\n", outfile);
    }

    return 1;
}

int get_json(char const *outfile, char const *long_url, char const *token)
{
    FILE *json_file = NULL;
    CURL *curl = NULL;
    char *query_url = NULL;
  
    json_file = fopen(outfile, "w");
    if (!json_file) {
        fprintf(stderr, "Unable to create a JSON file\n");
        goto ERROR_CURL;
    }

    /* Initialize CURL. Call it before creating any CURL object. */
    if (curl_global_init(CURL_GLOBAL_DEFAULT)) {
        fprintf(stderr, "Unable to initialize CURL\n");
        goto ERROR_CURL;
    }

    /* Create a CURL object. */
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Unable to create a CURL object\n");
        goto ERROR_CURL;
    }

    asprintf(
        &query_url,
        "%s/v3/shorten?access_token=%s&longUrl=%s&domain=%s",
	    BASEURL, token, long_url, DOMAIN
    );

    curl_easy_setopt(curl, CURLOPT_URL, query_url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, json_file);

    /* Query Bitly with CURL to get a JSON data, which may fail. */
    /* FIXME: Unable to query bitly.com on Windows. */
    CURLcode res = curl_easy_perform(curl);
    if (res) {
        fprintf(stderr, "Failed to query bitly.com\n");
        goto ERROR_CURL;
    }

    free(query_url);
    curl_easy_cleanup(curl);
    fclose(json_file);

    /* Clean up CURL. Call it after other CURL functions. */
    curl_global_cleanup();

    return 0;

ERROR_CURL:
    if (query_url)
        free(query_url);

    if (curl)
        curl_easy_cleanup(curl);

    if (json_file)
        fclose(json_file);

    /* Clean up CURL. Call it after other CURL functions. */
    curl_global_cleanup();

    return 1;
}

int get_url(char const *file, char *short_url)
{
    json_object *json = NULL;

    /* Create a JSON object. */
    json = json_object_from_file(file);
    if (!json)
        return 1;

    /* Check whether the HTTP status code is 200 (OK). */
    json_object * json_status = json_object_object_get(json, "status_code");
    /* Convert a JSON object to a C string which
        represents a HTTP status code. */
    const char *status_code = json_object_get_string(json_status);
    int status = atoi(status_code);
    if (status != 200)
        goto ERROR_JSON;

    /* Get a shortened URL. */
    json_object *json_data = json_object_object_get(json, "data");
    json_object *json_url = json_object_object_get(json_data, "url");
    /* Convert a JSON object to a C string. */
    const char *url = json_object_get_string(json_url);
    strcpy(short_url, url);

    /* Release the JSON object. */
    json_object_put(json);

    return 0;

ERROR_JSON:
    if (json)
        json_object_put(json);

    return 1;
}
