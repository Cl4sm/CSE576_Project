
}


int undisplay_plotter(PLOTTER pl, int direction)
{
  int i;
  PLOTTER pll;
  
  if (direction == 1) {
    pll = pl->next;
    if (pll == 0) {
      pll = the_plotter_list;
    }
  } else if (direction == -1) {
    pll = the_plotter_list; 
    if (pl != the_plotter_list) {
      while (pll) {
	if (pll->next == pl) {
	  break;
	}
	pll = pll->next;
      }
    } else {
      /* go to last plotter on list */
      while (pll->next)
	pll = pll->next;
    }
  } else {
    pll = 0;
  }

  /* if previous (next) plotter on list doesn't have a window yet
     give it our window, GCs, and font_struct and set up all the
     other stuff that display_plotter() would set up . */ 

  if (pll && pll->win == 0) {
    pll->win = pl->win;
    for (i = 0; i < NColors; i++)
      pll->gcs[i] = pl->gcs[i];
    pll->decgc = pl->decgc;
    pll->xorgc = pl->xorgc;
    pll->font_struct = pl->font_struct;

    pll->xplot_nagle_atom = pl->xplot_nagle_atom;

    pll->xsh = pl->xsh;  /* unnecessary? */

    pll->visibility = pl->visibility;

    pll->gcv = pl->gcv; /* unnecessary? */

    pll->mainsize = pl->mainsize;

    pll->size_changed = 1; /* will trigger clearing and redisplay */

    /* take window away from this plotter */
    pl->win = 0;
    
    return 1;
  } else {
    XDestroyWindow(pl->dpy, pl->win);
    for (i = 0; i < NColors; i++)
      XFreeGC(pl->dpy, pl->gcs[i]);
    XFreeGC(pl->dpy, pl->decgc);
    XFreeGC(pl->dpy, pl->xorgc);
    XFreeFont(pl->dpy, pl->font_struct);
    pl->win = 0;
    return 0;
  }
    
#if 0
  while (pl->commands) {
    c = pl->commands;
    pl->commands = pl->commands->next;
    free_command(c);
  }
  