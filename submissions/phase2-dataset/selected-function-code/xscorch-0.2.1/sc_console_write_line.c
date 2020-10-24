void sc_console_write_line(ScConsole *cons, gint x, gint y, const char *line) {
/* sc_console_write_line
   Writes a line of text starting at character position (x,y).  If the
   line overflows the width of the text buffer, then the line is
   truncated.  If x < 0, then the first (-x) characters of line will
   be trimmed, and printing will begin in column 0.  */

   gint width;
   const char *start;
   const char *end;
   char *p;

   g_return_if_fail(IS_SC_CONSOLE(cons));
   g_return_if_fail(line != NULL);

   width = sc_console_get_width(cons);
   if(y < 0 || y >= sc_console_get_height(cons)) return;

   for(start = line; *start != '\0' && (line - start) + x < 0; ++start) /* Just loop */;
   for(end = start; *end != '\0' && (end - start) + x < width; ++end) /* Just loop */;
   if(end == start) return;

   p = _sc_console_get_char_ptr(cons, x, y);
   g_return_if_fail(p != NULL);
   memcpy(p, start, end - start);
   _sc_console_draw_region(cons, x, y, end - start, 1);

}
