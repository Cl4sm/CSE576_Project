/* sc_console_lock_view_to_cursor
   Forces the cursor to appear within the viewport by altering the viewport
   if necessary.  */

   gboolean needrewrite = FALSE;
   
   assert(IS_SC_CONSOLE(cons));
   assert(cursor != NULL);
   
   if(cursor->x < cons->text.viewx) {
      cons->text.viewx = cursor->x;
      _sc_console_draw_horiz_scroll(cons);
      needrewrite = TRUE;
   } else if(cursor->x + cursor->width > cons->text.viewx + cons->text.vieww) {
      cons->text.viewx = cursor->y - (cons->text.vieww - cursor->width);
      _sc_console_draw_horiz_scroll(cons);
      needrewrite = TRUE;
   }

   if(cursor->y < cons->text.viewy) {
      cons->text.viewy = cursor->y;
      _sc_console_draw_vert_scroll(cons);
      needrewrite = TRUE;
   } else if(cursor->y + cursor->height > cons->text.viewy + cons->text.viewh) {
      cons->text.viewy = cursor->y - (cons->text.viewh - cursor->height);
      _sc_console_draw_vert_scroll(cons);
      needrewrite = TRUE;
   }

   if(needrewrite) {
      _sc_console_draw_region(cons, 0, 0, sc_console_get_width(cons), sc_console_get_height(cons));
   }

}
