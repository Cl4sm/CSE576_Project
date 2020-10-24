setupUrban(mode)
int mode;
{
    int i, vskip;
    char s[80];

    if (!do_urban) return;

    if (mode == -2) {
       urban_t[0] = urban_t[2] = 7;
       urban_t[1] = 15 + UrbanGeom.width/2;
       urban_t[3] = (UrbanGeom.width - 120)/2;
       urban_t[4] = UrbanGeom.width - 100;
       urban_x[0] = 100;
       urban_x[1] = 100 + (UrbanGeom.width+1)/2;
       urban_x[2] = 80;
       urban_x[3] = 20 + UrbanGeom.width/2;
       urban_x[4] = UrbanGeom.width-40;
       for (i=0; i<=1; i++)
           urban_y[i] = UrbanCaller->gdata->mapstrip/2 - 1;
       for (i=2; i<=4; i++)
           urban_y[i] = 2*UrbanCaller->gdata->mapstrip - 1;
       urban_w[0] = urban_w[1] = (UrbanGeom.width-210)/2;
       urban_w[2] = urban_w[3] = (UrbanGeom.width-320)/2;
       urban_w[4] = 35;
       return;
    }

    BasicSettings(UrbanCaller);
    XSetWindowColormap(dpy, Urban, UrbanCaller->gdata->cmap);
    XSetWindowBackground(dpy, Urban, UrbanCaller->gdata->pixel[MENUBGCOLOR]);

    urbanh = UrbanGeom.height-2*UrbanCaller->gdata->menustrip - 1;
    vskip = 3*UrbanCaller->gdata->menustrip/8;
    urban_lasthint = '\0';

    if (mode == -1) {
       XClearArea(dpy, Urban, 0,0, UrbanGeom.width,
                                    UrbanGeom.height, False);
       for (i=0; i<=N_URBAN; i++)
	  drawButton(Urban, i, 0);

       BasicSettings(UrbanCaller);
       for (i=0; i<=4; i++) {
           strcpy(s, Label[L_CITYNAME+i]);
           XDrawImageString(dpy, Urban, UrbanCaller->gdata->wingc, 
	       urban_t[i], 
               urban_y[i]+
                  UrbanCaller->gdata->font[MENUFONT]->max_bounds.ascent+4, 
               s, strlen(s));
           XDrawRectangle(dpy, Urban, UrbanCaller->gdata->wingc,
                           urban_x[i], urban_y[i], urban_w[i], 
                           UrbanCaller->gdata->menustrip);
       }
    }  

    for (i=0; i<=4; i++) 
       if (text_input != URBAN_INPUT+i)
          showCaret(UrbanCaller, Urban, &urban_entry[i], 
                                 urban_x[i]+6, urban_y[i], 0);
    for (i=0; i<=4; i++)  
       if (text_input == URBAN_INPUT+i || text_input < URBAN_INPUT) {
       XSetWindowBackground(dpy, Urban, UrbanCaller->gdata->pixel[OPTIONBGCOLOR]);
       XClearArea(dpy, Urban, urban_x[i]+1, urban_y[i]+1, 
              urban_w[i]-1,
              UrbanCaller->gdata->menustrip-1, False);
       XSetBackground(dpy, UrbanCaller->gdata->wingc, 
                      UrbanCaller->gdata->pixel[OPTIONBGCOLOR]);
       XSetForeground(dpy, UrbanCaller->gdata->wingc, 
                      UrbanCaller->gdata->pixel[OPTIONFGCOLOR]);
       XDrawImageString(dpy, Urban, UrbanCaller->gdata->wingc,
          urban_x[i]+6,
          urban_y[i]+ UrbanCaller->gdata->font[MENUFONT]->max_bounds.ascent +4,
          urban_entry[i].string, strlen(urban_entry[i].string));
       if (text_input == URBAN_INPUT+i)
          showCaret(UrbanCaller, Urban, &urban_entry[i], 
                                 urban_x[i]+6, urban_y[i], 1);
    }
    if (urban_newhint == '?')
       urban_newhint = urban_lasthint = '(';
    else
       showUrbanHint(NULL);
}
