static void _sc_console_draw_frame(ScConsole *cons) {
/* sc_console_draw_frame
   Draws the window border for this console, if applicable, and draws
   the scrollbars for this console if they need to be displayed.  */

   GtkWidget *widget = (GtkWidget *)cons;
   GdkGC *foreground;
   GdkGC *background;

   assert(IS_SC_CONSOLE(cons));

   /* Can we even draw yet? */
   if(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)) == NULL) return;

   /* Request a GC */
   foreground = gdk_gc_new(widget->window);
   background = gdk_gc_new(widget->window);

   /* setup gc colors */
   gdk_gc_set_foreground(foreground, &cons->colors.foreground);
   gdk_gc_set_foreground(background, &cons->colors.background);

   /* Clear the screen; draw window border if appropriate */
   gdk_draw_rectangle(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                      background,
                      TRUE,
                      0, 0,
                      widget->allocation.width, widget->allocation.height);
   if(cons->style != CONSOLE_BORDERLESS) {
      /* Draw a border as well... */
      gdk_draw_rectangle(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                         foreground,
                         FALSE,
                         _sc_console_char_width(cons) - 4, _sc_console_char_height(cons) / 2 - 2,
                         widget->allocation.width - 2 * _sc_console_char_width(cons) + 8,
                         widget->allocation.height - _sc_console_char_height(cons) + 4);

      /* Did we need scrollers? */
      if(cons->text.scrollx) {
         _sc_console_draw_horiz_scroll(cons);
      } /* horizontal scrollbar */
      if(cons->text.scrolly) {
         _sc_console_draw_vert_scroll(cons);
      } /* vertical scrollbar */
   } /* Draw the window border? */

   /* Release the GC's */
   g_object_unref(foreground);
   g_object_unref(background);

}
