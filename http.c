#include <wrap.h>
#include <http.h>
#include <pthread.h>

void send_bad_response(int cs,int code,char * inv_req)
{
  char * msg;
  
  switch (code) {
  case FORBIDDEN: 
    msg = ("HTTP/1.1 403 forbidden\r\nConnection: close\r\n\r\nForbidden\n"); // 403
    break;
  case BAD_REQUEST:
    msg = ("HTTP/1.1 405 Method not allowed\r\nConnection: close\r\n\r\nMethod not supported\n");  // 405
    break;
  case NOT_FOUND:
    msg = ("HTTP/1.1 404 Not found\r\nConnection: close\r\n\r\nNot Found\n");  // 404
    break;
  case CORRUPTED:
    /* FALLTHROUGH */
  default:
    msg = ("HTTP/1.1 444 Corrupted\r\nConnection: close\r\n\r\nRequest is corrupted\n");  // 444
    break;
  }

  Send(cs,msg,strlen(msg),0);
  Close (cs);
}

int create_connection( char * host_r)
{

#ifdef DEBUG
  printf ("Resolving [%s]\n ",host_r);
#endif

  // Resolve hostname
  struct addrinfo hints,*res;
  int error;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_flags |= AI_CANONNAME;
  if ( (error = getaddrinfo (host_r,NULL,&hints,&res)) != 0) {
    fprintf (stderr,"getaddrinfo: %s\n",gai_strerror(error));
    return -1;
  }
  free(host_r);

#ifdef DEBUG
  printf ("Resolved\n ");
#endif


  // host Socket
  int hs;
  hs = Socket(PF_INET, SOCK_STREAM, 0x0);
 
 
  // host address
  struct sockaddr_in hostaddr;
  hostaddr.sin_family = AF_INET;
  hostaddr.sin_port = htons(80);
  hostaddr.sin_addr = ((struct sockaddr_in *)res->ai_addr)->sin_addr;
  memset(&hostaddr.sin_zero,0x0,sizeof(hostaddr.sin_zero));

  freeaddrinfo(res); // free getaddrinfo stuff

  // connect
  if (connect(hs,(struct sockaddr *)&hostaddr,sizeof(hostaddr)) == -1) {
    perror("connect");
    return -1;
  }

  return hs;
}


void * process_p ( void * arg)
{
  fprintf (stdout, "==== start in thread %u ====\n", (unsigned int)pthread_self());
  struct args_p a = *((struct args_p*)arg);  
  process(a.cs, a.cli);
  fprintf (stdout, "==== done in thread %u ====\n", (unsigned int)pthread_self());
  return NULL;
}

