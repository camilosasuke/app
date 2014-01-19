#include "framework.h"

#include <X11/Xatom.h>


extern cairo_surface_t *  g_cairosurface;
extern cairo_t *  g_cairo;


oswindow_data::oswindow_data()
{

   m_plongmap              = new int_to_int;

   m_hthread               = NULL;

   m_window                = None;

   m_pui                   = NULL;

   m_bMessageOnlyWindow    = false;

   m_bDestroying           = false;

   m_osdisplay             = NULL;

}

oswindow_data::~oswindow_data()
{

   delete m_plongmap;

}



#define CA2_CCVOTAGUS_WINDOW_LONG "ca2_ccvotagus_fontopu_window_long"
#define CA2_CCVOTAGUS_WINDOW_LONG_STYLE "ca2_ccvotagus_fontopu_window_long_style"
#define CA2_CCVOTAGUS_WINDOW_LONG_STYLE_EX "ca2_ccvotagus_fontopu_window_long_style_ex"

oswindow_dataptra * oswindow_data::s_pdataptra = new oswindow_dataptra;
simple_mutex * oswindow_data::s_pmutex = new simple_mutex;


int32_t oswindow_find_message_only_window(::user::interaction_base * pui)
{

   if(pui == NULL)
      return -1;

   mutex_lock slOsWindow(*oswindow_data::s_pmutex, true);

   for(int32_t i = 0; i < ::oswindow_data::s_pdataptra->get_count(); i++)
   {
      if(::oswindow_data::s_pdataptra->element_at(i)->m_bMessageOnlyWindow
      && ::oswindow_data::s_pdataptra->element_at(i)->m_pui == pui)
      {
         return i;
      }
   }

   return -1;

}

int32_t oswindow_find(Display * pdisplay, Window window)
{

   mutex_lock slOsWindow(*::oswindow_data::s_pmutex, true);

   for(int32_t i = 0; i < ::oswindow_data::s_pdataptra->get_count(); i++)
   {
      if(!::oswindow_data::s_pdataptra->element_at(i)->m_bMessageOnlyWindow
      &&  ::oswindow_data::s_pdataptra->element_at(i)->m_osdisplay->display() == pdisplay
      &&  ::oswindow_data::s_pdataptra->element_at(i)->m_window == window)
      {
         return i;
      }
   }

   return -1;

}

int32_t oswindow_find(Window window)
{

   mutex_lock slOsWindow(*::oswindow_data::s_pmutex, true);

   for(int32_t i = 0; i < ::oswindow_data::s_pdataptra->get_count(); i++)
   {
      if(!::oswindow_data::s_pdataptra->element_at(i)->m_bMessageOnlyWindow
      &&  ::oswindow_data::s_pdataptra->element_at(i)->m_window == window)
      {
         return i;
      }
   }

   return -1;

}

oswindow_data * oswindow_get_message_only_window(::user::interaction_base * pui)
{

   if(pui == NULL)
      return NULL;

   mutex_lock slOsWindow(*::oswindow_data::s_pmutex, true);

   int_ptr iFind = oswindow_find_message_only_window(pui);

   if(iFind >= 0)
      return ::oswindow_data::s_pdataptra->element_at(iFind);

   ::oswindow_data * pdata = new oswindow_data;

   pdata->m_bMessageOnlyWindow      = true;
   pdata->m_window                  = None;
   pdata->m_pui                     = pui;
   pdata->m_osdisplay               = NULL;
   pdata->m_pvisual                 = NULL;

   ::oswindow_data::s_pdataptra->add(pdata);

   return pdata;

}


oswindow_data * oswindow_get(Display * pdisplay, Window window, Visual * pvisual)
{

   mutex_lock slOsWindow(*::oswindow_data::s_pmutex, true);

   int_ptr iFind = oswindow_find(pdisplay, window);

   if(iFind >= 0)
      return ::oswindow_data::s_pdataptra->element_at(iFind);

   ::oswindow_data * pdata = new ::oswindow_data;

   pdata->m_bMessageOnlyWindow      = false;
   pdata->m_osdisplay               = osdisplay_get(pdisplay);
   pdata->m_window                  = window;
   pdata->m_pvisual                 = pvisual;

   ::oswindow_data::s_pdataptra->add(pdata);

   return pdata;

}

oswindow_data * oswindow_get(Window window)
{

   mutex_lock slOsWindow(*::oswindow_data::s_pmutex, true);

   int_ptr iFind = oswindow_find(window);

   if(iFind < 0)
      return NULL;

   return ::oswindow_data::s_pdataptra->element_at(iFind);

}

