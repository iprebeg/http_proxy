#include <wrap.h>
#include <http.h>
#include <pthread.h>

char ** filters;
unsigned int filters_cnt = 0;

void usage(char * bin_name)
{
  printf ("usage: %s port filter1 ... filterN\n",bin_name);
  exit(1);
}

int main (int argc, char * argv[])
{
  char opt;
  const char * optstring  = "";
  unsigned int i;
  unsigned int cnt;
  struct addrinfo hints,*res;
  struct sockaddr_in addr;
  int backlog,cs,s,on;
  struct sockaddr_in cli;

  while ((opt=getopt(argc,argv,optstring)) != -1) {
    switch(opt) {
    default : usage(argv[0]);
    }
  }

  // resolve service
  if (argv[optind] == NULL)
    usage(argv[0]);

  bzero(&hints,sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  Getaddrinfo(NULL,argv[optind],&hints,&res);

  // process filters
  cnt = optind + 1;

  while (argv[cnt++] != NULL)
    filters_cnt++;

  filters = Malloc(filters_cnt * sizeof (char *));
  for (cnt = 0, i=optind + 1; i <= optind + filters_cnt; i++, cnt++)
    filters[cnt] = argv[i];
  
  // prepare address
  addr.sin_family = AF_INET;
  addr.sin_port = ((struct sockaddr_in *)(res->ai_addr))->sin_port;
  addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&addr.sin_zero,sizeof(addr.sin_zero)); 
  
  // free getaddrinfo stuff
  freeaddrinfo(res);

  // socket
  s = Socket(PF_INET, SOCK_STREAM, 0x0);

  // enable address reuse ( not TCP wait crap )
  on = 1;
  Setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  // bind 
  Bind(s,(struct sockaddr *)&addr,sizeof(addr));

  // listen
  socklen_t clilen = sizeof(cli);
  backlog = 10;
  Listen(s,backlog);

  for ( ; ; ) {
    cs = Accept(s,&cli,&clilen);

#ifdef PREFORK
    if ( Fork() ) {
      printf ("%s: ",inet_ntoa(cli.sin_addr));
    } else {
      process(cs,&cli);
      return 0;
    }
#else
    struct args_p *arg = (struct args_p*)malloc(sizeof(struct args_p));
    arg->cs = cs;
    arg->cli = &cli;
    pthread_create(&arg->tid, NULL, &process_p, arg);
    pthread_detach(arg->tid);
#endif

  }
  
  /* NOTREACHED */
  Close (s);
  return 0;
}
