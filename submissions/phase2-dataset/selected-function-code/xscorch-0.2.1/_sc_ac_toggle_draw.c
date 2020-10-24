/* _sc_ac_toggle_draw
   Draw a toggle, depressed or not. */

   ScACToggle *toggle = SC_AC_TOGGLE(gadget);
   ScActiveConsole *cons = gadget->console;
   GtkWidget *widget = (GtkWidget *)cons;
   gboolean focus, sensitive;
   GdkGC *foreground;
   GdkRectangle bounds;
   /* TEMP - We want to use a real palette color (see below). */
   GdkColor color;

   /* Can we even draw yet? */
   if(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)) == NULL) return;

   /* Request a GC. */
   foreground = gdk_gc_new(widget->window);

   /* Figure out our extents */
   sc_gadget_get_extents(gadget, &bounds);

   /* Find out if this gadget has the focus right now. */
   focus = (gtk_widget_has_focus((GtkWidget *)gadget->console) &&
            (gadget->console->current->data == gadget->spot))
           ? TRUE : FALSE;

   /* Find out if the gadget will be sensitive. */
   sensitive = gtk_widget_is_sensitive(GTK_WIDGET(gadget->console));

   /* Setup the GC colors for the toggle (drawing outer). */
   if(!sensitive)
      gdk_gc_set_foreground(foreground, sc_console_get_color(SC_CONSOLE(cons), SC_CONSOLE_FOREDISABLED));
   else if(focus)
      gdk_gc_set_foreground(foreground, sc_console_get_color(SC_CONSOLE(cons), SC_CONSOLE_FORECURSOR));
   else
      gdk_gc_set_foreground(foreground, sc_console_get_color(SC_CONSOLE(cons), SC_CONSOLE_FOREGROUND));

   /* Draw the outer circle of the toggle. */
   gdk_draw_arc(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                foreground, TRUE,
                bounds.x, bounds.y,
                bounds.width - 1, bounds.height - 1,
                0, 360 * 64);

#ifndef ___TEMP_toggle_color
   /*
    * TEMP - We want to use a real palette color here...
    * However, we haven't gotten around to deciding exactly
    * what colors go in the palette for consoles or even
    * how to make that decision, so for now we temporarily
    * allocate our own here in the toggle.  This should
    * definitely be considered a rather sad hack in nature.
    */
   if(!sensitive)
      gdk_color_parse("#444444", &color);
   else if(focus)
      gdk_color_parse("#886666", &color);
   else
      gdk_color_parse("#777777", &color);

   gdk_colormap_alloc_color(gtk_widget_get_colormap((GtkWidget *)cons), &color, FALSE, TRUE);
   gdk_gc_set_foreground(foreground, &color);
#else
   /* Setup the GC colors for the toggle (drawing center). */
   if(!sensitive)
      gdk_gc_set_foreground(foreground, sc_console_get_color(SC_CONSOLE(cons), SC_CONSOLE_BACKDISABLED));
   else if(focus)
      gdk_gc_set_foreground(foreground, sc_console_get_color(SC_CONSOLE(cons), SC_CONSOLE_BACKCURSOR));
   else
      gdk_gc_set_foreground(foreground, sc_console_get_color(SC_CONSOLE(cons), SC_CONSOLE_BACKGROUND));
#endif

   /* Draw the interior lighted section of the toggle. */
   gdk_draw_arc(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                foreground, TRUE,
                bounds.x + bounds.width * 1 / 8, bounds.y + bounds.height * 1 / 8,
                bounds.width * 3 / 4, bounds.height * 3 / 4,
                0, 360 * 64);

   /* If the toggle is depressed, give it some Prozac.
      No, seriously, draw its little filled inner circle. */
   if(toggle->state) {
      /* Setup the GC colors for the toggle (drawing inner). */
      if(!sensitive)
         gdk_gc_set_foreground(foreground, sc_console_get_color(SC_CONSOLE(cons), SC_CONSOLE_FOREDISABLED));
      else if(focus)
         gdk_gc_set_foreground(foreground, sc_console_get_color(SC_CONSOLE(cons), SC_CONSOLE_FORECURSOR));
      else
         gdk_gc_set_foreground(foreground, sc_console_get_color(SC_CONSOLE(cons), SC_CONSOLE_FOREGROUND));

      /* And then draw it. */
      gdk_draw_arc(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                   foreground, TRUE,
                   bounds.x + bounds.width * 1 / 3, bounds.y + bounds.height * 1 / 3,
                   bounds.width * 1 / 3, bounds.height * 1 / 3,
                   0, 360 * 64);
   }

   /* Release the GC. */
   g_object_unref(foreground);

   /* Make sure everything is queued to be drawn. */
   sc_drawbuf_queue_draw(SC_DRAWBUF(widget), bounds.x, bounds.y, bounds.width, bounds.height);

}
