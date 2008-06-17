

#ifndef MODELS_H
#define MODELS_H

#include <GL/glut.h>

#define VSET(a,b,c,d) a[0]=b; a[1]=c; a[2]=d;

#define DIFF3(_a,_b,_c) { \
    (_c)[0] = (_a)[0] - (_b)[0]; \
    (_c)[1] = (_a)[1] - (_b)[1]; \
    (_c)[2] = (_a)[2] - (_b)[2]; \
}

void box(float x1,float y1, float z1,float x2,float y2,float z2);
void make_notebook(GLint *notebook);
void make_arms(GLint *arm1, GLint *arm2, float c1, float c2, float c3,
	       int sticks);
void make_human(GLint *human, float c1, float c2, float c3);
void make_food(GLint *food);
void make_ground(GLint *ground);
void make_chair(GLint *chair);
void make_table(GLint *table);
void make_stick(GLint *stick);
void make_biff(GLint *biff);

#endif
