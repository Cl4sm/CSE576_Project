/* sc_window_draw_all_tanks_gtk
   Draws all player tanks to the screen which are partially or entirely
   within the bounding box given by (x1, y1)-(x2, y2).  All coordinates
   given to this function MUST be real coordinates.  */

   sc_player *p;              /* Current Player pointer */
   int radius;                /* Radius for player */
   int x;                     /* Current player X (land coords) */
   int y;                     /* Current player Y (land coords) */
   int px1;                   /* Player X range left */
   int px2;                   /* Player X range right */
   int i;

   /* Iterate through all players. */
   i = w->c->numplayers - 1;
   while(i >= 0) {
      /* Get player info; make sure they're not dead. */
      p = w->c->players[i];
      if(!p->dead) {
         /* Get player coordinates; see if any part of the tank falls in redraw field. */
         x = p->x;
         y = p->y;
         radius = p->tank->turretradius;
         px1 = x - radius;
         px2 = x + radius;
         if(sc_land_translate_x_range(w->c->land, &px1, &px2)) {
            if(_sc_rects_overlap_gtk(x1, y1, x2, y2, px1, y - radius, px2, y + radius)) {
               _sc_window_draw_tank_gtk(w, p);
            } /* Is tank in redraw range? */
            /* Check for overlap range as well */
            if(sc_land_overlap_x(w->c->land, &px1, &px2)) {
               if(_sc_rects_overlap_gtk(x1, y1, x2, y2, px1, y - radius, px2, y + radius)) {
                  _sc_window_draw_tank_gtk(w, p);
               } /* Is tank in redraw range? */
            } /* Any overlap? */
         } /* Range translation successful? */
      } /* Is player not dead? */
      --i;
   } /* Loop through players. */
   
}
