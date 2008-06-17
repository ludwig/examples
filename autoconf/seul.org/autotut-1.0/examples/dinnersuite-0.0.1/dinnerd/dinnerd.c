/* 
   dinnerd : dinner daemon
   implements the dinning hackers protocol
   (C) 2001 Felipe Bergo <bergo@seul.org> 
   Distributed under the terms of the GNU General
   Public License, version 2 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config.h"

#ifndef HAVE_socklen_t
typedef int socklen_t;
#endif

int TcpPort=8081;
int TcpPort2=8082;
int ListenSocket, ListenSocket2;
int amdaemon=1;

#define MAX_CLIENTS 256

int clients[MAX_CLIENTS];
int watchers[MAX_CLIENTS];
int ClientCount, WatcherCount;

struct Hacker {
  char name[16];
  int ST;   /* strength            */
  int HT;   /* health              */
  int DX;   /* dexterity (dodge)   */
  int Aggr; /* aggressiveness      */
  int LChop;
  int RChop;
  time_t LastAte;
} hackords[MAX_CLIENTS];

void show_msg(char *msg) {
  if (!amdaemon)
    printf("%s\n",msg);
}

void init() {
  int i;
  srand(time(0)/getpid());
  for(i=0;i<MAX_CLIENTS;i++) {
    clients[i]=0;
    watchers[i]=0;
  }
  ClientCount=0;
  WatcherCount=0;
}

void daemonize() {
  if (fork()>0)
    exit(0);
  chdir("/");
  umask(0);
  setsid();
}

void init_listen_socket() {
  struct sockaddr_in sin;

  ListenSocket=socket(AF_INET,SOCK_STREAM,0);
  fcntl(ListenSocket,F_SETFL,O_NONBLOCK);

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(TcpPort);
  
  if (bind(ListenSocket,(struct sockaddr *) &sin,sizeof(sin))==-1) {
    fprintf(stderr,"bind failed on port %d\n",TcpPort);
    exit(2);
  }

  listen(ListenSocket,5);

  /* second listen port -- watching */

  ListenSocket2=socket(AF_INET,SOCK_STREAM,0);
  fcntl(ListenSocket2,F_SETFL,O_NONBLOCK);

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(TcpPort2);
  
  if (bind(ListenSocket2,(struct sockaddr *) &sin,sizeof(sin))==-1) {
    fprintf(stderr,"bind failed on port %d\n",TcpPort2);
    exit(2);
  }

  listen(ListenSocket2,5);
}

int roll_d6() {
  int j;
  j=1+(int) (6.0*rand()/(RAND_MAX+1.0));
  return(j);
}

int get_left(int clid) {
  int i;
  for(i=clid+(MAX_CLIENTS-1);i>=clid;i--)
    if (clients[i%MAX_CLIENTS])
      return(i%MAX_CLIENTS);
  return(clid);
}

int get_right(int clid) {
  int i;
  for(i=clid+1;i<=(clid+MAX_CLIENTS);i++)
    if (clients[i%MAX_CLIENTS])
      return(i%MAX_CLIENTS);
  return(clid);
}

void maybe_hit(int attacker,int defender) {
  int n,a,d;
  char msg[256];
  
  if (attacker==defender)
    return;

  n=roll_d6()+roll_d6()+roll_d6();

  if (n>hackords[attacker].Aggr)
    return;

  sprintf(msg,"hit roll: %s (attacking) vs. %s (defending)",
	  hackords[attacker].name,
	  hackords[defender].name);
  show_msg(msg);

  a=hackords[attacker].DX+roll_d6()+2; /* +2 innitiative bonus */
  d=hackords[defender].DX+roll_d6();
  if (a<=d) {
    sprintf(msg,"hit roll: %s (defender) dodges!",
	    hackords[defender].name);
    show_msg(msg);
    return; /* dodge! */
  }
  
  a=(hackords[attacker].ST*roll_d6())/3;
  hackords[defender].HT-=a;

  sprintf(msg,"hit success: %s loses %d health point(s)",
	  hackords[defender].name,a);
  show_msg(msg);

  /* knocked out */
  if (hackords[defender].HT<=0) {
    sprintf(msg,"hit success: %s is knocked out",
	    hackords[defender].name);
    show_msg(msg);

    shutdown(clients[defender],2);
    clients[defender]=0;
  }
}

/* send full status report to a watcher */
void inform_the_audience(int wid) {
  unsigned char v;
  int i,rc;

  if (!watchers[wid])
    return;

  read(watchers[wid],&v,1);

  rc=0;
  for(i=0;i<MAX_CLIENTS;i++)
    if (clients[i])
      ++rc;

  v=(unsigned char)rc;
  if (write(watchers[wid],&v,1)!=1) {
    if (!amdaemon)
      printf("shutting down watcher\n");
    shutdown(watchers[wid],2);
    watchers[wid]=0;
    WatcherCount--;
    return;
  }

  for(i=0;i<MAX_CLIENTS;i++) {
    if (!clients[i])
      continue;  
    write(watchers[wid],hackords[i].name,16);
    v=(unsigned char) hackords[i].ST;
    write(watchers[wid],&v,1);
    v=(unsigned char) hackords[i].HT;
    if (hackords[i].HT < 0) v=0;
    write(watchers[wid],&v,1);
    v=(unsigned char) hackords[i].DX;
    write(watchers[wid],&v,1);
    v=(unsigned char) hackords[i].Aggr;
    write(watchers[wid],&v,1);
    v=0;
    if (hackords[i].LChop && hackords[i].RChop) ++v;
    write(watchers[wid],&v,1);
  }
}

