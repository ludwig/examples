
#include <math.h>
#include <GL/glut.h>

#include "models.h"

void box(float x1,float y1, float z1,float x2,float y2,float z2) {  
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,-(z1-z2)/fabs(z1-z2));
  glVertex3f(x1,y1,z1);
  glVertex3f(x1,y2,z1);
  glVertex3f(x2,y1,z1);
  glVertex3f(x2,y2,z1);
  glNormal3f(-(x2-x1)/fabs(x2-x1),0,0);
  glVertex3f(x2,y1,z2);
  glVertex3f(x2,y2,z2);
  glNormal3f(0,0,-(z2-z1)/fabs(z2-z1));
  glVertex3f(x1,y1,z2);
  glVertex3f(x1,y2,z2);
  glNormal3f(-(x1-x2)/fabs(x1-x2),0,0);
  glVertex3f(x1,y1,z1);
  glVertex3f(x1,y2,z1);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,(y1>y2)?1:-1,0);
  glVertex3f(x1,y1,z1);
  glVertex3f(x1,y1,z2);
  glVertex3f(x2,y1,z2);
  glVertex3f(x2,y1,z1);
  glVertex3f(x1,y1,z1);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,(y1<y2)?1:-1,0);
  glVertex3f(x1,y2,z1);
  glVertex3f(x1,y2,z2);
  glVertex3f(x2,y2,z2);
  glVertex3f(x2,y2,z1);
  glVertex3f(x1,y2,z1);
  glEnd();
}

static void
crossprod(GLfloat v1[3], GLfloat v2[3], GLfloat prod[3])
{
  GLfloat p[3];

  p[0] = v1[1] * v2[2] - v2[1] * v1[2];
  p[1] = v1[2] * v2[0] - v2[2] * v1[0];
  p[2] = v1[0] * v2[1] - v2[0] * v1[1];
  prod[0] = p[0];
  prod[1] = p[1];
  prod[2] = p[2];
}

static void
normalize(GLfloat v[3])
{
  GLfloat d;

  d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  if (d == 0.0)
    v[0] = d = 1.0;
  d = 1 / d;
  v[0] *= d;
  v[1] *= d;
  v[2] *= d;
}

void make_notebook(GLint *notebook) {
  *notebook=glGenLists(1);
  glNewList(*notebook,GL_COMPILE);

  glColor3f(0.10,0.10,0.15);
  box(-0.30,0.10,-0.60, 0.30, 0.20, 0.00);

  glPushMatrix();
  glTranslatef(0.0, 0.80, -0.6);
  glRotatef(-115.0, 1, 0, 0);
  box(-0.30,0.20,-0.60, 0.30, 0.30, 0.00);
  glPopMatrix();

  glEndList();
}

void make_arms(GLint *arm1, GLint *arm2, float c1, float c2, float c3,
	       int sticks) {
  float q,rx,rz,i,a[3],b[3],c[3],d[3],e[3],f[3];

  *arm1=glGenLists(1);
  *arm2=glGenLists(1);

  glNewList(*arm1,GL_COMPILE);
  glBegin(GL_QUAD_STRIP);
  glColor3f(c1,c2,c3);
  rx=0.120;
  rz=0.120;
  q=M_PI/4.0;
  for(i=0.0;i<=8.0;i++) {   
    VSET(a,rx*cos(i*q), 0.0, rz*sin(i*q));
    normalize(a);
    glNormal3fv(a);
    glVertex3f( rx*cos(i*q), 0.000, rz*sin(i*q) ); // 800
    glVertex3f( rx*cos(i*q), -0.400, rz*sin(i*q) );
  }
  glEnd();
  glutSolidSphere(0.130, 7, 7);
  glEndList();

  glNewList(*arm2,GL_COMPILE);
  glColor3f(c1,c2,c3);
  glBegin(GL_QUAD_STRIP);
  rx=0.115;
  rz=0.115;
  q=M_PI/4.0;
  for(i=0.0;i<=8.0;i++) {   
    VSET(a,rx*cos(i*q), 0.0, rz*sin(i*q));
    normalize(a);
    glNormal3fv(a);
    glVertex3f( rx*cos(i*q), 0.0, rz*sin(i*q) );
    glVertex3f( rx*cos(i*q), -0.40, rz*sin(i*q) );
  }
  glEnd();
  glutSolidSphere(0.120, 6, 6);
  glColor3f(0.98,0.63,0.51);
  glPushMatrix();
  glTranslatef(0.0, -0.40, 0.0);
  glutSolidSphere(0.100, 6, 6);
  glPopMatrix();

  
  /* chopsticks */
  if (sticks) {
    glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(0.0,-0.40, 0.0);
    box(0, 0, 0,  0.020, -0.250, 0.020);
    glPushMatrix();
    glTranslatef(0,-0.050,0);
    glRotatef(30.0, 1, 0, 0);
    box(0, 0, 0,  0.020, -0.250, 0.020);
    glPopMatrix();
    glPopMatrix();
  }

  glEndList();  

}