void process(int cs,struct sockaddr_in * cli)
{
  int n;
  unsigned int i;
  char buf[MAXLEN +1];

  char sbuf[MAXLEN+1];
  unsigned int sbuf_offset = 0;
  char * con = "Connection: close\r\n";

  char * req;
  char * tok;

  int bad_req = 0;
  int not_allowed = 0;
  int corrupted = 0;

  char * host_s;
  char * host_e;
  char * host_r;

  bzero(buf,MAXLEN +1);
  bzero(sbuf,MAXLEN +1);

  n = Recv(cs,buf,MAXLEN - 1,0);

#ifdef DEBUG
  printf ("RECEIVED\n====================\n%s====================\n",buf);
#endif

  tok = strtok(buf,"\n");
  for (i=0; tok != NULL; i++) {

#ifdef DEBUG_DEEP
    printf ("%s\n",tok);
#endif

    if (strstr(tok,"Keep-Alive:") != NULL)
      goto next;

    if (strstr(tok,"Connection:") != NULL) {
      memcpy(sbuf+sbuf_offset,con,strlen(con));
      sbuf_offset += strlen(con);
    } else {
      memcpy(sbuf+sbuf_offset,tok,strlen(tok));
      sbuf_offset += strlen(tok);
      memcpy(sbuf+sbuf_offset,"\n",1);
      sbuf_offset += 1;
    }

    if (i == 0) {
      if (strncmp(tok,"GET",3)) {
        printf ("KRIVO ");
        bad_req = 1;
        req = tok; // request  // does this fix anything
      } else
        req = tok; // request 

      // sometimes, you will get your fingers broken
      // for those litlle ugly dirty hacks

      // extract host from request
      // when using proxy, no relative path can be used 
      host_s = strstr(tok,"//");
      if (host_s == NULL) {
        corrupted = 1;
        break;
        //continue;
      }

      host_e = strstr(host_s+2,"/");
      if (host_e == NULL) {
        corrupted = 1;
        break;
        //continue;
      }

      host_r = calloc(1,host_e - host_s - 2 + 1); //// FREEEEEEEEEEEEE MEEEEEEEE SOMETIMES
      if (host_r == NULL) {
        // this is actually error on server side, but who gives a fuck
        corrupted = 1;
        break;
        //continue;
      }

      strncpy(host_r,host_s + 2,host_e - host_s - 2);

#ifdef DEBUG
      printf ("HOST: %s\n",host_r);
#endif

    }

    //if (!strncmp(tok,"Host:",4))
    //  host_line = tok;

    next:
    tok = strtok(NULL,"\n");
  }

  

  if (corrupted) {
    send_bad_response(cs,CORRUPTED,req);
    return;
  }

  /*
  tok = strtok(host_line," ");
  if (tok == NULL)
  {
    printf ("CORRUPTED REQUEST\n");
    return;
  }
  tok = strtok(NULL," ");
  if (tok == NULL)
  {
    printf ("CORRUPTED REQUEST\n");
    return;
  }

  for (i=0; tok[i] != '\r' && i<256; i++)
    host[i] = tok[i];
  host[i] = '\0';
  */

  extern char ** filters;
  extern unsigned int filters_cnt;

  for (i=0; i<filters_cnt; i++) {
    if (strstr(host_r,filters[i]) != NULL) {
      printf ("FILTERED ");
      not_allowed = 1;
    }
  }

  

  tok = strtok(req," ");
  if (tok == NULL) {
    printf ("CORRUPTED REQUEST\n");
    return;
  }
  else
    printf ("%s ",tok);
  
  tok = strtok(NULL," ");
  if (tok == NULL) {
    printf ("CORRUPTED REQUEST\n");
    return;
  }
  else
    printf ("%s\n",tok);

  if (bad_req) {
    send_bad_response(cs,BAD_REQUEST,req);
    return;
  }

  if (not_allowed) {
    send_bad_response(cs,FORBIDDEN,NULL);
    return;
  }

  int hs;
  if ( (hs = create_connection(host_r)) < 0) {
    send_bad_response(cs,NOT_FOUND,NULL); // testing
    return;
  }

#ifdef DEBUG
  printf ("connected\n ");
#endif

  struct timeval tv;
  tv.tv_sec = TIMEOUT_SEC;
  tv.tv_usec = 0;

  socklen_t tvlen = sizeof(tv);
  int recvd;

  Send(hs,sbuf,sbuf_offset,0x0);

#ifdef DEBUG
  printf ("FORWARDING\n====================\n%s====================\n",sbuf);
  printf ("forwarded request\n");
#endif

  for ( ; ; ) {
    bzero(sbuf,MAXLEN);
    setsockopt(hs,SOL_SOCKET,SO_RCVTIMEO,&tv,tvlen);
    recvd = recv(hs,sbuf,MAXLEN,0);

#ifdef DEBUG
    printf ("got %d bytea\n",recvd);
#endif

    if (recvd == -1) {
      perror ("recv");
      if (errno == EWOULDBLOCK)
        fprintf (stderr,"timeouted\n");
      break;
    }
    else if (recvd == 0) {
     // Send(cs,sbuf,recvd,0x0); // try to remove this
     
#ifdef DEBUG
      printf ("done\n");
#endif

      break;
    }

    /* IGNORING SIGNALS TO NOT TO DIE ON EPIPE */
    if ( ( n = send(cs,sbuf,recvd,MSG_NOSIGNAL) ) == -1 ) {
      perror ("send");
      break;
    }

#ifdef DEBUG
    printf ("returned to client\n");
#endif

  }

  Close(hs);
  Close(cs);

  return;
}
