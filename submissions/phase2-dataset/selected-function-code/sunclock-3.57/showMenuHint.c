showMenuHint(but_pos)
int but_pos;
{
        static int move_pos = -1;
        char key;
	char hint[128];

	if (!do_menu) return;
	if (!MenuCaller) return;

        if (menu_newhint == XK_Shift_L || menu_newhint == XK_Shift_R) return;
	key = toupper((char) menu_newhint);

        if (move_pos>=0) drawButton(Menu, move_pos, -2);
        if (but_pos>=0) drawButton(Menu, but_pos, -1);
	move_pos = but_pos;

	if (key == menu_lasthint) return;
	helpHint(MenuCaller, key, hint);

        menu_lasthint = key;
        BasicSettings(MenuCaller);
	XDrawImageString(dpy, Menu, MenuCaller->gdata->wingc, 4, 
              MenuCaller->gdata->font[MENUFONT]->max_bounds.ascent + 
                 MenuCaller->gdata->menustrip + 4, 
              hint, strlen(hint));
}
