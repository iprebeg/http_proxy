#include <wrap.h>

#ifndef HTTP_PROXY_HEADER
#define HTTP_PROXY_HEADER


#define MAX_CONS 30
#define MAXLEN 8096
#define MSG_MAX 4096
#define TIMEOUT_SEC 30

#define BAD_REQUEST  403 
#define FORBIDDEN    405
#define NOT_FOUND    404
#define CORRUPTED    444

/*
const char * http_get = "GET";
const char * http_11  = "HTTP/1.1";
const char * http_10  = "HTTP/1.0";
*/

void process(int cs,struct sockaddr_in * cli);

/* when using threads instead of fork */

struct args_p {
  int cs;
  struct sockaaddr_in * cli;
};

void * process_p ( struct args_p * );


/* preparing for optimized connection management */

struct con {
  unsigned int active;
  unsigned int fd;
  struct in_addr addr;
};

#endif
