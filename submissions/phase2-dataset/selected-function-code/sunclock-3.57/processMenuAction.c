void 
processMenuAction(Context, x, y, button, evtype)
Sundata * Context;
int x, y, button, evtype;
{
static int click_pos = -1;
int but_pos;
KeySym key;
	   if (evtype == ButtonRelease && click_pos>=0) {
	       drawButton(Menu, click_pos, 0);
               click_pos = -1;
	   }
           but_pos = getButton(Menu, x, y);
           if (evtype == ButtonPress && but_pos>=0 && but_pos<=N_MENU) {
	       drawButton(Menu, but_pos, 1);
	       click_pos = but_pos;
               return;
	   }
           if (evtype == MotionNotify) {
	     if (but_pos >= N_MENU) {
		menu_newhint = XK_Escape;
                showMenuHint(N_MENU);
		return;
	     }
	     if (but_pos <= -1) 
		menu_newhint = ' ';
	     else
	        menu_newhint = MenuKey[2*but_pos];
             showMenuHint(getNumCmd(menu_newhint));
             return;
           }
	   if (but_pos==-1) return;	   
           if (do_menu && but_pos >= N_MENU) {
              PopMenu(MenuCaller);
              return;
           }
           key = MenuKey[2*but_pos];
	   if (button<=2) key = tolower(key);
           processKey(Menu, (KeySym)key);
}
