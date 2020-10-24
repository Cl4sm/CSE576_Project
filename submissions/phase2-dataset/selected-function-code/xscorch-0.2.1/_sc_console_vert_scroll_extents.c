static inline void _sc_console_vert_scroll_extents(ScConsole *cons,
                                                   gint *startx, gint *starty,
                                                   gint *width, gint *height,
                                                   gint *arrowh) {
/* sc_console_vert_scroll_extents
   Figure out the extents of the vertical scrollbar.  The arguments are
      startx, starty: bottomleft coordinate of scrollbar body.  The starty
                      is ABOVE the bottom arrow.
      width, height:  total dimensions of the scrollbar body.  The height
                      does NOT include the height of the 2 arrows.
      arrowh:         height of each arrow (the width of arrow is == width).
 */

   *width  = ((_sc_console_char_width(cons) + 2) & ~1);
   *height = GTK_WIDGET(cons)->allocation.height - 3 * _sc_console_char_height(cons);
   *startx = GTK_WIDGET(cons)->allocation.width - 2 * _sc_console_char_width(cons) + 1;
   *starty = _sc_console_char_height(cons) * 3 / 2;
   *arrowh = *width + 2;

}