/*
oswindow::oswindow()
{

   m_pdata = NULL;

}

oswindow::oswindow(::user::interaction_base * pui)
{

   mutex_lock slOsWindow(*s_pmutex, true);

   m_pdata = get_message_only_window(pui);



}

oswindow::oswindow(Display * pdisplay, Window window, Visual * pvisual)
{

   mutex_lock slOsWindow(*s_pmutex, true);

   m_pdata = get(pdisplay, window);

   if(pvisual != NULL)
   {

      m_pdata->m_pvisual = pvisual;

   }


}

oswindow::oswindow(const oswindow & oswindow)
{

   m_pdata = oswindow.m_pdata;

}

oswindow::oswindow(const void * p)
{

   m_pdata = (data *) p;

}

oswindow::oswindow(const LPARAM & lparam)
{

   m_pdata = (data *) lparam;

}

oswindow::oswindow(const WPARAM & wparam)
{

   m_pdata = (data *) wparam;

}






oswindow & oswindow::operator = (const oswindow & oswindow)
{

   m_pdata = oswindow.m_pdata;

   return *this;

}



*/


oswindow oswindow_defer_get(Window window)
{

   return oswindow_get(window);

}



bool oswindow_remove(Display * pdisplay, Window window)
{

   mutex_lock slOsWindow(*::oswindow_data::s_pmutex, true);

   int_ptr iFind = oswindow_find(pdisplay, window);

   if(iFind < 0)
      return false;

   ::oswindow_data::s_pdataptra->remove_at(iFind);

   return true;

}


bool oswindow_remove_message_only_window(::user::interaction_base * puibaseMessageOnlyWindow)
{

   mutex_lock slOsWindow(*::oswindow_data::s_pmutex, true);

   int_ptr iFind = oswindow_find_message_only_window(puibaseMessageOnlyWindow);

   if(iFind < 0)
      return false;

   ::oswindow_data::s_pdataptra->remove_at(iFind);

   return true;

}


int32_t oswindow_data::store_name(const char * psz)
{

   mutex_lock sl(user_mutex(), true);

   mutex_lock slOsWindow(*s_pmutex, true);

   xdisplay d(display());

   return XStoreName(display(), window(), psz);

}


int32_t oswindow_data::select_input(int32_t iInput)
{


   mutex_lock sl(user_mutex(), true);

   mutex_lock slOsWindow(*s_pmutex, true);

   xdisplay d(display());

   return XSelectInput(display(), window(), iInput);

}


int32_t oswindow_data::select_all_input()
{

   xdisplay d(display());

   return select_input(ExposureMask | ButtonPressMask);

}


int32_t oswindow_data::map_window()
{


   mutex_lock sl(user_mutex(), true);

   mutex_lock slOsWindow(*s_pmutex, true);

   xdisplay d(display());

   return XMapWindow(display(), window());

}


void oswindow_data::post_nc_destroy()
{

   mutex_lock slOsWindow(*s_pmutex, true);

   oswindow_remove(display(), window());

}


void oswindow_data::set_user_interaction(::user::interaction_base * pui)
{

   mutex_lock slOsWindow(*s_pmutex, true);

   if(this == NULL)
      throw "error, m_pdata cannot be NULL to ::oswindow::set_user_interaction";

   m_pui = pui;

   m_hthread = pui->m_pthread->get_os_handle();

}


::user::interaction_base * oswindow_data::get_user_interaction_base()
{

   mutex_lock slOsWindow(*s_pmutex, true);

   if(this == NULL)
      return NULL;

   return m_pui;

}

::user::interaction_base * oswindow_data::get_user_interaction_base() const
{

   if(this == NULL)
      return NULL;

   return m_pui;

}

::user::interaction * oswindow_data::get_user_interaction()
{

   mutex_lock slOsWindow(*s_pmutex, true);

   if(this == NULL)
      return NULL;

   if(m_pui == NULL)
      return NULL;

   return m_pui->m_pui;

}

::user::interaction * oswindow_data::get_user_interaction() const
{

   mutex_lock slOsWindow(*s_pmutex, true);

   if(this == NULL)
      return NULL;

   if(m_pui == NULL)
      return NULL;

   return m_pui->m_pui;

}