void make_stick(GLint *stick) {
  *stick=glGenLists(1);  
  glNewList(*stick,GL_COMPILE);
  glColor3f(1.0,1.0,1.0);
  box(0, 0.150, 0,  0.020, 0.170, -0.350);
  glEndList();
}

void make_human(GLint *human, float c1, float c2, float c3) {
  float q,rx,rz,i,a[3],b[3],c[3],d[3],e[3],f[3];

  *human=glGenLists(1);
  glNewList(*human,GL_COMPILE);

  /* trunk */
  glBegin(GL_QUAD_STRIP);
  glColor3f(c1,c2,c3);
  rx=0.3;
  rz=0.18;
  q=M_PI/8.0;
  for(i=0.0;i<=16.0;i++) {   
    VSET(a,rx*cos(i*q), 0.0, rz*sin(i*q));
    normalize(a);
    glNormal3fv(a);
    glVertex3f( rx*cos(i*q), 0.9, rz*sin(i*q) );
    glVertex3f( rx*cos(i*q), 0.0, rz*sin(i*q) );
  }
  glEnd();

  /* shoulders */
  glBegin(GL_QUAD_STRIP);
  glColor3f(c1,c2,c3);
  rx=0.3;
  rz=0.18;
  q=M_PI/8.0;
  for(i=0.0;i<=16.0;i++) {   
    VSET(a,(rx*0.4)*cos(i*q), 1.0, (rz*0.4)*sin(i*q));
    VSET(b,rx*cos(i*q), 0.9, rz*sin(i*q));
    VSET(c,rx*cos((i+1.0)*q), 0.9, rz*sin((i+1.0)*q));
    DIFF3(a,b,d);
    DIFF3(b,c,e);
    crossprod(e,d,f);
    normalize(f);
    glNormal3fv(f);
    glVertex3fv(a);
    glVertex3fv(b);
  }
  glEnd();

  /* neck */
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.98,0.63,0.51);
  rx=0.3*0.4;
  rz=0.25*0.4;
  q=M_PI/4.0;
  for(i=0.0;i<=8.0;i++) {   
    VSET(a,rx*cos(i*q), 0.0, rz*sin(i*q));
    normalize(a);
    glNormal3fv(a);
    glVertex3f( rx*cos(i*q), 1.0, rz*sin(i*q) );
    glVertex3f( rx*cos(i*q), 1.15, rz*sin(i*q) );
  }
  glEnd();

  /* head */
  glPushMatrix();
  glTranslatef(0.0, 1.25, 0.0);
  glutSolidSphere(0.220,12,5);
  glPopMatrix();

  /* pants, vert */
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.2,0.2,0.6);
  rx=0.3;
  rz=0.18;
  q=M_PI/8.0;
  for(i=0.0;i<=16.0;i++) {   
    VSET(a,rx*cos(i*q), 0.0, rz*sin(i*q));
    normalize(a);
    glNormal3fv(a);
    glVertex3f( rx*cos(i*q), 0.0, rz*sin(i*q) );
    glVertex3f( rx*cos(i*q), -0.25, rz*sin(i*q) );
  }
  glEnd();

  /* pants, horz right */
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.2,0.2,0.6);
  rx=0.12;
  rz=0.12;
  q=M_PI/3.0;
  for(i=0.0;i<=6.0;i++) {   
    VSET(a,rx*cos(i*q), rz*sin(i*q), 0.0);
    normalize(a);
    glNormal3fv(a);
    glVertex3f( rx*cos(i*q)+0.18, rz*sin(i*q)-0.18 , 0.0 );
    glVertex3f( rx*cos(i*q)+0.18, rz*sin(i*q)-0.18 ,-0.6 );
  }
  glEnd();

 /* pants, vert right */
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.2,0.2,0.6);
  rx=0.12;
  rz=0.12;
  q=M_PI/3.0;
  for(i=0.0;i<=6.0;i++) {   
    VSET(a,rx*cos(i*q), rz*sin(i*q), 0.0);
    normalize(a);
    glNormal3fv(a);
    glVertex3f( rx*cos(i*q)+0.18, -0.25, rz*sin(i*q)-0.6);
    glVertex3f( rx*cos(i*q)+0.18, -0.9,  rz*sin(i*q)-0.6);
  }
  glEnd();

  /* pants, horz left */
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.2,0.2,0.6);
  rx=0.12;
  rz=0.12;
  q=M_PI/3.0;
  for(i=0.0;i<=6.0;i++) {   
    VSET(a,rx*cos(i*q), rz*sin(i*q), 0.0);
    normalize(a);
    glNormal3fv(a);
    glVertex3f( rx*cos(i*q)-0.18, rz*sin(i*q)-0.18 , 0.0 );
    glVertex3f( rx*cos(i*q)-0.18, rz*sin(i*q)-0.18 ,-0.6 );
  }
  glEnd();

 /* pants, vert left */
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.2,0.2,0.6);
  rx=0.12;
  rz=0.12;
  q=M_PI/3.0;
  for(i=0.0;i<=6.0;i++) {   
    VSET(a,rx*cos(i*q), rz*sin(i*q), 0.0);
    normalize(a);
    glNormal3fv(a);
    glVertex3f( rx*cos(i*q)-0.18, -0.25, rz*sin(i*q)-0.6);
    glVertex3f( rx*cos(i*q)-0.18, -0.9,  rz*sin(i*q)-0.6);
  }
  glEnd();

  glEndList();
}

