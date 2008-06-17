/* 
   libdinner - by Felipe Bergo <bergo@seul.org>
   Distributed under the terms of the GNU General Public
   License
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "dinner.h"

dinner_t dinner_open(char *hostname,int port)
{
  struct hostent *he;
  struct sockaddr_in sa;
  int sock,r;
  char c;

  he=gethostbyname(hostname);
  if (he==NULL)
    return -1;
  sock=socket(AF_INET,SOCK_STREAM,0);
  if (sock<0)
    return -1;
  sa.sin_family=he->h_addrtype;
  sa.sin_port=htons(port);
  memcpy(&sa.sin_addr,he->h_addr_list[0],he->h_length);
  
  r=connect(sock,(struct sockaddr *)&sa,sizeof(sa));
  if (r!=0)
    return -1;

  c=0;
  while(c!='\n')
    if (read(sock,&c,1)<=0) c=0;

  return( (dinner_t) sock );
}

void     dinner_close(dinner_t did)
{
  shutdown(did,2);
}

int      dinner_set_name(dinner_t did,char *name)
{
  int r;
  r=write(did,"0",1);
  if (r<0) return -1;
  r=write(did,name,strlen(name));
  if (r<0) return -1;
  r=write(did,"\n",1);
  if (r<0) return -1;
  return 0;
}

/* strength, dexterity, aggressiveness, health (ht is read-only) */

int      dinner_set_st(dinner_t did,int st)
{
  int r;
  unsigned char c;
  r=write(did,"1",1);
  if (r<0) return -1;
  c=(unsigned char)st;
  r=write(did,&c,1);
  if (r<0) return -1;
  return 0;
}

int      dinner_set_dx(dinner_t did,int dx) {
  int r;
  unsigned char c;
  r=write(did,"2",1);
  if (r<0) return -1;
  c=(unsigned char)dx;
  r=write(did,&c,1);
  if (r<0) return -1;
  return 0;
}

int      dinner_set_ag(dinner_t did,int ag) {
  int r;
  unsigned char c;
  r=write(did,"3",1);
  if (r<0) return -1;
  c=(unsigned char)ag;
  r=write(did,&c,1);
  if (r<0) return -1;
  return 0;
}

int      dinner_get_st(dinner_t did) {
  unsigned char c=0;
  write(did,"?1",2);
  read(did,&c,1);
  return( (int) c );
}

int      dinner_get_dx(dinner_t did) {
  unsigned char c=0;
  write(did,"?2",2);
  read(did,&c,1);
  return( (int) c );
}

int      dinner_get_ag(dinner_t did) {
  unsigned char c=0;
  write(did,"?3",2);
  read(did,&c,1);
  return( (int) c );
}

int      dinner_get_ht(dinner_t did) {
  unsigned char c=0;
  write(did,"?4",2);
  read(did,&c,1);
  return( (int) c );
}

int      dinner_try_eat(dinner_t did) {
  char c;
  write(did,"L",1);
  if (read(did,&c,1)<0) return -1;
  if (c=='0') return 0;
  write(did,"R",1);
  if (read(did,&c,1)<0) {
    write(did,"l",1);
    return -1;
  }
  if (c=='0') {
    write(did,"l",1);
    return 0;
  }
  return 1;
}

int      dinner_stop_eat(dinner_t did) {
  if (write(did,"l",1)<0) return -1;
  if (write(did,"r",1)<0) return -1;
  return 0;
}

/* dinner observation interface */

dinner_t dinner_watch(char *hostname, int clientport) {
  struct hostent *he;
  struct sockaddr_in sa;
  int sock,r;
  char c;
  he=gethostbyname(hostname);
  if (he==NULL)
    return -1;
  sock=socket(AF_INET,SOCK_STREAM,0);
  if (sock<0)
    return -1;
  sa.sin_family=he->h_addrtype;
  sa.sin_port=htons(clientport+1);
  memcpy(&sa.sin_addr,he->h_addr_list[0],he->h_length); 
  r=connect(sock,(struct sockaddr *)&sa,sizeof(sa));
  if (r!=0)
    return -1;
  return( (dinner_t) sock );
}

void     dinner_unwatch(dinner_t did) {
  dinner_close(did);
}

/*
   get the current global state

   will fill dest with hacker records. nhackers is both an input
   and output field. On input it must point to the maximum
   number of items in the dest vector, on output it points
   the number of hacker structures placed in dest.

   returns 0 on success, -1 on failure
*/
int dinner_observe(dinner_t did, struct dinner_hacker_stat **dest,
		   int *nhackers)
{
  unsigned char c;
  int i,maxn;

  c='?';
  if (write(did,&c,1)!=1) return -1;

  maxn=*nhackers;

  usleep(5000);
  if (read(did,&c,1)!=1) return -1;
  
  *nhackers=(maxn>c) ? c : maxn; 

  for(i=0; i< (*nhackers) ; i++) {
    if (read(did,dest[i]->name,16)!=16) return -1;
    if (read(did,&c,1)!=1) return -1; dest[i]->ST=(int) c;
    if (read(did,&c,1)!=1) return -1; dest[i]->HT=(int) c;
    if (read(did,&c,1)!=1) return -1; dest[i]->DX=(int) c;
    if (read(did,&c,1)!=1) return -1; dest[i]->Aggr=(int) c;
    if (read(did,&c,1)!=1) return -1; dest[i]->eating=(int) c;
  }
  return 0;
}