bool oswindow_data::is_child(::oswindow oswindow)
{

   mutex_lock sl(user_mutex(), true);

   mutex_lock slOsWindow(*s_pmutex, true);

   oswindow = oswindow->get_parent();

   while(!oswindow->is_null())
   {
      if(oswindow == this)
         return true;
   }

   return false;

}

oswindow oswindow_data::get_parent()
{

   mutex_lock sl(user_mutex(), true);

   mutex_lock slOsWindow(*s_pmutex, true);

   if(this == NULL)
      return NULL;

   return NULL;

   Window root = 0;
   Window parent = 0;
   Window * pchildren = NULL;
   uint32_t ncount = 0;

   XQueryTree(display(), window(), &root, &parent, &pchildren, &ncount);

   if(pchildren != NULL)
      XFree(pchildren);

   return oswindow_get(display(), parent);

}

oswindow oswindow_data::set_parent(oswindow oswindow)
{

   mutex_lock sl(user_mutex(), true);

   mutex_lock slOsWindow(*s_pmutex, true);

   if(this == NULL)
      return NULL;

   xdisplay d(display());

   ::oswindow oswindowOldParent = get_parent();

   XReparentWindow(display(), window(), oswindow->window(), 0, 0);

   return oswindowOldParent;

}

bool oswindow_data::show_window(int32_t nCmdShow)
{

   mutex_lock sl(user_mutex(), true);

   mutex_lock slOsWindow(*s_pmutex, true);

   xdisplay d(display());

   if(nCmdShow == SW_HIDE)
   {

      XUnmapWindow(display(), window());

   }
   else
   {

      XMapWindow(display(), window());

   }

}



LONG oswindow_data::get_window_long(int32_t nIndex)
{

   return m_plongmap->operator[](nIndex);

/*   Atom type = 0;
   int32_t format = 0;
   unsigned long itemcount = 0;
   unsigned long remaining = 0;
   LONG * pl = NULL;
   LONG l;

   if(XGetWindowProperty(display(), window(), m_pdata->m_osdisplay.get_window_long_atom(nIndex), 0, 1, False, m_pdata->m_osdisplay.atom_long_type(), &type, &format, &itemcount, &remaining, (unsigned char **) &pl) == Success)
   {

      if(pl != NULL)
      {

         l = *pl;

         XFree(pl);

      }
      else
      {

         l = 0;

      }

   }
   else
   {

      l = 0;

   }


   return l;*/

}


LONG oswindow_data::set_window_long(int32_t nIndex, LONG l)
{

   LONG lOld = m_plongmap->operator[](nIndex);

   m_plongmap->operator[](nIndex) = l;
/*   LONG lOld = get_window_long(nIndex);

   XChangeProperty(display(), window(), m_osdisplay.get_window_long_atom(nIndex), m_osdisplay.atom_long_type(), 32, PropModeReplace, (unsigned char *) &l, 1);*/

   return lOld;

}


bool oswindow_data::client_to_screen(POINT * pp)
{

   return true;

}


bool oswindow_data::screen_to_client(POINT * pp)
{

   return true;

}

Atom get_window_long_atom(int32_t nIndex);




long oswindow_data::get_state()
{


   mutex_lock sl(user_mutex(), true);

   xdisplay d(display());

  static const long WM_STATE_ELEMENTS = 2L;

  unsigned long nitems = 0;
  unsigned long leftover = 0;
  Atom xa_WM_STATE = 0;
  Atom actual_type = 0;
  int32_t actual_format = 0;
  int32_t status = 0;
  unsigned char* p = NULL;

  xa_WM_STATE = XInternAtom(display(), "WM_STATE", false);

  status = XGetWindowProperty(display(), window(), xa_WM_STATE, 0L, WM_STATE_ELEMENTS, False, xa_WM_STATE, &actual_type, &actual_format, &nitems, &leftover, &p);


  if(status == 0)
  {
     long lStatus = -1;
     if(p!= NULL)
      lStatus = (long)*p;
      XFree(p);
      return lStatus;
  }

  return -1;

}




bool oswindow_data::is_iconic()
{

   return get_state() == IconicState;

}

bool oswindow_data::is_window_visible()
{

   mutex_lock sl(user_mutex(), true);
   xdisplay d(display());

   if(d.m_pdisplay == NULL)
    return false;

   XWindowAttributes attr;
   if(!XGetWindowAttributes(display(), window(), &attr))
      return false;
   return attr.map_state == IsViewable;

}






/* Copyright (c) 2012 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Hello_World_(C,_Cairo)?action=history&offset=20070528220552

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Hello_World_(C,_Cairo)?oldid=10388
*/


