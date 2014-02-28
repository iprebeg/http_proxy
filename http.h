#include <wrap.h>

#ifndef HTTP_PROXY_HEADER
#define HTTP_PROXY_HEADER


#define MAX_CONS 900
#define MAXLEN 8096
#define MSG_MAX 4096
#define TIMEOUT_SEC 5

#define BAD_REQUEST  403 
#define FORBIDDEN    405
#define NOT_FOUND    404
#define CORRUPTED    444

/*
const char * http_get = "GET";
const char * http_11  = "HTTP/1.1";
const char * http_10  = "HTTP/1.0";
*/

#ifndef MSG_NOSIGNAL
  #define MSG_NOSIGNAL SO_NOSIGPIPE
#endif

void process(int cs,struct sockaddr_in * cli);

/* when using threads instead of fork */

struct args_p {
  int cs;
  struct sockaddr_in * cli;
  pthread_t tid;
};

void * process_p ( void * arg);


/* preparing for optimized connection management */

struct con {
  unsigned int active;
  unsigned int fd;
  struct in_addr addr;
};

#endif
