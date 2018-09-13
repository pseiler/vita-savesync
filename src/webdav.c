#define VITASDK

#include <psp2/io/stat.h> 
#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/display.h>
 
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>
 
#include <psp2/io/fcntl.h>

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include <curl/curl.h>

struct stringcurl {
  char *ptr;
  size_t len;
}; 
void init_string(struct stringcurl *s) {
  s->len = 0;
  s->ptr = (char*)malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    return;
    //exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct stringcurl *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = (char*)realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    return 0;
    //exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

static size_t write_data_to_disk(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = sceIoWrite(   *(int*) stream , ptr , size*nmemb);
  return written;
}
static size_t read_data_from_disk(void *ptr, size_t size, size_t nmemb, void *stream) {
  size_t read = sceIoRead( * (int*) stream, ptr, size*nmemb);
  return read;
}

char *user_agent = "libcurl (Playstation Vita/VitaSDK)";

void webdavDownloadFile(char * url, char * file, char * userpwd){
    int writeFD = sceIoOpen( file, SCE_O_WRONLY | SCE_O_CREAT, 0777);
    if(!writeFD){
        return;
    }

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
//        struct stringcurl body;
//        init_string(&body);
        struct stringcurl header;
        init_string(&header);
        // Set useragant string
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
        // not sure how to use this when enabled
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        // not sure how to use this when enabled
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        // Set SSL VERSION to TLS 1.2
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        // Set timeout for the connection to build
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        // Follow redirects (?)
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // The function that will be used to write the data 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_disk);
        // The data filedescriptor which will be written to
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeFD);
        // set the login user + password
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
        // write function of response headers
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
        // the response header data where to write to
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
        // Request Header :
//        struct curl_slist *headerchunk = NULL;
//        headerchunk = curl_slist_append(headerchunk, "Accept: */*");
//        headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
//        headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
//        //headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");  Setting this will lead to errors when trying to download. should be set depending on location : possible : cdn.discordapp.com or images.discordapp.com
//        headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
//        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);


        // Perform the request
        res = curl_easy_perform(curl);
        int httpresponsecode = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);

        if(res != CURLE_OK){
            //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        }else{

        }

    }else{

    }

    // close filedescriptor
    sceIoClose(writeFD);

    // cleanup
    curl_easy_cleanup(curl);

}

void webdavUploadFile(char * url , char * file, char * userpwd){
    int readFD = sceIoOpen( file, SCE_O_RDONLY | SCE_O_CREAT, 0777);
    if(!readFD){
        return;
    }

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
//        struct stringcurl body;
//        init_string(&body);
        struct stringcurl header;
        init_string(&header);
//      SSL stuff 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//      HTTP options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_data_from_disk);
        curl_easy_setopt(curl, CURLOPT_READDATA, &readFD);
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);


        // Perform the request
        res = curl_easy_perform(curl);
        int httpresponsecode = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);

        if(res != CURLE_OK){
            //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        }else{

        }


    }else{

    }
    // close filedescriptor
    sceIoClose(readFD);

    // cleanup
    curl_easy_cleanup(curl);
}

void webdavCreateDir(char * url, char * userpwd){

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
//        struct stringcurl body;
//        init_string(&body);
        struct stringcurl header;
        init_string(&header);
//      SSL stuff 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//      HTTP options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "MKCOL");
        // the response header data where to write to
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);


        // Perform the request
        res = curl_easy_perform(curl);
        int httpresponsecode = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);

        if(res != CURLE_OK){
            //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        }else{

        }


    }else{

    }
    // cleanup
    curl_easy_cleanup(curl);
}

int webdavMove(char * url_old, char * url_new,  char *userpwd){

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
//        struct stringcurl body;
//        init_string(&body);
        struct stringcurl header;
        init_string(&header);
//      SSL stuff 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
//      HTTP options
        curl_easy_setopt(curl, CURLOPT_URL, url_old);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "MOVE");
        // the response header data where to write to
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
        // Request Header :
        char * dest_header_chunk = malloc(strlen("Destination:") + strlen(url_new) + 1);
        strcpy(dest_header_chunk, "Destination:");
        strcat(dest_header_chunk, url_new);
        struct curl_slist *headerchunk = NULL;
        headerchunk = curl_slist_append(headerchunk, dest_header_chunk);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
//        headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
//        headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
//        //headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");  Setting this will lead to errors when trying to download. should be set depending on location : possible : cdn.discordapp.com or images.discordapp.com
//        headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");

        // Perform the request
        res = curl_easy_perform(curl);

        if(res != CURLE_OK){
            //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        }
    }
        int httpresponsecode = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);
    // cleanup
    curl_easy_cleanup(curl);
        return httpresponsecode;
}

int webdavTest(char * url, char * userpwd){

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
//        struct stringcurl body;
//        init_string(&body);
        struct stringcurl header;
        init_string(&header);
//      SSL stuff 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
//      HTTP options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_disk);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PROPFIND");
        // the response header data where to write to
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);

        // Perform the request
        res = curl_easy_perform(curl);
        int httpresponsecode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);

        if(res != CURLE_OK){
            //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        }else{
        }
        return httpresponsecode;

    }else{

    }
    // cleanup
    curl_easy_cleanup(curl);
}