#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <X11/Xlib.h>


#define SIZEX 100
#define SIZEY  50


void message_box_paint(cairo_surface_t * cs, stringa & stra, ::lemon_array < bool >  & baTab, ::lemon_array < int >  & ya,SIZE * psize)
{
	cairo_t *c;

	c=cairo_create(cs);
	cairo_rectangle(c, 0.0, 0.0, psize->cx, psize->cy);
	cairo_set_source_rgb(c, 0.84, 0.84, 0.77);
	cairo_fill(c);

	cairo_set_source_rgb(c, 0.0, 0.0, 0.0);

	for(index i = 0; i < stra.get_count(); i++)
	{
      cairo_move_to(c, 10.0 + 50.0 + (baTab[i] ? 25.0 : 0), 10.0 + 50.0 + ya[i]);
      cairo_show_text(c, stra[i]);
	}


	cairo_show_page(c);


	if(c ==  ::ca_cairo_keep::g_cairo)
	{
         printf("123");

	}

	cairo_destroy(c);

}

void message_box_show_xlib(const char * lpText, const char * lpCaption)
{

	Window rootwin;
	Window win;
	XEvent e;
	int32_t scr;
	cairo_surface_t *cs;


	xdisplay dpy;

	dpy.open(NULL);

	if(dpy== NULL)
	{
		fprintf(stderr, "ERROR: Could not open display\n");
		return ;
//		exit(1);
	}

	scr         = dpy.default_screen();
	rootwin     = RootWindow(dpy.m_pdisplay, scr);


	simple_graphics g;

	g.create(NULL);

	SIZE sz;

	sz.cx = 0;
	sz.cy = 0;


	stringa stra;

	stra.add_tokens(lpText, "\n");

	::lemon_array < bool > baTab;

	::lemon_array < int > ya;

	for(index i = 0; i < stra.get_count(); i++)
	{

	   string str = stra[i];

	   bool bTab = str_begins_dup(str, "\t");

	   str.trim();

	   bool bEmpty = str.is_empty();

	   if(bEmpty)
         str = "L";

	   SIZE sizeItem = g.get_text_extent(str);

	   int x = bTab ? 25 : 0;

	   if(sizeItem.cx + x > sz.cx)
	   {

	       sz.cx = sizeItem.cx + x;

	   }

	   baTab.add(bTab);

	   ya.add( sz.cy);

      sz.cy += sizeItem.cy;

      if(bEmpty)
      {
         stra[i] = "";
      }
      else
      {
         stra[i] = str;
      }

	}

	sz.cx += 100;
	sz.cy += 100;


	win=XCreateSimpleWindow(dpy.m_pdisplay, rootwin, 1, 1, sz.cx, sz.cy, 0, BlackPixel(dpy.m_pdisplay, scr), BlackPixel(dpy.m_pdisplay, scr));

	XStoreName(dpy, win, lpCaption);
	XSelectInput(dpy, win, ExposureMask|ButtonPressMask);
	XMapWindow(dpy, win);

	cs = cairo_xlib_surface_create(dpy.m_pdisplay, win, DefaultVisual(dpy.m_pdisplay, 0), sz.cx, sz.cy);

	while(1) {
		XNextEvent(dpy, &e);
		if(e.type==Expose && e.xexpose.count<1) {
			message_box_paint(cs, stra, baTab, ya, &sz);
		} else if(e.type==ButtonPress) break;
	}

   if(cs == ::ca_cairo_keep::g_cairosurface)
   {

      printf("123");

   }

	cairo_surface_destroy(cs);

	XCloseDisplay(dpy);

}



int32_t WINAPI MessageBoxA(oswindow hWnd, const char * lpText, const char * lpCaption, UINT uType)
{

   message_box_show_xlib(lpText, lpCaption);

   return 0;

}


static oswindow g_oswindowCapture;


oswindow GetCapture()
{
      return g_oswindowCapture;
}

oswindow SetCapture(oswindow window)
{

   mutex_lock sl(user_mutex(), true);

   oswindow windowOld(g_oswindowCapture);

   if(window->display() == NULL)
      return NULL;

   if(window->window() == None)
      return NULL;

   xdisplay d(window->display());

   if(XGrabPointer(window->display(), window->window(), False, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime) == GrabSuccess)
   {

      g_oswindowCapture = window;

      return windowOld;

   }

   return NULL;

}


