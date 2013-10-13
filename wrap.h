#include <stdio.h>

/* close()  */
/* fork()   */
#include <unistd.h>

/* exit()   */
/* strtoul()*/
#include <stdlib.h>

/* socket() */
/* bind()   */
/* listen() */
/* accept() */
/* fork()   */
/* recv()   */
/* send()   */
/* inet_ntoa() */
/* getaddrinfo() */
/* gai_strerror() */
/* getnameinfo() */
/* sendfile() */
#include <sys/types.h>
#include <sys/socket.h>

/* struct sockaddr_in */
/* inet_ntoa() */
#include <netinet/in.h>

/* memset() */
#include <string.h>

/* strtoul()*/
#include <limits.h>

/* inet_ntoa() */
#include <arpa/inet.h>

/* getaddrinfo() */
/* gai_strerror() */
/* getnameinfo() */
#include <netdb.h>

/* errx() */
#include <err.h>

/* sendfile() */
#include <sys/uio.h>

/* open() */
#include <fcntl.h>

/* fchmod() */
#include<sys/stat.h>

/* waitpid() */
#include <sys/wait.h>

/* sigaction */
#include <signal.h>

/* errno */
#include <errno.h>

/* openlog() */
/* syslog() */
#include <syslog.h>

/* signal() */
/* sigaction() */
#include <signal.h>


#ifndef WRAP
#define WRAP

unsigned int
Socket(int domain, 
       int type,
       int protocol);

unsigned int
Socket_TCP ();

unsigned int
Bind (const int sockfd,
      struct sockaddr * addr,
      socklen_t addrlen);

unsigned int
Listen (const int sockfd,
        const int backlog);

unsigned int 
Accept(const unsigned int listen_sfd,
       struct sockaddr_in * addr,
       socklen_t * addrlen);

unsigned int 
Fork();

unsigned int 
Close(const int sockfd);

unsigned int 
Recv(const int sockfd,
     void * buf,
     const int len,
     const int flags);

unsigned int
Send (const int sockfd,
      const void * buffer,
      const int len,
      int flags);

unsigned int 
Connect (const int sockfd,
         struct sockaddr_in *addr,
         socklen_t addrlen);

unsigned int 
Getaddrinfo (char * addr, 
             char * port, 
             struct addrinfo * hints, 
             struct addrinfo ** res);

unsigned int 
Getnameinfo (const struct sockaddr_in *addr, 
             const int salen,
             char * host, int hsize,
             char * service, int ssize);

unsigned int 
Open(const char * f_name,
     const int flags);

unsigned int 
Fchmod(const int fd,
       const int mode);

unsigned int 
Write(int fd,
      const void * buffer,
      size_t nbytes);

unsigned int 
Read(int fd,
     void * buffer,
     ssize_t nbytes);

unsigned int 
Recvfrom(int s,
         void * buf,
         size_t len,
         int flags,
         struct sockaddr *from,
         socklen_t *fromlen);

unsigned int
Sendto(int s,
       const void * buf,
       size_t len,
       int flags,
       const struct sockaddr *to,
       socklen_t tolen);

void * 
Malloc(size_t size);

int
Setsockopt( int s,
            int level,
            int optname,
            const void *optval,
            socklen_t optlen);
#endif
