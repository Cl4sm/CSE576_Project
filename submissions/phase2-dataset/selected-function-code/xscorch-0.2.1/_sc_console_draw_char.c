/* sc_console_draw_char
   Draws a single character using the indicated fg/bg GCs, and at the
   CHARACTER position (x,y).  The coordinate will be converted to
   pixels automatically in this function.  The character to write is
   specified by this function; this function will select the bold or
   normal font based on the "bold" flag.  
   
   The coordinates are CHARACTER coordinates, but they are with respect
   to the VIEWPORT, not the character buffer.  */

   GdkFont *font;

   assert(IS_SC_CONSOLE(cons));
   assert(fg != NULL);
   assert(bg != NULL);

   /* Which font to select? */
   if(bold) {
      font = cons->screen_bold_font;
   } else {
      font = cons->screen_font;
   }

   /* Update X, Y to account for a window frame; also,
      set X, Y to the screen coordinates to write to. */
   sc_console_get_pixel_from_char(cons, &x, &y, FALSE);

   /* Write the text! */
   gdk_draw_rectangle(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                      bg,
                      TRUE,
                      x, y,
                      _sc_console_char_width(cons), _sc_console_char_height(cons));
   gdk_draw_text(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                 font, fg,
                 x, y + font->ascent,
                 &ch, 1);

}
