
   ScConsole *cons;
   gint x, y;

   if(bounds == NULL) return;
   bounds->width = bounds->height = 0;

   if(gadget == NULL) return;
   if(gadget->spot == NULL) return;
   if(gadget->console == NULL) return;
   cons = SC_CONSOLE(gadget->console);

   /* Determine the pixel locations. */
   x = gadget->spot->x;
   y = gadget->spot->y;
   sc_console_get_pixel_from_char(cons, &x, &y, TRUE);

   /* Update the result rectangle */
   bounds->x      = x;
   bounds->y      = y;
   bounds->width  = sc_console_get_col_width (cons) * gadget->spot->width;
   bounds->height = sc_console_get_row_height(cons) * gadget->spot->height;

}