WINBOOL ReleaseCapture()
{

   mutex_lock sl(user_mutex(), true);

   xdisplay d(g_oswindowCapture->display());


   WINBOOL bRet = XUngrabPointer(g_oswindowCapture->display(), CurrentTime) != FALSE;

   if(bRet)
      g_oswindowCapture = NULL;

   return bRet;

}


oswindow SetFocus(oswindow window)
{

   mutex_lock sl(user_mutex(), true);

   xdisplay display(window->display());

   if(!IsWindow(window))
      return NULL;

   oswindow windowOld = ::GetFocus();

   if(!XSetInputFocus(window->display(), window->window(), RevertToNone, CurrentTime))
      return NULL;

   return windowOld;

}

oswindow GetFocus()
{

   mutex_lock sl(user_mutex(), true);

   xdisplay pdisplay;

   pdisplay.open(NULL);

   if(pdisplay == NULL)
   return NULL;

   Window window = None;

   int revert_to = 0;

   bool bOk = XGetInputFocus(pdisplay, &window, &revert_to) != 0;

    pdisplay.close();

   if(!bOk)
      return NULL;

   if(window == None || window == PointerRoot)
      return NULL;

   return oswindow_defer_get(window);

}


oswindow GetActiveWindow()
{

   return GetFocus();

}


oswindow SetActiveWindow(oswindow window)
{

   return SetFocus(window);

}


oswindow GetWindow(oswindow windowParam, int iParentHood)
{

   mutex_lock sl(user_mutex(), true);




   oswindow window = windowParam;

   if(window == NULL)
      return NULL;

   xdisplay d(window->display());

   if(iParentHood == GW_HWNDFIRST
   || iParentHood == GW_HWNDLAST
   || iParentHood == GW_HWNDNEXT
   || iParentHood == GW_HWNDPREV)
   {

      window = ::GetParent(window);

      if(window == NULL)
         return NULL;

   }


   Window root = 0;
   Window parent = 0;
   Window * pchildren = NULL;
   uint32_t ncount = 0;

   XQueryTree(window->display(), window->window(), &root, &parent, &pchildren, &ncount);

   switch(iParentHood)
   {
      case GW_CHILD:
      case GW_HWNDFIRST:
      {

         if(pchildren == NULL)
            return NULL;

         window = ::oswindow_get(window->display(), pchildren[0]);

      }
      break;
      case GW_HWNDLAST:
      {

         if(pchildren == NULL)
            return NULL;

         window = ::oswindow_get(window->display(), pchildren[ncount - 1]);

      }
      break;
      case GW_HWNDNEXT:
      case GW_HWNDPREV:
      {

         if(pchildren == NULL) // ????
            return NULL;

         int iFound = -1;

         for(int i = 0; i < ncount; i++)
         {
               if(pchildren[i] == windowParam->window())
               {
                  iFound = i;
                  break;
               }
         }

         if(iFound < 0)
            return NULL;

         if(iParentHood == GW_HWNDNEXT)
         {

            if(iFound + 1 >= ncount)
               return NULL;

            window = ::oswindow_get(window->display(), pchildren[iFound - 1]);

         }
         else
         {

            if(iFound - 1 < 0)
               return NULL;

            window = ::oswindow_get(window->display(), pchildren[iFound - 1]);

         }

      }

   }


   if(pchildren != NULL)
      XFree(pchildren);


   return window;

}



WINBOOL DestroyWindow(oswindow window)
{

   mutex_lock sl(user_mutex(), true);

   if(!IsWindow(window))
      return FALSE;

   Display * pdisplay = window->display();

   Window win = window->window();

   xdisplay d(pdisplay);


   oswindow_data * pdata = (oswindow_data *) (void *) window;

   pdata->m_bDestroying = true;

   bool bIs = IsWindow(window);

   XDestroyWindow(pdisplay, win);

   return true;

}


bool oswindow_data::is_destroying()
{

   if(this == NULL)
      return true;

   if(m_bDestroying)
      return true;

   return false;

}


bool IsWindow(oswindow oswindow)
{
   return oswindow->get_user_interaction() != NULL && !oswindow->is_destroying();
}


oswindow g_oswindowDesktop;


bool c_xstart()
{

   Display * dpy = XOpenDisplay(NULL);

   g_oswindowDesktop = oswindow_get(dpy, DefaultRootWindow(dpy));

   XSelectInput(g_oswindowDesktop->display(), g_oswindowDesktop->window(), StructureNotifyMask);

   return true;

}



oswindow GetDesktopWindow()
{

return g_oswindowDesktop;
}
