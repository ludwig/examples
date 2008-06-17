

#ifndef GDU_H
#define GDU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <list>
#include <gtk/gtk.h>

class ClickableTarget {
 public:
  ClickableTarget(string &prefix,string &name,int x,int y,int w,int h);
  int inside(int x,int y);
  string & GetTarget();
 private:
  string target;
  int X,Y,X2,Y2;

};

class Node {
 public:
  Node(char *fullpath);
  Node(const char *fullpath);
  ~Node();

  void clean();
  void Set(char *path,int val);
  void recalc();
  void print(int nest=0);

  int GetTotal();
  int GetSingle();

  void paint(GdkDrawable *dest,GdkGC *gc,GdkFont *font,
	     int X,int Y,int W,int H,int PW,
	     list<ClickableTarget *> &links,
	     double kb_per_pixel=0.0,
	     int hue=0,int bright=0);

 private:
  int  IsPrefix(char *fullname);
  int  depth();
  int  CalcPW(GdkFont *font);

  vector<Node *> children;
  string name;
  string prefix;
  int    s_value;
  int    r_value;
};

#endif