void process_request(int clid) {
  char z[16],c,r,*p,msg[256];
  unsigned char u;
  int n,i;

  read(clients[clid],&c,1);

  switch(c) {
  case 'Q': /* client quits */
    shutdown(clients[clid],2);
    clients[clid]=0;
    sprintf(msg,"[%0.3d] %s quits",clid,hackords[clid].name);
    show_msg(msg);
    break;    
  case '?': /* query */
    read(clients[clid],&c,1);
    switch(c) {
    case '0':
      write(clients[clid],hackords[clid].name,16);
      break;
    case '1':
      u=(unsigned char)hackords[clid].ST;
      write(clients[clid],&u,1);
      break;
    case '2':
      u=(unsigned char)hackords[clid].DX;
      write(clients[clid],&u,1);
      break;
    case '3':
      u=(unsigned char)hackords[clid].Aggr;
      write(clients[clid],&u,1);
      break;
    case '4':
      u=(unsigned char)hackords[clid].HT;
      if (hackords[clid].HT<0) u=0;
      write(clients[clid],&u,1);
      break;      
    }
    break;
  case '0': /* parameter: name */
    memset(hackords[clid].name,0,16);
    p=hackords[clid].name;
    for(i=0;i<15;i++) {
      n=read(clients[clid],&c,1);
      if (n==1) {
	if (c=='\n')
	  break;
	(*p++)=c;
      }
    }
    sprintf(msg,"[%0.3d] update: name=%s st=%d dx=%d ag=%d ht=%d",
	    clid,
	    hackords[clid].name,
	    hackords[clid].ST,
	    hackords[clid].DX,
	    hackords[clid].Aggr,
	    hackords[clid].HT);
    show_msg(msg);
    break;
  case '1': /* parameter: ST */
    read(clients[clid],&c,1);
    hackords[clid].ST=(int)((unsigned char)c);
    sprintf(msg,"[%0.3d] update: name=%s st=%d dx=%d ag=%d ht=%d",
	    clid,
	    hackords[clid].name,
	    hackords[clid].ST,
	    hackords[clid].DX,
	    hackords[clid].Aggr,
	    hackords[clid].HT);
    show_msg(msg);
    break;
  case '2': /* parameter: DX */
    read(clients[clid],&c,1);
    hackords[clid].DX=(int)((unsigned char)c);
    sprintf(msg,"[%0.3d] update: name=%s st=%d dx=%d ag=%d ht=%d",
	    clid,
	    hackords[clid].name,
	    hackords[clid].ST,
	    hackords[clid].DX,
	    hackords[clid].Aggr,
	    hackords[clid].HT);
    show_msg(msg);
    break;
  case '3': /* parameter: Aggr */
    read(clients[clid],&c,1);
    hackords[clid].Aggr=(int)((unsigned char)c);
    sprintf(msg,"[%0.3d] update: name=%s st=%d dx=%d ag=%d ht=%d",
	    clid,
	    hackords[clid].name,
	    hackords[clid].ST,
	    hackords[clid].DX,
	    hackords[clid].Aggr,
	    hackords[clid].HT);
    show_msg(msg);
    break;
  case 'L': /* request left chopstick */
    if ((hackords[get_left(clid)].RChop)&&(!hackords[clid].LChop)) {
      r='0';
    } else {
      r='1';
      hackords[clid].LChop=1;
    }
    write(clients[clid],&r,1);
    sprintf(msg,"[%0.3d] %s requests left chopstick. request %s",
	    clid,
	    hackords[clid].name,
	    (hackords[clid].LChop?"succeeds":"fails"));
    show_msg(msg);
    if (r=='0') maybe_hit(clid,get_left(clid));
    break;
  case 'R': /* request right chopstick */
    if ((hackords[get_right(clid)].LChop)&&(!hackords[clid].RChop)) {
      r='0';
    } else {
      r='1';
      hackords[clid].RChop=1;
    }
    write(clients[clid],&r,1);
    sprintf(msg,"[%0.3d] %s requests right chopstick. request %s",
	    clid,
	    hackords[clid].name,
	    (hackords[clid].RChop?"succeeds":"fails"));
    show_msg(msg);
    if (r=='0') maybe_hit(clid,get_right(clid));
    break;
  case 'l': /* drop left chopstick */
    hackords[clid].LChop=0;
    if (hackords[clid].RChop)
      hackords[clid].LastAte=time(0);
    sprintf(msg,"[%0.3d] %s drops left chopstick.",
	    clid,
	    hackords[clid].name);
    show_msg(msg);
    break;
  case 'r': /* drop right chopstick */
    hackords[clid].RChop=0;
    if (hackords[clid].LChop)
      hackords[clid].LastAte=time(0);
    sprintf(msg,"[%0.3d] %s drops right chopstick.",
	    clid,
	    hackords[clid].name);
    show_msg(msg);
    break;
  }

}

