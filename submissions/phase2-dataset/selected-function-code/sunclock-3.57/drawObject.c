void
drawObject(Context, lon, lat, mode, color, name)
struct Sundata * Context;
double lon, lat;                /* Latitude and longtitude of the city */
int    mode;
int    color;
char *name;
{
    /*
     * Local Variables
     */
    Pixel pixel;

    int ilon, ilat;             /* Screen coordinates of the city */
    int i, j, dx, dy, u, which, bool=0;
    unsigned short * bits;
    char slat[20], slon[20];

    if (mode == 0) return;
    if ((!Context->wintype && mode > 0) || mode < -SPECIALBITMAPS) return;
    if (mode > city_cat) mode = city_cat;
    if (mode > 0) {
       which = SPECIALBITMAPS + Context->spotsizes[mode-1] - 1;
       if (which < SPECIALBITMAPS) return;
       if (Context->flags.colorlevel>0 && !Context->flags.citymode) return;
       if (Context->zoom.width < Context->sizelimits[mode-1]
           && color==0) return;
       if (color<0) color = 0;
    } else
       which = -mode - 1;

    ilon = int_longitude(Context, lon);
    if (ilon<0 || ilon>Context->geom.width) return;

    ilat = int_latitude(Context, lat);
    if (ilat<0 || ilat>Context->geom.height) return;

    bits = symbol_bits[which];
 
    dx = bits[0]/2;
    dy = bits[1]/2;

    pixel = Context->gdata->pixel[CITYCOLOR0+color];

    if (Context->flags.colorlevel == FULLCOLORS) {
       for (j=0; j<bits[1]; j++) {
	  if (ilat-dy+j >= (int) Context->geom.height) break;
          u = bits[j+2];
          for (i=0; i<bits[0]; i++) {
             if (u&1) SetPixelLight(Context, ilon-dx+i, ilat-dy+j, pixel);
             u = u>>1;
          }
       }
    } else 
    if (Context->flags.colorlevel >= FEWCOLORS) {
       XSetForeground(dpy, Context->gdata->wingc, pixel);
       for (j=0; j<bits[1]; j++) {
	  if (ilat-dy+j >= (int) Context->geom.height) break;
          u = bits[j+2];
          for (i=0; i<bits[0]; i++) {
	     if (u&1) XDrawPoint(dpy, Context->win, Context->gdata->wingc, 
                ilon-dx+i, ilat-dy+j);
	     u = u>>1;
	  }
       }
    } else {
       XSetForeground(dpy, Context->gdata->pixgc, pixel);
       for (j=0; j<bits[1]; j++) {
	  if (ilat-dy+j >= (int) Context->geom.height) break;
          u = bits[j+2];
          for (i=0; i<bits[0]; i++) {
	     if (u&1) XDrawPoint(dpy, Context->mappix, Context->gdata->pixgc, 
                ilon-dx+i, ilat-dy+j);
	     u = u>>1;
	  }
       }
    }

    if (mode<0) ++dx;

    if (Context->flags.citymode==2 && name) {
       if (Context->flags.colorlevel == FULLCOLORS)
          XPutStringImage(Context, ilon+dx, ilat-1, name, strlen(name), 2);
       else
	 if (Context->flags.colorlevel >= FEWCOLORS) {
          XSetForeground(dpy, Context->gdata->wingc, 
			     Context->gdata->pixel[CITYNAMECOLOR]);
          XDrawString(dpy, Context->win, Context->gdata->wingc, 
                      ilon+dx, ilat-1, name, strlen(name));
       } else
          XDrawString(dpy, Context->mappix, Context->gdata->pixgc, 
                      ilon+dx, ilat-1, name, strlen(name));
    }

    if (!Context->wintype) return;
    if ((Context->flags.citymode==3 && mode>0) ||
        (bool=(Context->flags.objectmode==2 && mode<-1))) {
       dy = Context->gdata->mapstrip/2;
       (void) num2str(lat, slat, Context->flags.dms);
       (void) num2str(lon, slon, Context->flags.dms);
       if (Context->flags.colorlevel == FULLCOLORS) {
	  XSetFont(dpy, Context->gdata->pixgc,
		        Context->gdata->font[CITYFONT]->fid);
          XPutStringImage(Context, ilon+dx, ilat-1, slat, strlen(slat), 2);
          XPutStringImage(Context, ilon+dx, ilat-1+dy, slon, strlen(slon), 2);
       } else
       if (Context->flags.colorlevel >= FEWCOLORS ) {
          XSetBackground(dpy, Context->gdata->wingc, Context->gdata->pixel[MAPBGCOLOR]);
          XSetForeground(dpy, Context->gdata->wingc, Context->gdata->pixel[CITYNAMECOLOR]);
          XSetFont(dpy, Context->gdata->wingc, 
                        Context->gdata->font[CITYFONT]->fid);
          XDrawString(dpy, Context->win, Context->gdata->wingc, 
                      ilon+dx, ilat-1, slat, strlen(slat));
          XDrawString(dpy, Context->win, Context->gdata->wingc, 
                      ilon+dx, ilat-1+dy, slon, strlen(slon));
       } else {
	  XSetFont(dpy, Context->gdata->pixgc,
		        Context->gdata->font[CITYFONT]->fid);
          XDrawString(dpy, Context->mappix, Context->gdata->pixgc, 
                      ilon+dx, ilat-1, slat, strlen(slat));
          XDrawString(dpy, Context->mappix, Context->gdata->pixgc, 
                      ilon+dx, ilat-1+dy, slon, strlen(slon));
       }
    }
}
