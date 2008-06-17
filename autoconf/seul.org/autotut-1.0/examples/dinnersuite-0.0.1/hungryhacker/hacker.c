/* hungryhacker - distributed under the terms of the GNU
   General Public License -- Author: Felipe Bergo */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <dinner.h>

char     name[17],hostname[128];
int      st,dx,ag,port;
dinner_t me;
int      silent=0;

void alternate_dinning_and_coding() {
  int am_eating=0;
  time_t last_meal,now,dt,ht=100;
  int j;
  int action;

  last_meal=time(0);
  ht=dinner_get_ht(me);

  while(1) {
    j=1+(int) (8.0*rand()/(RAND_MAX+1.0));
    dt=time(0)-last_meal;
    if ((dt>20)&&(!am_eating))
      j=2;
    
    sleep(j);
    
    if (am_eating) {
      last_meal=time(0);
      dinner_stop_eat(me);
      am_eating=0;
      action=0;
    } else {
      j=dinner_try_eat(me);
      if (j<0) {
	dinner_close(me);
	if (!silent)
	  printf("%s(%d): link closed\n",name,ht);
	return;
      }
      action=1;
      if (j) {
	am_eating=1;
	action=2;
      }
    }
    
    ht=dinner_get_ht(me);
    if (ht==0) {
      dinner_close(me);
      printf("%s(%d): dead!\n",name,ht);
      return;
    }

    if (!silent) {
      printf("%s(%d): ",name,ht);
      switch(action) {
      case 0:
	printf(" stops eating.\n");
	break;
      case 1:
	printf(" tries to eat but fails\n");
	break;
      case 2:
	printf(" starts eating.\n");
	break;
      }
    }
  }
}

void print_usage() {
  fprintf(stderr,"hungryhacker - author: Felipe Bergo\n");
  fprintf(stderr,"Distributed under the terms of the GNU General\n");
  fprintf(stderr,"Public License\n\n");
  fprintf(stderr,"Usage:\n");
  fprintf(stderr," hungryhacker [-q] [-h host] [-p port] [-n name]\n");
  fprintf(stderr,"              [-s strength] [-d dexterity] [-a aggressivity]\n");
  fprintf(stderr,"\nstrength, dexterity and aggressivty work fairer\n");
  fprintf(stderr,"if in the range [5,15]\n\n");  
}

int main(int argc,char **argv) {
  int i,j;

  srand(time(0)/getpid());

  memset(name,0,17);
  j=(int) (26.0*rand()/(RAND_MAX+1.0));
  name[0]=j+'a';
  j=(int) (26.0*rand()/(RAND_MAX+1.0));
  name[1]=j+'a';
  j=(int) (26.0*rand()/(RAND_MAX+1.0));
  name[2]=j+'a';
  strcpy(hostname,"localhost");
  port=8081;

  st=5+(int) (10.0*rand()/(RAND_MAX+1.0));
  dx=5+(int) (10.0*rand()/(RAND_MAX+1.0));
  ag=5+(int) (10.0*rand()/(RAND_MAX+1.0));

  for(i=1;i<argc;i++) {
    if (!strcmp(argv[i],"--help")) {
      print_usage();
      exit(1);
    }
    if (!strcmp(argv[i],"-q")) {
      silent=1;
      continue;
    }

    if (i!=(argc-1)) {

      if (!strcmp(argv[i],"-h")) {
	strncpy(hostname,argv[++i],127);
	continue;
      }

      if (!strcmp(argv[i],"-p")) {
	port=atoi(argv[++i]);
	continue;
      }

      if (!strcmp(argv[i],"-n")) {
	strncpy(name,argv[++i],16);
	continue;
      }

      if (!strcmp(argv[i],"-s")) {
	st=atoi(argv[++i]);
	continue;
      }

      if (!strcmp(argv[i],"-d")) {
	dx=atoi(argv[++i]);
	continue;
      }

      if (!strcmp(argv[i],"-a")) {
	ag=atoi(argv[++i]);
	continue;
      }
    }
  }

  if (!silent) {
    printf("hacker info: %s, st=%d, dx=%d, aggr=%d\n",name,st,dx,ag);
    printf("connecting to %s:%d...\n",hostname,port);
  }
  me=dinner_open(hostname,port);

  if (me<0) {
    if (!silent)
      printf("failed.\n");
    perror(argv[0]);
    exit(2);
  }

  assert(dinner_set_name(me,name)==0);
  assert(dinner_set_st(me,st)==0);
  assert(dinner_set_dx(me,dx)==0);
  assert(dinner_set_ag(me,ag)==0);

  alternate_dinning_and_coding();

  return 0;
}
