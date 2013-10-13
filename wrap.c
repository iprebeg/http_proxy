#include <wrap.h>

unsigned int
Socket(int domain, 
       int type,
       int protocol)
{
  int sockfd;
  if ((sockfd = socket(domain,
                       type,
                       protocol)) == -1)
  {
    perror("socket");
    exit(1);
  }
  else
    return sockfd;
}

unsigned int
Socket_TCP ()
{
  int sockfd;
  if ((sockfd = socket(PF_INET,
                       SOCK_STREAM,
                       0x0)) == -1)
  {
    perror("socket");
    exit(1);
  }
  else
    return sockfd;
}

unsigned int
Bind (const int sockfd,
      struct sockaddr * addr,
      socklen_t addrlen)
{
  if (bind(sockfd,
           addr,
           addrlen) == -1)
  {
    perror("bind");
    exit(2);
  }
  else 
    return 0;
}

unsigned int
Listen (const int sockfd,
        const int backlog)
{
  if (listen(sockfd,
             backlog))
  {
    perror("listen");
    exit(3);
  }
  else
    return 0;
}

unsigned int 
Accept(const unsigned int listen_sfd,
       struct sockaddr_in * addr,
       socklen_t * addrlen)
{
  int connect_sfd;
  if ((connect_sfd = accept(listen_sfd,
                            (struct sockaddr *)addr,
                            addrlen)) == -1) // could wrap addrlen tough
  {
    perror("accept");
    exit(4);
  }
  else
    return connect_sfd;
}


/* eeeek, seek for proper way of wrapping 
 * fork() system call... HINT: Stevens likes
 * wrapping system calls.
 */
unsigned int 
Fork()
{
  int ret;
  if ((ret = fork()) == -1)
  {
     perror("fork");
     exit(5);
  }
  else
    return ret;

  
}

unsigned int 
Close(const int sockfd)
{
  if (close(sockfd) == -1)
  {
    perror("close");
    exit(1);
  }
  else
    return 0;
}

unsigned int 
Recv(const int sockfd,
     void * buf,
     const int len,
     const int flags)
{
  ssize_t n;
  if ((n = recv(sockfd,buf,len,flags)) == -1)
  {
    perror("recv");
    exit(6);
  }
  else
    return n;
}

unsigned int
Send (const int sockfd,
      const void * buffer,
      const int len,
      int flags)
{
  ssize_t n;
  if ((n = send(sockfd,buffer,len,flags)) == -1)
  {  
    perror("send");
    exit(7);
  }
  else
    return n;
}

unsigned int 
Connect (const int sockfd,
         struct sockaddr_in *addr,
         socklen_t addrlen)
{
  if (connect(sockfd,
              (struct sockaddr *)addr,
              addrlen) == -1)
  {
    perror("connect");
    exit(8);
  }
  else
    return 0;
}

unsigned int 
Getaddrinfo (char * addr, 
             char * port, 
             struct addrinfo * hints, 
             struct addrinfo ** res)
{
  int error;
  if ((error = getaddrinfo(addr, port, hints, res)) != 0)
    errx(9, "%s", gai_strerror(error));
  else
    return 0;
}

unsigned int 
Getnameinfo (const struct sockaddr_in *addr, 
             const int salen,
             char * host, int hsize,
             char * service, int ssize)
{
  int error;
  if ((error = getnameinfo((struct sockaddr *)addr,salen,
                            host, hsize,
                            service, ssize,
                            NI_NOFQDN | NI_NUMERICSERV)) != 0)
    errx(10, "%s", gai_strerror(error));
  else
    return 0;
}

/* FreeBSD specific
unsigned int
Sendfile(const int fd,
         const int sockfd)
{
  if (sendfile (fd,     // descriptor of file to be sent
                sockfd, // socket
                0x0,    // start from beginning
                0x0,    // read while EOF is reached
                NULL,   // no headers or trailers
                NULL,   // no offset either
                0x0)    // would not use EBUSY
                == -1)
  {
    perror("sendfile");
    exit(11);
  }
  else
    return 0;
}
*/

unsigned int 
Open(const char * f_name,
     const int flags)
{
  int fd;
  if ((fd = open(f_name,flags)) == -1)
  {
    perror("open");
    exit(12);
  }
  else 
    return fd;
}

unsigned int 
Fchmod(const int fd,
       const int mode)
{
  if (fchmod(fd,mode) == -1)
  {
    perror("fchmod");
    exit(13);
  }
  else
    return 0;
}

unsigned int 
Write(int fd,
      const void * buffer,
      size_t nbytes)
{
  int n;
  if ((n = write(fd,buffer,nbytes)) == -1)
  {
    perror("write");
    exit(14);
  }
  else
    return n;
}

unsigned int 
Read(int fd,
     void * buffer,
     ssize_t nbytes)
{
  int n;
  if ((n = read(fd,buffer,nbytes)) == -1)
  {
    perror("read");
    exit(15);
  }
  else
    return n;
}

unsigned int 
Recvfrom(int s,
         void * buf,
         size_t len,
         int flags,
         struct sockaddr *from,
         socklen_t *fromlen)
{
  int n;
  if ((n = recvfrom(s,buf,len,flags,from,fromlen)) == -1)
  {
    perror("recvfrom");
    exit(16);
  }
  else
    return n;
}

unsigned int
Sendto(int s,
       const void * buf,
       size_t len,
       int flags,
       const struct sockaddr *to,
       socklen_t tolen)
{
  int n;
  if ((n = sendto(s,buf,len,flags,to,tolen)) == -1)
  {
    perror("sendto");
    exit(17);
  }
  else
    return n;

}

void * 
Malloc(size_t size)
{
  void * ptr;
  if ( (ptr = malloc(size)) == NULL)
  {
    perror("malloc");
    exit(18);
  }
  else
    return ptr;
}

int
Setsockopt( int s,
            int level,
            int optname,
            const void *optval,
            socklen_t optlen)
{
  int ret;
  if ( ( ret = setsockopt (s,level,optname,optval,optlen) ) < 0 ) {
    perror ("setsockopt");
    exit(18);
  } else 
    return ret;
}
