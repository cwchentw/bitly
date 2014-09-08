#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<json/json.h>
#include "token.h"

#define BASEURL "https://api-ssl.bitly.com"
#define DOMAIN "bit.ly"

int
get_json(char const *outfile, char const *long_url) {
  FILE *json_file = fopen(outfile, "w");
  char *query_url;
  if (!json_file) {
    return -1;
  }

  CURL *curl = curl_easy_init();
  if (!curl) {
    return -1;
  }
  asprintf(&query_url, "%s/v3/shorten?access_token=%s&longUrl=%s&domain=%s",
	   BASEURL, TOKEN, long_url, DOMAIN);
  curl_easy_setopt(curl, CURLOPT_URL, query_url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, json_file);
  free(query_url);
  CURLcode res = curl_easy_perform(curl);
  if (res) {
    return -1;
  }

  curl_easy_cleanup(curl);
  fclose(json_file);
  return 0;
}

int
get_url(char const *file, char *short_url) {
  json_object *json = json_object_from_file(file);
  json_object * json_status = json_object_object_get(json, "status_code");
  const char *status_code = json_object_get_string(json_status);
  int status = atoi(status_code);
  if (status != 200) {
    json_object_put(json_status);
    json_object_put(json);
    return -1;
  }

  json_object *json_data = json_object_object_get(json, "data");
  json_object *json_url = json_object_object_get(json_data, "url");
  const char *url = json_object_get_string(json_url);
  strcpy(short_url, url);

  json_object_put(json_url);
  json_object_put(json_data);
  json_object_put(json_status);
  json_object_put(json);
  return 0;
}

int
main(int argc, char *argv[]) {
  char *outfile = "/tmp/output.json";
  char *long_url;
  char short_url[256];
  if (argc < 2) {
    return 0;
  }

  asprintf(&long_url, "%s", argv[1]);

  if (get_json(outfile, long_url)) {
    fprintf(stderr, "failed connection\n");
    free(long_url);
    return 1;
  }

  if (get_url(outfile, short_url)) {
    fprintf(stderr, "unable to get url\n");
    free(long_url);
    return 1;
  }
   
  printf("%s\n", short_url);

  free(long_url);
  if(remove(outfile)) {
    fprintf(stderr, "unable to delete %s\n", outfile);
    return 1;
  }
  return 0;
}
