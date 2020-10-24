                            const int *ylist, int totalsize) {
/* sc_window_clear_napalm */

   sc_window_gtk *w = (sc_window_gtk *)w_;
   int boundx1;
   int boundy1;
   int boundx2;
   int boundy2;

   if(xlist == NULL || ylist == NULL || w_ == NULL) return;

   boundx1 = w->c->fieldwidth;
   boundx2 = w->c->fieldheight;
   boundy1 = 0;
   boundy2 = 0;

   while(totalsize > 0) {
      if(*xlist < boundx1) boundx1 = *xlist;
      if(*xlist > boundx2) boundx2 = *xlist;
      if(*ylist < boundy1) boundy1 = *ylist;
      if(*ylist > boundy2) boundy2 = *ylist;
      ++xlist;
      ++ylist;
      --totalsize;
   }

   boundx1 -= SC_NAPALM_FLAME_RAD;
   boundx2 += SC_NAPALM_FLAME_RAD;
   boundy2 += 2 * SC_NAPALM_FLAME_RAD;

   sc_window_paint(w_, boundx1, boundy1, boundx2, boundy2,
                   SC_REDRAW_LAND | SC_REDRAW_TANKS);

}
