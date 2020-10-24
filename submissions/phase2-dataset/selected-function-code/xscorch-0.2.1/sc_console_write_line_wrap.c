void sc_console_write_line_wrap(ScConsole *cons, gint x, gint y, const char *line) {
/* sc_console_write_line_wrap
   Writes a line of text starting at character position (x,y).  If the
   line overflows the width of the text buffer, then it is continued
   on the next line starting in column 0.  If the text overflows the
   height of the text buffer in this manner, then it will be truncated.
   
   It is considered an error if x < 0 or y < 0.  If x is beyond the
   width of the line, then printing will start on the next line.  */

   gint width;
   gint height;
   gint minx;
   char *p;
   
   g_return_if_fail(IS_SC_CONSOLE(cons));
   g_return_if_fail(line != NULL);
   g_return_if_fail(x >= 0 && y >= 0);

   width = sc_console_get_width(cons);
   height= sc_console_get_height(cons);
   if(x >= width) {
      x = 0;
      ++y;
   }
   if(y >= height) return;

   minx = x;
   p = _sc_console_get_char_ptr(cons, x, y);
   g_return_if_fail(p != NULL);
   while(y < height && *line != '\0') {
      *p = *line;
      ++line;
      ++p;
      ++x;

      if(x >= width) {
         _sc_console_draw_region(cons, minx, y, x - minx, 1);
         minx = 0;
         x = 0;
         ++y;
      }
   }

   if(x > minx) {
      _sc_console_draw_region(cons, minx, y, x - minx, 1);
   }

}
