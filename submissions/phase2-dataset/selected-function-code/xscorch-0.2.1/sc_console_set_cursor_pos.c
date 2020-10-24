void sc_console_set_cursor_pos(ScConsole *cons, gint x, gint y) {
/* sc_console_set_cursor_pos
   Modifies the cursor position to the CHARACTER coordinates (x,y) (which are
   relative to the TEXT BUFFER, not the viewport).  The cursor's width and
   height are left unaltered.  The viewport may be altered to ensure that the
   cursor is visible within the viewport.  */

   ScConsoleCursor *cursor;   /* Pointer to the cursor structure */
   gint oldx;                 /* Original cursor position X */
   gint oldy;                 /* Original cursor position Y */

   g_return_if_fail(IS_SC_CONSOLE(cons));

   /* Get the cursor */
   cursor = &cons->cursor;

   /* Update the position */
   oldx = cursor->x;
   oldy = cursor->y;
   cursor->x = x;
   cursor->y = y;

   /* Update the display.  Make sure the cursor is on the screen somewhere. */
   _sc_console_lock_view_to_cursor(cons, cursor);
   _sc_console_draw_region(cons, oldx, oldy, cursor->width, cursor->height);
   _sc_console_draw_region(cons, cursor->x, cursor->y, cursor->width, cursor->height);

}
