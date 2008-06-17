/* vdinner - Mesa-based dining hackers visualization */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <GL/glut.h>
#include <dinner.h>

#define FRAMERATE 5

#include "models.h"

struct dinner_hacker_stat hackers[256];
struct dinner_hacker_stat *hackerp[256];
int nhackers=0;
dinner_t netconn;
char hostname[128];
int  port;

int last_health[256], last_n=0;
int biffq[256];

GLint table, chair, ground, food, notebook, stick, biff;
GLint rhuman, rarm1, rarm2, rarm1s, rarm2s;
GLint yhuman, yarm1, yarm2;
float roty=-30.0, rotx=40.0, zoom=-7.50;

float armangle=0.0, forearm=0.0;
float armangle2=0.0, forearm2=0.0;

static struct timeval lf, tf;

unsigned long stat_a=0, stat_b=0;

static float light0_ambient[] = {0.1, 0.1, 0.1, 1.0};
static float light0_diffuse[] = {1.0, 1.0, 1.0, 0.0};
static float light0_position[] = {0.8660254, 1.5, 1.0, 0};
static float light0_specular[] = {0.3, 0.3, 0.3, 0.0};

static float light1_ambient[] = {0.1, 0.1, 0.1, 1.0};
static float light1_diffuse[] = {1.0, 1.0, 1.0, 1.0};
static float light1_position[] = {4.0, 0.5, 2.0, 0};
static float light1_specular[] = {1.0, 1.0, 1.0, 0.0};

static float light2_ambient[] = {0.1, 0.1, 0.1, 1.0};
static float light2_diffuse[] = {1.0, 1.0, 1.0, 1.0};
static float light2_position[] = {-4.0, 0.5, -2.0, 0};
static float light2_specular[] = {1.0, 1.0, 1.0, 0.0};

int smooth=1, wireframe=2, color=1, paused=0;

float animpath[30][2];

void init_path() {
  float a,b,c,i;

  a=0.0; b=0.0;

  for(i=0.0;i<15.00;i++) {
    animpath[(int)i][0]=5.0*i;
    animpath[(int)i][1]=-5.0*i;
    if (i>10.0)
      animpath[(int)i][1]+=7.0*(i-10.0);

    animpath[29-(int)i][0]=animpath[(int)i][0];
    animpath[29-(int)i][1]=animpath[(int)i][1];
  }
}

void model_init(void) {
  int i;

  for(i=0;i<256;i++) {
    hackerp[i]=&hackers[i];
    biffq[i]=0;
  }

  make_table(&table);
  make_chair(&chair);
  make_ground(&ground);
  make_food(&food);
  make_stick(&stick);
  make_human(&rhuman,0.8,0.2,0.2);
  make_arms(&rarm1,&rarm2,0.8,0.2,0.2,0);
  make_arms(&rarm1s,&rarm2s,0.8,0.2,0.2,1);

  make_human(&yhuman,0.8,0.8,0.4);
  make_arms(&yarm1,&yarm2,0.8,0.8,0.4,0);
  make_biff(&biff);

  init_path();
  /*  make_notebook(&notebook); */
}

void dinner_init (void)
{
  static float lmodel_ambient[] = {0.0, 0.0, 0.0, 0.0};
  static float lmodel_twoside[] = {GL_FALSE};
  static float lmodel_local[] = {GL_FALSE};

  static float bevel_mat_ambient[] = {0.0, 0.0, 0.0, 1.0};
  static float bevel_mat_shininess[] = {0.0};
  static float bevel_mat_specular[] = {0.0, 0.0, 0.0, 0.0};
  static float bevel_mat_diffuse[] = {0.0, 0.0, 0.0, 0.0};  
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glEnable(GL_LIGHT0); 
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
  glEnable(GL_LIGHT2);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_local);
  glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glEnable(GL_LIGHTING);
  glMaterialfv(GL_FRONT, GL_AMBIENT, bevel_mat_ambient);
  glMaterialfv(GL_FRONT, GL_SHININESS, bevel_mat_shininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bevel_mat_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, bevel_mat_diffuse);
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glShadeModel(GL_SMOOTH);
}

void drawstring(char *s) {
  int i,l;
  l=strlen(s);
  for(i=0;i<l;i++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN,s[i]);
}

void write_state(int idx) {
  int st=10,dx=12,ag=8,ht=98,i;
  char name[16];
  char line[256];

  strcpy(name,hackers[idx].name);

  for(i=0;i<strlen(name);i++)
    name[i]=toupper(name[i]);

  glPushMatrix();
  glColor3f(0,0,0);
  glScalef(1.0/550.0,1.0/550.0, 1.0);
  drawstring(name);
  glPopMatrix();

  sprintf(line,"Life %d",hackers[idx].HT);
  glPushMatrix();
  glColor3f(0,0,0);
  glTranslatef(0,-0.2,0);
  glScalef(1.0/600.0,1.0/600.0, 1.0);
  drawstring(line);
  glPopMatrix();

}

