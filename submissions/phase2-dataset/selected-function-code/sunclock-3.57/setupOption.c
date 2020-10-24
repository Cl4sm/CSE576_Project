setupOption(mode)
int mode;
{
    int i, vskip;
    char s[80];

    if (!do_option) return;

    BasicSettings(OptionCaller);
    XSetWindowColormap(dpy, Option, OptionCaller->gdata->cmap);
    XSetWindowBackground(dpy, Option, OptionCaller->gdata->pixel[MENUBGCOLOR]);

    vskip = 3*OptionCaller->gdata->menustrip/8;
    option_lasthint = '\0';

    if (mode == -1) {
       XClearArea(dpy, Option, 0,0, OptionGeom.width,
                                    OptionGeom.height, False);
       for (i=0; i<=N_OPTION; i++)
	   drawButton(Option, i, 0);

       XSetBackground(dpy, OptionCaller->gdata->wingc, 
                      OptionCaller->gdata->pixel[MENUBGCOLOR]);
       XSetForeground(dpy, OptionCaller->gdata->wingc, 
                      OptionCaller->gdata->pixel[MENUFGCOLOR]);
       strcpy(s, Label[L_OPTION]);
       XDrawImageString(dpy, Option, OptionCaller->gdata->wingc, 
	       8, OptionCaller->gdata->font[MENUFONT]->max_bounds.ascent + vskip + 3,
               s, strlen(s));
       XDrawRectangle(dpy, Option, OptionCaller->gdata->wingc,
                           70, vskip, OptionGeom.width-85, 
                           OptionCaller->gdata->menustrip);
    }  

    XSetWindowBackground(dpy, Option,
       OptionCaller->gdata->pixel[OPTIONBGCOLOR]);
    XClearArea(dpy, Option, 71,vskip+1, OptionGeom.width-86,
           OptionCaller->gdata->menustrip-1, False);
    XSetBackground(dpy, OptionCaller->gdata->wingc, 
                        OptionCaller->gdata->pixel[OPTIONBGCOLOR]);
    XSetForeground(dpy, OptionCaller->gdata->wingc, 
                        OptionCaller->gdata->pixel[OPTIONFGCOLOR]);

    XDrawImageString(dpy, Option, OptionCaller->gdata->wingc, 76,
       OptionCaller->gdata->font[MENUFONT]->max_bounds.ascent + vskip + 3,
       option_entry.string, strlen(option_entry.string));
    if (text_input == OPTION_INPUT) 
       showCaret(OptionCaller, Option, &option_entry, 76, vskip, 1);
    showOptionHint(-1);
}