void make_food(GLint *food) {
  float a[3],b[3],c[3],d[3],e[3],f[3],aa[3],bb[3],cc[3];

  *food=glGenLists(1);
  glNewList(*food,GL_COMPILE);

  glBegin(GL_QUAD_STRIP);
  VSET(a,-0.10,0.10,-0.10);
  VSET(b,-0.15,0.45,-0.15);
  VSET(c, 0.10,0.10,-0.10);
  DIFF3(a,b,d);
  DIFF3(b,c,e);
  crossprod(d,e,f);
  normalize(f);
  glNormal3fv(f);
  glColor3f(1.0,1.0,1.0);
  glVertex3fv(a); /* 1 */
  glVertex3fv(b); /* 2 */
  glVertex3fv(c); /* 3 */
  VSET(aa, 0.15,0.45,-0.15);
  glVertex3fv(aa); /* 4 */
  VSET(a, 0.10,0.10, 0.10); /* 5 */
  VSET(b, 0.15,0.45, 0.15); /* 6 */
  VSET(bb, -0.10,0.10,0.10); /* 7 */
  DIFF3(c,aa,d);
  DIFF3(c,a,e);
  crossprod(d,e,f);
  normalize(f);
  glNormal3fv(f);
  glVertex3fv(a); /* 5 */
  glVertex3fv(b); /* 6 */
  DIFF3(a,b,d);
  DIFF3(a,bb,e);
  crossprod(d,e,f);
  normalize(f);
  glNormal3fv(f);
  glVertex3fv(bb); /* 7 */
  VSET(a, -0.15,0.45, 0.15); /* 8 */
  VSET(b, -0.10,0.10, -0.10); /* 9 */
  DIFF3(bb,a,d);
  DIFF3(bb,b,e);
  crossprod(d,e,f);
  normalize(f);
  glNormal3fv(f);
  glVertex3fv(a); /* 8 */
  glVertex3fv(b); /* 9 */
  glVertex3f(-0.15,   0.45,   -0.15);
  glEnd();

  glBegin(GL_QUADS);
  glColor3f(1.0,0.7,0.3);
  glNormal3f(0,1,0);
  glVertex3f(-0.10, 0.35, -0.10);
  glVertex3f( 0.10, 0.35, -0.10);
  glVertex3f( 0.10, 0.35,  0.10);
  glVertex3f(-0.10, 0.35,  0.10);
  glEnd();

  glEndList();
}

void make_ground(GLint *ground) {
  *ground=glGenLists(1);
  glNewList(*ground,GL_COMPILE);
  glBegin(GL_QUADS);

  glNormal3f(0,1,0);
  glColor3f(0.3, 0.35, 0.3);
  glVertex3f(-3.5,-1.03,-3.5);
  glVertex3f( 3.5,-1.03,-3.5);
  glVertex3f( 3.5,-1.03, 3.5);
  glVertex3f(-3.5,-1.03, 3.5);

  glEnd();
  glEndList();
}

