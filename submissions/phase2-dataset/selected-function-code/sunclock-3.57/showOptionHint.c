void
showOptionHint(but_pos)
int but_pos;
{
        static int move_pos = -1;
	char hint[128];
	int l, v;

        if (move_pos>=0) drawButton(Option, move_pos, -2);
        if (but_pos>=0) drawButton(Option, but_pos, -1);
	move_pos = but_pos;

	if (!do_option || option_lasthint==option_newhint) return;

	*hint = '\0';

	option_lasthint = option_newhint;
	v = OptionGeom.height - OptionCaller->gdata->menustrip;

	if (option_newhint=='\033')
           strcpy(hint, Label[L_ESCMENU]);
	else
	if (option_newhint==' ')
	   strcpy(hint, Label[L_OPTIONINTRO]);
	else
	if (option_newhint=='\n')
           strcpy(hint, Label[L_ACTIVATE]);
	else
	if (option_newhint=='?')
           strcpy(hint, Label[L_INCORRECT]);
	else {
	   if (option_newhint == 'G' || option_newhint == 'J') 
              option_lasthint = ' ';
	   helpHint(OptionCaller, option_newhint, hint);
	}

	l = strlen(hint);

	BasicSettings(OptionCaller);
        XSetWindowBackground(dpy, Option, OptionCaller->gdata->pixel[MENUBGCOLOR]);

	XClearArea(dpy, Option, 0, v+1, OptionGeom.width, 
             OptionCaller->gdata->menustrip-1, False);

        XDrawImageString(dpy, Option, OptionCaller->gdata->wingc, 
              4, v + OptionCaller->gdata->font[MENUFONT]->max_bounds.ascent + 3,
              hint, l);
}