void display (void)
{
  float i, aa, ab, fh;
  int ii;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  
  glTranslatef(0, 0, zoom);
  glRotatef(rotx, 1, 0, 0);
  glRotatef(roty, 0, 1, 0);

  glLightfv(GL_LIGHT0, GL_POSITION, light0_position); 
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

  glCallList(table);

  fh=(float)(nhackers);

  for(i=0.0;i<fh;i++) {
    ii=(int)i;

    aa=hackers[ii].eating?armangle:0.0;
    ab=hackers[ii].eating?forearm:0.0;

    glPushMatrix();
    glRotatef(i*360.0/fh, 0.0, 1.0 , 0.0);
    glTranslatef(0, 0, 2.1);
    glCallList(chair);
    glPopMatrix();

    glPushMatrix();
    glRotatef(i*360.0/fh, 0.0, 1.0 , 0.0);
    glTranslatef(0, 0, 1.7);
    glCallList(food);
    glPopMatrix();

    /*
    glPushMatrix();
    glRotatef(i*360.0/fh, 0.0, 1.0 , 0.0);
    glTranslatef(0, 0, 1.5);
    glCallList(notebook);
    glPopMatrix();
    */

    glPushMatrix();
    glRotatef(i*360.0/fh, 0.0, 1.0 , 0.0);
    glTranslatef(0, 0, 2.4);
    glCallList(hackers[ii].eating?rhuman:yhuman);
    glPopMatrix();


    /* stick between ii and ii+1 */

    if ((!hackers[ii].eating)&&(!hackers[(ii+1)%nhackers].eating)) {
      glPushMatrix();
      glRotatef((i+0.5)*360.0/fh, 0.0, 1.0 , 0.0);
      glTranslatef(0, 0, 1.8);
      glCallList(stick);
      glPopMatrix();
    }

    glPushMatrix();
    glRotatef(i*360.0/fh, 0.0, 1.0 , 0.0);
    glTranslatef(-0.5, 0.300, 1.2);
    glRotatef(-75.0,1,0,0);
    write_state(ii);
    glPopMatrix();

    /* biff! */

    if (biffq[ii]) {
      glPushMatrix();
      glRotatef(i*360.0/fh, 0, 1, 0);
      glTranslatef(0,1.7,2.25);
      glScalef( ((float)(8-biffq[ii]))*0.10,
		((float)(8-biffq[ii]))*0.10,
		1.0);
      glCallList(biff);
      biffq[ii]--;
      glPopMatrix();
    }

    /* ARM */

    glPushMatrix();
    glRotatef(i*360.0/fh, 0.0, 1.0 , 0.0);
    glTranslatef(0.350, 0.800, 2.4);
    glRotatef(aa,1,0,0);
    glCallList(hackers[ii].eating?rarm1:yarm1);
    glPopMatrix();

    glPushMatrix();	       
    glRotatef(i*360.0/fh, 0.0, 1.0 , 0.0);
    glTranslatef(0.350, 0.400, 2.40);

    glRotatef(90.0, 1.0, 0.0 , 0.0);
    glRotatef(aa,1,0,0);

    glTranslatef(0,
		 -0.400*sin(aa*M_PI/180.0),
		 -0.400*cos(aa*M_PI/180.0));
    glRotatef(aa,1,0,0);
    glTranslatef(0,
		 0.400*sin(aa*M_PI/180.0),
		 0.400*cos(aa*M_PI/180.0));

    glRotatef(ab,1,0,0);

    glCallList(hackers[ii].eating?rarm2s:yarm2);
    glPopMatrix();
    
    /* --- */

    glPushMatrix();
    glRotatef(i*360.0/fh, 0.0, 1.0 , 0.0);
    glTranslatef(-0.350, 0.800, 2.4);
    glRotatef(armangle2,1,0,0);
    glCallList(hackers[ii].eating?rarm1:yarm1);
    glPopMatrix();

    glPushMatrix();	       
    glRotatef(i*360.0/fh, 0.0, 1.0 , 0.0);
    glTranslatef(-0.350, 0.400, 2.40);

    glRotatef(90.0, 1.0, 0.0 , 0.0);
    glRotatef(armangle2,1,0,0);

    glTranslatef(0,
		 -0.400*sin(armangle2*M_PI/180.0),
		 -0.400*cos(armangle2*M_PI/180.0));
    glRotatef(armangle2,1,0,0);
    glTranslatef(0,
		 0.400*sin(armangle2*M_PI/180.0),
		 0.400*cos(armangle2*M_PI/180.0));

    glRotatef(forearm2,1,0,0);

    glCallList(hackers[ii].eating?rarm2:yarm2);
    glPopMatrix();


  }

  glCallList(ground);
  
  glPopMatrix();  
  glFlush ();
  glutSwapBuffers();
}