void mainloop() {
  struct sockaddr_in pin;
  socklen_t addrlen;
  int NewClient;
  int i,j,maxh;
  char z[1024],msg[256];
  unsigned int c[4];
  fd_set rdset;
  struct timeval timeout;
  time_t now;

  while(1) {

    /* check incoming connections */
    if (ClientCount<MAX_CLIENTS) {
      addrlen=(socklen_t)sizeof(pin);
      NewClient = accept(ListenSocket, (struct sockaddr *)  &pin, &addrlen);
      if (NewClient>0) {
	if (!amdaemon) {
	  c[3] = ( pin.sin_addr.s_addr >> 24 ) & 0xff;
	  c[2] = ( pin.sin_addr.s_addr >> 16 ) & 0xff;
	  c[1] = ( pin.sin_addr.s_addr >> 8 )  & 0xff;
	  c[0] = ( pin.sin_addr.s_addr ) & 0xff;
	  snprintf(z, 32, "%d.%d.%d.%d", c[0], c[1], c[2], c[3]);
	  printf("incoming client connection from %s\n",z);
	}

	for(i=0;i<MAX_CLIENTS;i++)
	  if (!clients[i]) {
	    clients[i]=NewClient;
	    ClientCount++;

	    strcpy(hackords[i].name,"anon");
	    hackords[i].ST=10;
	    hackords[i].DX=10;
	    hackords[i].HT=100;
	    hackords[i].Aggr=10;
	    hackords[i].LastAte=time(0);
	    hackords[i].LChop=0;
	    hackords[i].RChop=0;

	    printf(" [%d]\n",i);
	    break;
	  }
	sprintf(z,"DINNERD %s\n",VERSION);
	write(NewClient,z,strlen(z));
      }
    }

    /* check incoming watcher connections */
    if (WatcherCount<MAX_CLIENTS) {
      addrlen=(socklen_t)sizeof(pin);
      NewClient = accept(ListenSocket2, (struct sockaddr *)  &pin, &addrlen);
      if (NewClient>0) {
	if (!amdaemon) {
	  c[3] = ( pin.sin_addr.s_addr >> 24 ) & 0xff;
	  c[2] = ( pin.sin_addr.s_addr >> 16 ) & 0xff;
	  c[1] = ( pin.sin_addr.s_addr >> 8 )  & 0xff;
	  c[0] = ( pin.sin_addr.s_addr ) & 0xff;
	  snprintf(z, 32, "%d.%d.%d.%d", c[0], c[1], c[2], c[3]);
	  printf("incoming watcher connection from %s\n",z);
	}

	for(i=0;i<MAX_CLIENTS;i++)
	  if (!watchers[i]) {
	    watchers[i]=NewClient;
	    WatcherCount++;
	    break;
	  }
      }
    }
    
    /* deal with clients */
    timeout.tv_sec=0;
    timeout.tv_usec=5000;
    FD_ZERO(&rdset);
    maxh=0;
    for(i=0;i<MAX_CLIENTS;i++) {
      if (clients[i]) {
	FD_SET(clients[i],&rdset);
	if (clients[i]>maxh)
	  maxh=clients[i];
      } /* if */
    } /* for */
    if (select(maxh+1,&rdset,0,0,&timeout)>0)
      for(i=0;i<MAX_CLIENTS;i++) {
	if (clients[i])
	  if (FD_ISSET(clients[i],&rdset))
	    process_request(i);
      }

    /* deal with watchers */
    timeout.tv_sec=0;
    timeout.tv_usec=5000;
    FD_ZERO(&rdset);
    maxh=0;
    for(i=0;i<MAX_CLIENTS;i++) {
      if (watchers[i]) {
	FD_SET(watchers[i],&rdset);
	if (watchers[i]>maxh)
	  maxh=watchers[i];
      } /* if */
    } /* for */
    if (select(maxh+1,&rdset,0,0,&timeout)>0)
      for(i=0;i<MAX_CLIENTS;i++) {
	if (watchers[i])
	  if (FD_ISSET(watchers[i],&rdset))
	    inform_the_audience(i);
      }

    /* check starved out guys */
    now=time(0);
    for(i=0;i<MAX_CLIENTS;i++)
      if (clients[i])	
	if ((now-hackords[i].LastAte)>(hackords[i].ST*3)) {
	  sprintf(msg,"[%0.3d] %s dies of starvation.",
		  i,hackords[i].name);
	  show_msg(msg);
	  shutdown(clients[i],2);
	  clients[i]=0;
	}

  } /* while(1) */

}

int main(int argc, char **argv) {
  int i;

  signal(SIGPIPE,SIG_IGN);

  for(i=1;i<argc;i++)
    if (!strcmp(argv[i],"--nodaemon"))
      amdaemon=0;

  if (amdaemon)
    daemonize();
  init();
  init_listen_socket();
  mainloop();
  return 0;
}
