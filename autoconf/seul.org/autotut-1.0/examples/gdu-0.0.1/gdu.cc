// Distributed under the terms of the GNU General Public
// License, version 2 or later.
// Author: Felipe Bergo <bergo@seul.org>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>

#include <stack>

#include "gdu.h"
#include "config.h"

#include "back.xpm"
#include "reload.xpm"
#include "root.xpm"
#include "home.xpm"
#include "stop.xpm"
#include "about.xpm"

void start_poll();
void poll_iteration();
void create_interface();

void     gdu_destroy(GtkWidget *w,gpointer data);
void     gdu_paint(GtkWidget *w,GdkEventExpose *ee,gpointer data);
gboolean gdu_click(GtkWidget *w,GdkEventButton *eb,gpointer data);
gboolean idle_hook(gpointer data);
void     cb_go(GtkWidget *w,gpointer data);
void     cb_back(GtkWidget *w,gpointer data);
void     cb_reload(GtkWidget *w,gpointer data);
void     cb_stop(GtkWidget *w,gpointer data);
void     cb_about(GtkWidget *w,gpointer data);
void     about_close(GtkWidget *w,gpointer data);

FILE *du_pipe=0;
Node root("/");
GtkWidget *gdu, *canvas, *back,*stop,*status;
GdkFont *helv10=0;
int interrupted=0;

stack<string> history;
list<ClickableTarget *> links;

int main(int argc,char **argv) {

  gtk_init(&argc,&argv);
  gdk_rgb_init();
  
  create_interface();

  history.push(*(new string(getenv("HOME"))));

  start_poll();
  gtk_idle_add(idle_hook,0);
  
  gtk_main();

  if (helv10)
    gdk_font_unref(helv10);
  return 0;
}

// interface

void create_interface() {
  GtkWidget *v,*tb,*pm,*hb;
  GdkColor white;
  GtkStyle *style;
  GdkPixmap *pic;
  GdkBitmap *mask;

  int i;

  helv10=gdk_font_load("-*-helvetica-medium-r-normal--10-*-*-*-*-*-*");
  white.red=0xffff;
  white.green=0xffff;
  white.blue=0xffff;

  gdu=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(gdu),800,400);
  gtk_window_set_title(GTK_WINDOW(gdu),"gdu");
  gtk_widget_realize(gdu);

  v=gtk_vbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(gdu),v);  

  // toolbar

  hb=gtk_handle_box_new();
  gtk_handle_box_set_shadow_type(GTK_HANDLE_BOX(hb),GTK_SHADOW_OUT);
  gtk_handle_box_set_handle_position(GTK_HANDLE_BOX(hb),GTK_POS_LEFT);
  gtk_container_set_border_width(GTK_CONTAINER(hb),4);

  style=gtk_widget_get_style(gdu);
  tb=gtk_toolbar_new(GTK_ORIENTATION_HORIZONTAL,GTK_TOOLBAR_BOTH);
  gtk_toolbar_set_button_relief(GTK_TOOLBAR(tb),GTK_RELIEF_NONE);
  gtk_toolbar_set_space_style(GTK_TOOLBAR(tb),GTK_TOOLBAR_SPACE_LINE);

  // BACK
  pic=gdk_pixmap_create_from_xpm_d(gdu->window,&mask,
				   &style->bg[GTK_STATE_NORMAL],
				   (gchar **) back_xpm);
  pm=gtk_pixmap_new(pic,mask);
  gtk_widget_show(pm);
  back=gtk_toolbar_append_item(GTK_TOOLBAR(tb),"Back","Goes back",0,
			       pm,GTK_SIGNAL_FUNC(cb_back),0);

  // RELOAD
  pic=gdk_pixmap_create_from_xpm_d(gdu->window,&mask,
				   &style->bg[GTK_STATE_NORMAL],
				   (gchar **) reload_xpm);
  pm=gtk_pixmap_new(pic,mask);
  gtk_widget_show(pm);
  gtk_toolbar_append_item(GTK_TOOLBAR(tb),"Reload","Updates",0,
			  pm,GTK_SIGNAL_FUNC(cb_reload),0);

  // ROOT
  pic=gdk_pixmap_create_from_xpm_d(gdu->window,&mask,
				   &style->bg[GTK_STATE_NORMAL],
				   (gchar **) root_xpm);
  pm=gtk_pixmap_new(pic,mask);
  gtk_widget_show(pm);
  gtk_toolbar_append_item(GTK_TOOLBAR(tb),"Root","Loads /",0,
			  pm,GTK_SIGNAL_FUNC(cb_go),(gpointer)"/");


  // HOME
  pic=gdk_pixmap_create_from_xpm_d(gdu->window,&mask,
				   &style->bg[GTK_STATE_NORMAL],
				   (gchar **) home_xpm);
  pm=gtk_pixmap_new(pic,mask);
  gtk_widget_show(pm);
  gtk_toolbar_append_item(GTK_TOOLBAR(tb),"Home","Loads your home dir",0,
			  pm,GTK_SIGNAL_FUNC(cb_go),(gpointer)"~");

  // STOP
  pic=gdk_pixmap_create_from_xpm_d(gdu->window,&mask,
				   &style->bg[GTK_STATE_NORMAL],
				   (gchar **) stop_xpm);
  pm=gtk_pixmap_new(pic,mask);
  gtk_widget_show(pm);
  stop=gtk_toolbar_append_item(GTK_TOOLBAR(tb),"Stop","Interrupts",0,
			       pm,GTK_SIGNAL_FUNC(cb_stop),0);


  // ABOUT
  pic=gdk_pixmap_create_from_xpm_d(gdu->window,&mask,
				   &style->bg[GTK_STATE_NORMAL],
				   (gchar **) about_xpm);
  pm=gtk_pixmap_new(pic,mask);
  gtk_widget_show(pm);
  gtk_toolbar_append_item(GTK_TOOLBAR(tb),"About","About gdu",0,
			  pm,GTK_SIGNAL_FUNC(cb_about),0);

  gtk_container_add(GTK_CONTAINER(hb),tb);
  gtk_box_pack_start(GTK_BOX(v),hb,FALSE,FALSE,0);

  // canvas

  canvas=gtk_drawing_area_new();
  gtk_widget_set_events(canvas,
      (GdkEventMask)(GDK_EXPOSURE_MASK|GDK_BUTTON_PRESS_MASK));
  gtk_drawing_area_size(GTK_DRAWING_AREA(canvas),800,400);
  gtk_box_pack_start(GTK_BOX(v),canvas,TRUE,TRUE,0);

  style=gtk_style_new();
  for(i=0;i<5;i++)
    style->bg[i]=white;
  gtk_widget_set_style(canvas,style);

  // status bar

  status=gtk_statusbar_new();
  gtk_box_pack_start(GTK_BOX(v),status,FALSE,FALSE,0);

  gtk_signal_connect(GTK_OBJECT(gdu),"destroy",
		     GTK_SIGNAL_FUNC(gdu_destroy),0);
  gtk_signal_connect(GTK_OBJECT(canvas),"expose_event",
		     GTK_SIGNAL_FUNC(gdu_paint),0);
  gtk_signal_connect(GTK_OBJECT(canvas),"button_press_event",
		     GTK_SIGNAL_FUNC(gdu_click),0);
  
  gtk_widget_show(status);
  gtk_widget_show(tb);
  gtk_widget_show(hb);
  gtk_widget_show(v);
  gtk_widget_show(canvas);
  gtk_widget_show(gdu);

  gtk_widget_set_sensitive(back,FALSE);
}