void myReshape(int w, int h)
{
  glViewport (0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1.0, 0.1, 200.0);
  glMatrixMode(GL_MODELVIEW);
}

static void
key2(int k, int x, int y) {
  (void) x;
  (void) y;
  switch(k) {
  case GLUT_KEY_UP:
    rotx+=5.0;
    break;
  case GLUT_KEY_DOWN:
    rotx-=5.0;
    break;
  case GLUT_KEY_LEFT:
    roty-=5.0;
    break;
  case GLUT_KEY_RIGHT:
    roty+=5.0;
    break;
  }
  /*
  printf("arm=%.2f forearm=%.2f zoom=%.2f rotx=%.2f roty=%.2f\n",
	 armangle,forearm,zoom,rotx,roty);
  */
  glutPostRedisplay();
}

static void
key(unsigned char k, int x, int y)
{
  switch (k) {
  case 'p':
    paused=!paused;
    break;
  case 'a':
    armangle+=5;
    forearm-=5;
    break;
  case 'z':
    armangle-=5;
    forearm+=5;
    break;
  case 's':
    forearm+=5;
    break;
  case 'x':
    forearm-=5;
    break;
  case '-':
    zoom-=0.5;
    break;
  case '=':
    zoom+=0.5;
    break;
  case 'f':
    if (stat_b>0) {
      printf("avg time per frame=%lu , framerate=%.3f\n",
	     stat_a/stat_b, 1000.0/((double)(stat_a/stat_b)));
    }
    break;
  case '1':
    smooth=!smooth;
    glShadeModel(smooth?GL_SMOOTH:GL_FLAT);
    break;
  case '2':
    wireframe=(++wireframe)%3;
    glPolygonMode(GL_FRONT_AND_BACK,
		  wireframe==0?GL_POINT:wireframe==1?GL_LINE:GL_FILL);
    break;
  case '3':
    color=!color;
    if (color) glEnable(GL_COLOR_MATERIAL); else glDisable(GL_COLOR_MATERIAL);
      break;
  case 27:  /* Escape */
    dinner_unwatch(netconn);
    exit(0);
    break;
  default:
    return;
  }

  if (paused)
    glutPostRedisplay();
}

void anim(void) {
  static int frame=0;
  int i;
  long msec;

  if (paused) {
    usleep(50000);
    return;
  }
    

  gettimeofday(&tf,0);
  
  msec=(tf.tv_sec-lf.tv_sec)*1000;
  msec+=(tf.tv_usec-lf.tv_usec)/1000;
  
  if ( msec < (1000/FRAMERATE) )
    return;

  stat_a+=msec;
  ++stat_b;
  
  if (stat_a > (2<<30)) {
    stat_a=0;
    stat_b=0;
  }

  memcpy(&lf,&tf,sizeof(struct timeval));

  armangle=animpath[frame][0];
  forearm=animpath[frame][1];
  frame+=3;
  frame%=30;

  last_n=nhackers;
  for(i=0;i<last_n;i++)
    last_health[i]=hackers[i].HT;

  nhackers=256;
  if (dinner_observe(netconn,hackerp,&nhackers)<0) {
    perror("vdinner");
    exit(2);
  }

  if (last_n==nhackers)
    for(i=0;i<last_n;i++)
      if (hackers[i].HT!=last_health[i])
	biffq[i]=8;

  glutPostRedisplay();
}

void network_start(void) {
  printf("Connecting to %s:%d\n",hostname,port);
  netconn=dinner_watch(hostname,port);
  if (netconn<0) {
    perror("vdinner");
    exit(1);
  }
  printf("ok");
}


int main(int argc,char **argv) {
  int i;

  strcpy(hostname,"localhost");
  port=8081;

  for(i=1;i<(argc-1);i++) {
    if (!strcmp(argv[i],"-h"))
      strcpy(hostname,argv[++i]);
    if (!strcmp(argv[i],"-p"))
      port=atoi(argv[++i]);
  }

  network_start();

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (400, 400);
  glutCreateWindow ("Virtual Dinner");

  dinner_init ();
  model_init();

  glutReshapeFunc (myReshape);
  glutDisplayFunc(display);
  glutSpecialFunc(key2);
  glutKeyboardFunc(key);

  gettimeofday(&lf,0);
  glutIdleFunc(anim);
  glutMainLoop();
  dinner_unwatch(netconn);
  return 0;
}
