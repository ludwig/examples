// Distributed under the terms of the GNU General Public
// License, version 2 or later.
// Author: Felipe Bergo <bergo@seul.org>

#include <iostream.h>
#include <string.h>
#include "gdu.h"

ClickableTarget::ClickableTarget(string &prefix,string &name,
				 int x,int y,int w,int h) {
  target=prefix+name;
  X=x; Y=y;
  X2=x+w;
  Y2=y+h;
}

int ClickableTarget::inside(int x,int y) {
  return( (x>=X)&&(y>=Y)&&(x<=X2)&&(y<=Y2) );
}

string & ClickableTarget::GetTarget() {
  return(target);
}

Node::Node(char *fullpath) {
  char *p,z[1024];
  
  //  cout << "new Node [" << fullpath << "]\n";

  strcpy(z,fullpath);  
  p=rindex(z,'/');
  if (p==0)
    name="";
  else
    name=p+1;

  if (p!=0) {
    *(p+1)=0;
    prefix=z;
  } else
    prefix="/";
  s_value=0;
  r_value=0;
}

Node::Node(const char *fullpath) {
  char *p,z[1024];
  
  //  cout << "new Node [" << fullpath << "]\n";

  strcpy(z,fullpath);  
  p=rindex(z,'/');
  if (p==0)
    name="";
  else
    name=p+1;

  if (p!=0) {
    *(p+1)=0;
    prefix=z;
  } else
    prefix="/";
  r_value=0;
  s_value=0;
}

Node::~Node() {
  int i;
  for(i=0;i<children.size();i++)
    delete(children[i]);
}

void Node::clean() {
  int i;
  for(i=0;i<children.size();i++)
    delete(children[i]);
  children.clear();
  s_value=0;
  r_value=0;
}

void Node::Set(char *path,int val) {
  string me,son;
  Node *n1,*n2;
  int i,j;
  me=prefix+name;

  //  cout << "I am " << me << endl;
  //  cout << "trying " << path << " val=" << val << endl;
  
  if (me==path){
    s_value=val;
    return;
  }

  for(i=0;i<children.size();i++) {
    if (children[i]->IsPrefix(path)) {
      children[i]->Set(path,val);
      return;
    }
  }

  // inexistent, must breed, this is the last common prefix
  son=path;
  
  n1=this;
  i=me.size();

  do {
    i=son.find("/",i+1);
    //  /home/joe/target
    //            ^
    //            i+1

    n1->children.push_back(n2=new Node(son.substr(0,i).c_str()));
    //    cout << n1->prefix << n1->name << ": created " << n2->prefix << n2->name << endl;
    n1=n2;
  } while(i!=son.npos);

  Set(path,val);
}


int  Node::IsPrefix(char *fullname) {
  string me,it;
  me=prefix+name;
  //  cout << "IsPrefix: am(" << me << ") full(" << fullname << ")\n";

  it=fullname;
  return(it.find(me)==0);
}

void Node::recalc() {
  int i,v=0;

  v=s_value;

  for(i=0;i<children.size();i++) {
    children[i]->recalc();
    v+=children[i]->r_value;
  }

  r_value=v;
}

int Node::GetTotal() {
  return(r_value);
}

int Node::GetSingle() {
  return(s_value);
}

int Node::depth() {
  int i,md=0,d;
  if (children.empty())
    return 1;
  for(i=0;i<children.size();i++) {
    d=children[i]->depth();
    if (d>md) md=d;
  }
  return(md+1);
}

void Node::print(int nest=0) {
  int i;

  for(i=0;i<nest;i++)
    cerr << " ";

  cerr << "Node prefix=[" << prefix << "] name=[" << name << "] #children=" << children.size() << endl;

  for(i=0;i<children.size();i++)
    children[i]->print(nest+3);
}

void Node::paint(GdkDrawable *dest,GdkGC *gc,GdkFont *font,
		 int X,int Y,int W,int H,int PW,
		 list<ClickableTarget *> &links,
		 double kb_per_pixel=0.0,
		 int hue=0,int bright=0)
{
  int ca,cb,ci,cpw,i,ih,mw,sw,x,y,pw;
  double ch;
  char s[128];

  if ((!s_value)&&(!r_value))
    return;

  if (W<1)
    return;

  if (kb_per_pixel==0.0)
    kb_per_pixel=((double)r_value)/((double)H);

  ci=(hue&0x01)<<1;
  ci|=(bright&0x01);

  switch(ci) {
  case 0:
    ca=0xd0d0ff;
    cb=0x0000ff;
    break;
  case 1:
    ca=0x8080ff;
    cb=0x0000ff;
    break;
  case 2:
    ca=0xffd0d0;
    cb=0xff0000;
    break;
  case 3:
    ca=0xff8080;
    cb=0xff0000;
    break;
  }

  // draw my own box

  gdk_rgb_gc_set_foreground(gc,ca);
  gdk_draw_rectangle(dest,gc,TRUE,X,Y,PW,H);
  gdk_rgb_gc_set_foreground(gc,cb);
  gdk_draw_rectangle(dest,gc,FALSE,X,Y,PW,H);

  mw=PW-10;
  if (mw<10)
    goto no_string;

  sprintf(s,"%s %d",name.c_str(),r_value);
  sw=gdk_string_width(font,s);
  
  if (sw>mw) {
    strcpy(s,name.c_str());
    i=strlen(s)-1;
    if (i)
      do {
	sw=gdk_string_width(font,s);
	if (sw>mw)
	  s[i--]=0;
	if (!i)
	  goto no_string;
      } while(sw>mw);
  }
  
  if (H<14)
    goto no_string;

  if (sw<=mw) {
    gdk_rgb_gc_set_foreground(gc,0x000000);
    gdk_draw_string(dest,font,gc,X+5+(mw-sw)/2,Y+12+(H-12)/2,s);
  }

  links.push_back(new ClickableTarget(prefix,name,X,Y,PW,H));

 no_string:
  cpw=CalcPW(font)+10;

  for(y=Y,i=0;i<children.size();i++) {
    ch=((double)(children[i]->r_value))/kb_per_pixel;
    ih=(int)ch;
    if (ih>0) {
      children[i]->paint(dest,gc,font,
			 X+PW,y,
			 W-PW,ih,cpw,
			 links,
			 kb_per_pixel,
			 hue+1,i);
      y+=ih;
    }
  }  
}

int Node::CalcPW(GdkFont *font) {
  int i,w,mw=0;
  char z[128];

  if (children.empty())
    return 0;

  for(i=0;i<children.size();i++) {
    sprintf(z,"%s (%d)",children[i]->name.c_str(),children[i]->r_value);
    w=gdk_string_width(font,z);
    if (w>mw) mw=w;	    
  }
  return mw;
}