void gdu_destroy(GtkWidget *w,gpointer data) {
  if (du_pipe) {
    pclose(du_pipe);
    du_pipe=0;
  }
  gtk_main_quit();
}

gboolean gdu_click(GtkWidget *w,GdkEventButton *eb,gpointer data) {
  int i,x,y;
  string *nt;
  list<ClickableTarget *>::iterator li;

  if (eb==0)
    return FALSE;

  x=(int)(eb->x);
  y=(int)(eb->y);

  for(li=links.begin();li!=links.end();li++) {
    if ((*li)->inside(x,y)) {
      nt=new string();
      (*nt)=(*li)->GetTarget();
      history.push(*nt);
      if (history.size()>1)
	gtk_widget_set_sensitive(back,TRUE);
      cb_reload(0,0);
      return FALSE;
    }
  }

  return FALSE;
}

void gdu_paint(GtkWidget *w,GdkEventExpose *ee,gpointer data) {
  int W,H,i;
  GdkGC *gc;
  list<ClickableTarget *>::iterator li;
  
  root.recalc();

  W=canvas->allocation.width;
  H=canvas->allocation.height;

  for(li=links.begin();li!=links.end();li++)
    delete(*li);
  links.clear();

  gc=gdk_gc_new(canvas->window);
  root.paint(canvas->window,gc,helv10,0,0,W,H,64,links);
  gdk_gc_destroy(gc);
}

void cb_go(GtkWidget *w,gpointer data) {
  char *s;
  s=(char *)data;

  if (du_pipe!=0)
    cb_stop(0,0);
  
  if (s[0]=='~')
    s=getenv("HOME");

  if (history.top()!=s)
    history.push(*(new string(s)));
  start_poll();
  gtk_idle_add(idle_hook,0);
  if (history.size()>1)
    gtk_widget_set_sensitive(back,TRUE);
}