void make_chair(GLint *chair) {
  float vdata[3];

  *chair=glGenLists(1);
  
  glNewList(*chair,GL_COMPILE);

  glColor3f(0.8,0.4,0.2);
  box(-0.3,  1.0, 0.6,  -0.25,  -1.0, 0.55);
  box(0.25,  1.0, 0.6,   0.3,   -1.0, 0.55);
  box(-0.3, -0.3, 0.05,  -0.25, -1.0,  0.0);
  box(0.25, -0.3, 0.05,   0.3,  -1.0,  0.0);

  box(-0.3, -0.3, 0.6,   0.3,  -0.25,  0.0);
  box(-0.3,  0.9, 0.6,   0.3,   0.7,  0.55);
  box(-0.3,  0.5, 0.6,   0.3,   0.3,  0.55);

  glEnd();
  glEndList();
}

void make_table(GLint *table) {
  float vdata[3];
  float i,q,r;

  r=2.0; q=M_PI/12.0;

  *table=glGenLists(1);

  glNewList(*table,GL_COMPILE);

  // table top
  glColor3f(0.4,0.7,0.4);
  glBegin(GL_TRIANGLE_FAN);

  vdata[0]=0.0; vdata[1]=0.1; vdata[2]=0.0;
  glNormal3f(0.0,1.0,0.0); glVertex3fv(vdata);

  for(i=0.0;i<=24.0;i++) {
    vdata[0]=r*(float)cos(q*i);    
    vdata[2]=r*(float)sin(q*i);
    glVertex3fv(vdata);
  }
  glEnd();


  // table bottom
  glColor3f(0.4,0.7,0.4);
  glBegin(GL_TRIANGLE_FAN);
  vdata[0]=0.0; vdata[1]=0.0; vdata[2]=0.0;
  glNormal3f(0.0,0.1,0.0); glVertex3fv(vdata);

  for(i=0.0;i<=24.0;i++) {
    vdata[0]=r*(float)cos(q*i);
    vdata[2]=r*(float)sin(q*i);
    glVertex3fv(vdata);
  }
  glEnd();

  // table border
  glColor3f(0.2,0.7,0.2);
  glBegin(GL_QUAD_STRIP);
  for(i=0.0;i<=24.0;i++) {
    vdata[0]=(float)cos(q*i);
    vdata[1]=0.0;
    vdata[2]=(float)sin(q*i);
    glNormal3f(cos(q*i),0.0,sin(q*i));
    vdata[0]*=r;
    vdata[2]*=r;
    glVertex3fv(vdata);
    vdata[1]=0.1;
    glVertex3fv(vdata);
  }
  glEnd();

  // table support
  r=0.2; q=M_PI/5.0;
  glColor3f(0.2,0.7,0.2);
  glBegin(GL_QUAD_STRIP);
  for(i=0.0;i<=10.0;i++) {
    vdata[0]=(float)cos(q*i);
    vdata[1]=0.0;
    vdata[2]=(float)sin(q*i);
    glNormal3f(cos(q*i),0.0,sin(q*i));
    vdata[0]*=r;
    vdata[2]*=r;
    glVertex3fv(vdata);
    vdata[1]=-1.0;
    glVertex3fv(vdata);
  }
  glEnd();


  glEndList();
}

void make_biff(GLint *biff) {
  float alt,i,ra,rb,q;

  *biff=glGenLists(1);

  glNewList(*biff,GL_COMPILE);

  q=360.0/18.0;

  glColor3f(1.0,0.0,1.0);
  glBegin(GL_TRIANGLE_FAN);

  glNormal3f(0,0,1);
  glVertex3f(0,0,0);

  alt=0.2;
  ra=0.8;
  rb=0.6;
  for(i=0.0;i<=360.0;i+=q) {
    glVertex3f((rb+alt)*cos(i*M_PI/180.0),
	       (ra+alt)*sin(i*M_PI/180.0),0);
    alt=-alt;
  }
  glEnd();

  glColor3f(1,1,1);
  glPushMatrix();
  glTranslatef(-0.5,0,0.020);
  glScalef(1.0/400.0,1.0/400.0,1.0);
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'B');
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'I');
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'F');
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'F');
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'!');
  glPopMatrix();

  glPushMatrix();
  glRotatef(180.0, 0,1,0);
  glTranslatef(-0.5,0,0.020);
  glScalef(1.0/400.0,1.0/400.0,1.0);
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'B');
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'I');
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'F');
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'F');
  glutStrokeCharacter(GLUT_STROKE_ROMAN,'!');
  glPopMatrix();

  glEndList();
}
