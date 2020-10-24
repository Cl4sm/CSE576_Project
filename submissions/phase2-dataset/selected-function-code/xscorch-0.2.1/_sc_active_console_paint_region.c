/* _sc_active_console_paint_region
   This signal is sent to us from sconsole.c to tell us
   when we need to ask our widgets to redraw in a region. */

   ScActiveConsole *cons = SC_ACTIVE_CONSOLE(_cons);
   ScActiveConsoleSpot *spot;
   gint x1, y1, x2, y2;
   GList *cur;

   #if SC_GTK_DEBUG_PAINT
      SC_DEBUG_ENTER("paint region call%s", "");
   #endif /* debug */

   /* Check parent handler FIRST; we want to override them */
   if(parent_class->paint_region != NULL) {
      parent_class->paint_region(_cons, bounds);
   } /* Parent handler */

   /* Find our bounding box in terms of console location, not pixels. */
   x1 = bounds->x;
   y1 = bounds->y;
   x2 = bounds->x + bounds->width  - 1;
   y2 = bounds->y + bounds->height - 1;
   #if SC_GTK_DEBUG_PAINT
      SC_DEBUG_MSG("paint region is %d,%d-%d,%d CHARS", x1, y1, x2, y2);
   #endif /* debug */

   /* Look through the spots for overlapping gadgets to repaint. */
   cur = cons->spots;
   while(cur != NULL) {
      spot = cur->data;

      /* If the spot is inside the bounding box, it may need repainting. */
      if(_sc_rects_overlap_gtk(x1, y1, x2, y2,
                               spot->x, spot->y,
                               spot->x + spot->width - 1,
                               spot->y + spot->height - 1)) {
         /* Tell the gadget to repaint itself. */
         #if SC_GTK_DEBUG_PAINT
            SC_DEBUG_MSG("found a spot to repaint!  %p", spot);
         #endif /* debug */
         if(spot->gadget != NULL) {
            #if SC_GTK_DEBUG_PAINT
               SC_DEBUG_MSG("found a gadget even!  %p", spot->gadget);
            #endif /* debug */
            g_signal_emit_by_name(GTK_OBJECT(spot->gadget), "paint", NULL, NULL);
         }
      }
      cur = cur->next;
   }

}