void cb_back(GtkWidget *w,gpointer data) {
  history.pop();
  if (history.size()==1)
    gtk_widget_set_sensitive(back,FALSE);
  cb_reload(0,0);
}

void cb_reload(GtkWidget *w,gpointer data) {
  if (du_pipe!=0)
    cb_stop(0,0);
  start_poll();
  gtk_idle_add(idle_hook,0);
}

void cb_stop(GtkWidget *w,gpointer data) {
  if (!du_pipe)
    return;
  pclose(du_pipe);
  du_pipe=0;
  interrupted=1;
  gtk_statusbar_push(GTK_STATUSBAR(status),1,"Interrupted.");
}

int close_clicked=0;

void cb_about(GtkWidget *w,gpointer data) {
  GtkWidget *dlg,*v,*l,*hs,*hb,*ok;
  char z[1024];
  
  dlg=gtk_window_new(GTK_WINDOW_DIALOG);
  gtk_window_set_title(GTK_WINDOW(dlg),"About...");
  gtk_window_set_position(GTK_WINDOW(dlg),GTK_WIN_POS_CENTER);
  gtk_window_set_transient_for(GTK_WINDOW(dlg),GTK_WINDOW(gdu));
  gtk_container_set_border_width(GTK_CONTAINER(dlg),6);
  gtk_widget_realize(dlg);

  v=gtk_vbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(dlg),v);

  sprintf(z,"gdu version %s\nGTK+ Disk Usage Browser\n"\
	  "(C) 2001 Free Software Foundation\n"\
	  "Author: Felipe Bergo <bergo@seul.org>\n\n"\
	  "This program is distributed under the terms of the GNU\n"\
	  "General Public License version 2 or later. Visit\n"\
   	  "http://www.gnu.org/copyleft/gpl.html for details.",VERSION);
  l=gtk_label_new(z);
  gtk_label_set_justify(GTK_LABEL(l),GTK_JUSTIFY_LEFT);
  gtk_box_pack_start(GTK_BOX(v),l,TRUE,TRUE,4);

  hs=gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(v),hs,FALSE,FALSE,4);
  hb=gtk_hbutton_box_new();
  gtk_button_box_set_layout(GTK_BUTTON_BOX(hb), GTK_BUTTONBOX_END);
  gtk_button_box_set_spacing(GTK_BUTTON_BOX(hb), 5);
  gtk_box_pack_start(GTK_BOX(v),hb,FALSE,FALSE,4);
  ok=gtk_button_new_with_label("Close");
  GTK_WIDGET_SET_FLAGS(ok,GTK_CAN_DEFAULT);
  gtk_box_pack_start(GTK_BOX(hb),ok,TRUE,TRUE,0);
  gtk_widget_grab_default(ok);
  
  gtk_widget_show(ok);
  gtk_widget_show(hb);
  gtk_widget_show(hs);
  gtk_widget_show(l);
  gtk_widget_show(v);
  gtk_widget_show(dlg);

  gtk_grab_add(dlg);

  close_clicked=0;
  gtk_signal_connect(GTK_OBJECT(ok),"clicked",
		     GTK_SIGNAL_FUNC(about_close),0);
  while(!close_clicked)
    gtk_main_iteration();

  gtk_grab_remove(dlg);
  gtk_widget_destroy(dlg);
  close_clicked=0;
}

void about_close(GtkWidget *w,gpointer data) {
  close_clicked=1;
}

// polling

void start_poll() {
  char dustring[256],s[256];
  interrupted=0;
  root.clean();
  sprintf(dustring,"du -S %s 2>/dev/null",history.top().c_str());
  du_pipe=popen(dustring,"r");
  gtk_window_set_title(GTK_WINDOW(gdu),"gdu (in progress...)");
  gtk_widget_set_sensitive(stop,TRUE);
  sprintf(s,"Running %s ...",dustring);
  gtk_statusbar_push(GTK_STATUSBAR(status),1,s);
}

void poll_iteration() {
  char z[1024],*p;
  int v,i;

  if (!du_pipe)
    return;

  for(i=0;i<60;i++) {
    if (!fgets(z,1023,du_pipe)) {
      pclose(du_pipe);
      du_pipe=0;
      return;
    }
    v=atoi(strtok(z," \t\n\r"));
    p=strtok(0," \t\n\r");
    root.Set(p,v);
  }

}

gboolean idle_hook(gpointer data) {
  if (!du_pipe) {
    gtk_window_set_title(GTK_WINDOW(gdu),"gdu");
    gtk_widget_set_sensitive(stop,FALSE);
    if (!interrupted)
      gtk_statusbar_push(GTK_STATUSBAR(status),1,"Complete.");
    return FALSE;
  }
  poll_iteration();
  gdk_window_clear(canvas->window);
  gdu_paint(canvas,0,0);
  return TRUE;
}
