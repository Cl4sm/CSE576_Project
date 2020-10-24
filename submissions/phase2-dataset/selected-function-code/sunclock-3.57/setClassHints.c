setClassHints(win, num)
Window win;
int    num;
{
        char *titlename, *iconname;
        XClassHint xch;

	titlename = NULL;
	if (!Title) StringReAlloc(&Title, ProgName);

	if (!ClassName) {
	   StringReAlloc(&ClassName, ProgName);
	   *ClassName = toupper(*ClassName);
	}

	if (ClassName && !ClockClassName) 
           StringReAlloc(&ClockClassName, ClassName);

	if (ClassName && !MapClassName) 
           StringReAlloc(&MapClassName, ClassName);

	if (ClassName && !AuxilClassName) 
           StringReAlloc(&AuxilClassName, ClassName);

	if (num == 0)
	  xch.res_class = ClockClassName;
        else
	if (num == 1)
	  xch.res_class = MapClassName;
	else
	  xch.res_class = AuxilClassName;

        xch.res_name = ProgName;

        XSetClassHint(dpy, win, &xch);

	iconname = (char *)
           salloc((strlen(Title)+strlen(VERSION)+10)*sizeof(char));
        sprintf(iconname, "%s %s", Title, VERSION);
        XSetIconName(dpy, win, iconname);
	free(iconname);

	titlename = (char *)
           salloc((strlen(Title)+20)*sizeof(char));
        sprintf(titlename, "%s / %s", Title, widget_type[num]);
        XStoreName(dpy, win, titlename);
	free(titlename);
}
