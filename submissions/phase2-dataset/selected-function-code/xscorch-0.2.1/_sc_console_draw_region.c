static void _sc_console_draw_region(ScConsole *cons, gint x, gint y, gint width, gint height) {
/* sc_console_draw_region
   Draws a region of the console, specified by the rectangle using CHARACTER
   coordinates, with corner (x,y) and the indicated width and height.  This
   function uses information about active highlights and the text buffer to
   determine what to draw.  width/height may be negative, in which case the
   rectangle is flipped appropriately.  
   
   The coordinates are CHARACTER coordinates, but they are relative to the
   CHARACTER BUFFER, not the viewport.  */

   ScConsoleHighlight *high;  /* Active highlight for current position */
   GdkColor *oldfgcolor;      /* Previous foreground colour (to speed up GC ops) */
   GdkColor *oldbgcolor;      /* Previous background colour (to speed up GC ops) */
   GdkRectangle region;       /* Rectangle representing redraw region */
   GdkColor *fgcolor;         /* Current foreground colour */
   GdkColor *bgcolor;         /* Current background colour */
   GdkGC *fg_gc;              /* Foreground GC */
   GdkGC *bg_gc;              /* Background GC */
   GList *cur;                /* List to iterate the highlights */

   char *chptr;               /* Pointer to current character to draw */
   gint x1;                   /* Left X boundary */
   gint y1;                   /* Top Y boundary  */
   gint x2;                   /* Right X boundary */
   gint y2;                   /* Bottom Y boundary */
   gint cx;                   /* Current X iter */
   gint cy;                   /* Current Y iter */
   gboolean bold;             /* Draw this in bold? */

   assert(IS_SC_CONSOLE(cons));

   if(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)) == NULL) return;

   /* Set up the region variable. */
   region.x = x;
   region.y = y;
   region.width = width;
   region.height = height;

   x1 = x;
   y1 = y;
   x2 = x + width - 1;
   y2 = y + height - 1;

   /* Check to see if width/height were negative, and swap if needed */
   if(x1 > x2) {
      x1 = x1 + x2;
      x2 = x1 - x2;
      x1 = x1 - x2;
   }
   if(y1 > y2) {
      y1 = y1 + y2;
      y2 = y1 - y2;
      y1 = y1 - y2;
   }

   /* Clip the bottom/left coordinates based on extents of the text buffer */
   if(x1 < cons->text.viewx) x1 = cons->text.viewx;
   if(y1 < cons->text.viewy) y1 = cons->text.viewy;
   if(x1 >= cons->text.viewx + cons->text.vieww) return;
   if(y1 >= cons->text.viewy + cons->text.viewh) return;

   /* Clip the top/right coordinates based on extents of the text buffer */
   if(x2 < cons->text.viewx) return;
   if(y2 < cons->text.viewy) return;
   if(x2 >= cons->text.viewx + cons->text.vieww) x2 = cons->text.viewx + cons->text.vieww - 1;
   if(y2 >= cons->text.viewy + cons->text.viewh) y2 = cons->text.viewy + cons->text.viewh - 1;

   /* Request a GC */
   fg_gc = gdk_gc_new(((GtkWidget *)cons)->window);
   bg_gc = gdk_gc_new(((GtkWidget *)cons)->window);

   /* Setup default foreground, background colors */
   oldfgcolor = &cons->colors.foreground;
   oldbgcolor = &cons->colors.background;
   gdk_gc_set_foreground(fg_gc, oldfgcolor);
   gdk_gc_set_foreground(bg_gc, oldbgcolor);

   /* Start printing characters... */
   for(cy = y1; cy <= y2; ++cy) {

      /* Get the character to be drawn */
      chptr = _sc_console_get_char_ptr(cons, x1, cy);
      g_return_if_fail(chptr != NULL);
      for(cx = x1; cx <= x2; ++cx, ++chptr) {

         fgcolor = &cons->colors.foreground;
         bgcolor = &cons->colors.background;
         bold = cons->colors.bold;

         /* What highlight are we using? */
         cur = cons->highlights;
         while(cur != NULL) {
            /* Are we on this highlight? */
            high = cur->data;
            assert(high != NULL);
            cur = cur->next;
            if(_sc_console_in_bounds(cx, cy, high->x, high->y, high->width, high->height)) {
               /* We are on this highlight; set new fg/bg color */
               if(!high->colors.colors_alloc) {
                  high->colors.colors_alloc = TRUE;
                  gdk_colormap_alloc_color(gtk_widget_get_colormap((GtkWidget *)cons),
                                           &high->colors.foreground, FALSE, TRUE);
                  gdk_colormap_alloc_color(gtk_widget_get_colormap((GtkWidget *)cons),
                                           &high->colors.background, FALSE, TRUE);
               }
               fgcolor = &high->colors.foreground;
               bgcolor = &high->colors.background;
               bold = high->colors.bold;
            } /* Coordinates in bound? */
         } /* Iterating thru installed highlights ... */

         /* Are we drawing the new cursor? */
         if(_sc_console_in_bounds(cx, cy, cons->cursor.x, cons->cursor.y,
                                  cons->cursor.width, cons->cursor.height)) {
            /* Welp, we're on the cursor.. all that work, for nothing */
            if(gtk_widget_has_focus((GtkWidget *)cons)) {
               if(cons->cursor.highlighted) {
                  fgcolor = &cons->colors.forelight;
                  bgcolor = &cons->colors.backlight;
               } else {
                  fgcolor = &cons->colors.forecursor;
                  bgcolor = &cons->colors.backcursor;
               }
            } else {
               fgcolor = &cons->colors.foreshadow;
               bgcolor = &cons->colors.backshadow;
            }
         } /* We be a cursor? */

         /* Only update the GC if we absolutely have to */
         if(oldfgcolor != fgcolor) {
            oldfgcolor = fgcolor;
            gdk_gc_set_foreground(fg_gc, fgcolor);
         }
         if(oldbgcolor != bgcolor) {
            oldbgcolor = bgcolor;
            gdk_gc_set_foreground(bg_gc, bgcolor);
         }

         _sc_console_draw_char(cons, fg_gc, bg_gc, bold,
                               cx - cons->text.viewx, cy - cons->text.viewy, *chptr);
      } /* Iterating thru X */
   } /* Iterate thru Y */

   /* Update X, Y to account for a window frame; also,
      set X, Y to the screen coordinates to write to. */
   sc_console_get_pixel_from_char(cons, &x, &y, TRUE);

   /* Update width, height to screen coordinates */
   width = width * _sc_console_char_width(cons);
   height= height* _sc_console_char_height(cons);

   /* Make sure this update is queued for display to screen. */
   sc_drawbuf_queue_draw(SC_DRAWBUF(cons), x, y, width, height);

   /* Propagate the draw request to the active console. */
   g_signal_emit_by_name(GTK_OBJECT(cons), "paint_region", &region, NULL);

   /* Release the GC's */
   g_object_unref(fg_gc);
   g_object_unref(bg_gc);

}
