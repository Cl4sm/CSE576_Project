                                 gint x, gint y, gint width, gint height) {
/* sc_console_highlight_attach
   Attach a new highlight to the console, with the indicated colours and
   font weight, and indicated bounding box.  If either colour is NULL,
   then the default console text colours will be used.  Highlights are
   used to change the appearance of text in a particular region; they do
   not alter the text itself, however.
   
   The coordinates are relative to the TEXT BUFFER, not the viewport.  */

   ScConsoleHighlight *high;
   
   g_return_if_fail(IS_SC_CONSOLE(cons));

   high = (ScConsoleHighlight *)malloc(sizeof(ScConsoleHighlight));
   g_return_if_fail(high != NULL);

   if(fg != NULL) {
      high->colors.foreground.red   = fg->red;
      high->colors.foreground.green = fg->green;
      high->colors.foreground.blue  = fg->blue;
   } else {
      high->colors.foreground.red   = cons->colors.foreground.red;
      high->colors.foreground.green = cons->colors.foreground.green;
      high->colors.foreground.blue  = cons->colors.foreground.blue;
   }
   if(bg != NULL) {
      high->colors.background.red   = bg->red;
      high->colors.background.green = bg->green;
      high->colors.background.blue  = bg->blue;
   } else {
      high->colors.background.red   = cons->colors.background.red;
      high->colors.background.green = cons->colors.background.green;
      high->colors.background.blue  = cons->colors.background.blue;
   }
   high->colors.colors_alloc = FALSE;
   high->colors.bold = bold;

   high->x = x;
   high->y = y;
   high->width = width;
   high->height= height;

   cons->highlights = g_list_append(cons->highlights, high);
   _sc_console_draw_region(cons, high->x, high->y, high->width, high->height);

}
