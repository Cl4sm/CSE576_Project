void sc_console_set_cursor(ScConsole *cons, gint x, gint y, gint width, gint height) {
/* sc_console_set_cursor
   Updates all attributes of the cursor.  The cursor's position is given in
   (x,y) which are CHARACTER coordinates relative to the TEXT BUFFER (not
   the viewport).  The cursor's dimensions are also specified.  To make the
   cursor invisible, use width = height = 0.  The viewport may be altered to
   ensure that the cursor is visible within the viewport.  */

   ScConsoleCursor *cursor;   /* Pointer to the cursor structure */
   gint oldx;                 /* Cursor's original position X */
   gint oldy;                 /* Cursor's original position Y */
   gint oldw;                 /* Cursor's original width */
   gint oldh;                 /* Cursor's original height */

   g_return_if_fail(IS_SC_CONSOLE(cons));
   g_return_if_fail(width >= 0 && height >= 0);

   /* Get the cursor */
   cursor = &cons->cursor;

   /* Update the cursor attributes */
   oldx = cursor->x;
   oldy = cursor->y;
   oldw = cursor->width;
   oldh = cursor->height;
   cursor->x = x;
   cursor->y = y;
   cursor->width = width;
   cursor->height= height;

   /* Update the display.  Make sure the cursor is on the screen somewhere. */
   _sc_console_lock_view_to_cursor(cons, cursor);
   _sc_console_draw_region(cons, oldx, oldy, oldw, oldh);
   _sc_console_draw_region(cons, cursor->x, cursor->y, cursor->width, cursor->height);

}
