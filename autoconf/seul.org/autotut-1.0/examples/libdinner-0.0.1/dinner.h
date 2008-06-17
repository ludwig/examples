
#ifndef LIB_DINNER_H
#define LIB_DINNER_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* for use with a dinnerd server */

typedef int dinner_t;

dinner_t dinner_open(char *hostname,int port);
void     dinner_close(dinner_t did);

int      dinner_set_name(dinner_t did,char *name);

/* strength, dexterity, aggressiveness, health (ht is read-only) */

int      dinner_set_st(dinner_t did,int st);
int      dinner_set_dx(dinner_t did,int dx);
int      dinner_set_ag(dinner_t did,int ag);

int      dinner_get_st(dinner_t did);
int      dinner_get_dx(dinner_t did);
int      dinner_get_ag(dinner_t did);
int      dinner_get_ht(dinner_t did);

int      dinner_try_eat(dinner_t did);
int      dinner_stop_eat(dinner_t did);

/*
   watcher interface - the real port is the client port plus one,
   just supply the client port and the library will add one
   internally
*/

struct dinner_hacker_stat {
  char name[16];
  int  ST;
  int  DX;
  int  Aggr;
  int  HT;
  int  eating;  
};


/* special watcher connection */
dinner_t dinner_watch(char *hostname, int clientport);
void     dinner_unwatch(dinner_t did);

/*
   get the current global state

   will fill dest with hacker records. nhackers is both an input
   and output field. On input it must point to the maximum
   number of items in the dest vector, on output it points
   the number of hacker structures placed in dest.

   returns 0 on success, -1 on failure
*/
int      dinner_observe(dinner_t did, struct dinner_hacker_stat **dest,
			int *nhackers);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
