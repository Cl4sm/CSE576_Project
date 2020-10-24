/* sc_console_set_colors
   Modify the normal foreground/background colours for this console.
   This does not alter the colour attributes for highlights or the
   cursor.  If either colour is NULL, then the console's current colour
   is preserved.  */

   g_return_if_fail(IS_SC_CONSOLE(cons));

   if(fg != NULL) {
      cons->colors.foreground.red   = fg->red;
      cons->colors.foreground.green = fg->green;
      cons->colors.foreground.blue  = fg->blue;
   }
   if(bg != NULL) {
      cons->colors.background.red   = bg->red;
      cons->colors.background.green = bg->green;
      cons->colors.background.blue  = bg->blue;
   }
   cons->colors.colors_alloc = FALSE;
   cons->colors.bold = FALSE;
   _sc_console_draw_all(cons);

}
