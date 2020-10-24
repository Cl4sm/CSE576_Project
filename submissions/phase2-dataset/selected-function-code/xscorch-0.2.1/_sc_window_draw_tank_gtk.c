/* sc_window_draw_tank_gtk
   Draws the player's tank to the screen, as well as any shielding which
   might currently be equipped on the player.  */

   GdkPixmap *buffer;         /* Screen buffer */
   GdkGC *gc;                 /* Screen gc */
   sc_gradient_list grad;     /* Gradient for shields */
   unsigned char *data;       /* Profile data pointer */
   int radius;                /* Tank's radius */
   int index;                 /* Shield gradient index */
   int size;                  /* Size = radius * 2 + 1 */
   int tx1;                   /* Translate interval X1 */
   int tx2;                   /* Translate interval X2 */
   int cx;                    /* Current X coord for draw */
   int cy;                    /* Current Y coord for draw */
   int tx;                    /* Translated X coord */
   int ty;                    /* Translated Y corod */
   int x;                     /* Player X (screen coords) */
   int y;                     /* Player Y (screen coords) */

   /* Get the buffer and GC to use in drawing. */
   buffer = sc_display_get_buffer(SC_DISPLAY(w->screen));   
   gc = sc_display_get_gc(SC_DISPLAY(w->screen));

   /* Get current player coordinates. */
   x = p->x;
   y = w->c->fieldheight - p->y - 1;
   radius = p->tank->radius;
   size = radius + radius + 1;
      
   /* Draw tank shields (if available) */
   if(p->shield != NULL) {
      /* Determine which gradient to use */
      if(SC_ACCESSORY_SHIELD_IS_FORCE(p->shield->info)) {
         grad = SC_GRAD_FORCE;
      } else if(SC_ACCESSORY_SHIELD_IS_MAGNETIC(p->shield->info)) {
         grad = SC_GRAD_MAGNETIC;
      } else {
         grad = SC_GRAD_SHIELD;
      }

      /* Draw the shield */
      index = w->c->colors->gradsize[grad] * p->shield->life / (p->shield->info->shield + 1);
      gdk_gc_set_foreground(gc, &w->colormap->gradient[grad][index]);
      gdk_draw_arc(buffer, gc, FALSE, 
                   x - radius, y - radius, 
                   2 * radius + 1, 2 * radius + 1, 
                   0, 360 * 64);
      if(SC_SHIELD_IS_MEDIUM(p->shield) || SC_SHIELD_IS_STRONG(p->shield)) {
         gdk_draw_arc(buffer, gc, FALSE, 
                      x - radius - 1, y - radius - 1, 
                      2 * radius + 3, 2 * radius + 3, 
                      0, 360 * 64);
         if(SC_SHIELD_IS_STRONG(p->shield)) {
            gdk_draw_arc(buffer, gc, FALSE, 
                         x - radius - 2, y - radius - 2, 
                         2 * radius + 5, 2 * radius + 5, 
                         0, 360 * 64);
         } /* Extra shielding for strongest shields */
      } /* Extra shielding for stronger shields */
   }

   /* Set foreground pen color. */
   gdk_gc_set_foreground(gc, &w->colormap->pcolors[p->index]);

   /* Draw tank base and body. */
   data = p->tank->data;
   for(cy = -radius; cy <= 0; ++cy) {
      for(cx = -radius; cx <= radius; ++data, ++cx) {
         if(*data != SC_TANK_PROFILE_CLEAR) {
            tx = x + cx;
            ty = y + cy;
            if(sc_land_translate_x(w->c->land, &tx)) {
               gdk_draw_point(buffer, gc, tx, ty);
            } /* Is this coordinate on the screen? */
         } /* Is this point part of the tank? */
      } /* Iterate over X */
   } /* Iterate over Y */

   /* Draw tank's turret. */
   radius = p->tank->turretradius;
   gdk_draw_line(buffer, gc, 
                 x, y, 
                 (int)(x + rint(radius * cos(p->turret * M_PI / 180))), 
                 (int)(y - rint(radius * sin(p->turret * M_PI / 180))));

   /* Make sure all of this gets drawn. */
   tx1 = x - radius;
   tx2 = x + radius;
   if(sc_land_translate_x_range(w->c->land, &tx1, &tx2)) {
      sc_display_queue_draw(SC_DISPLAY(w->screen), 
                            tx1, y - radius, 
                            tx2 - tx1 + 1, size);
      if(sc_land_overlap_x(w->c->land, &tx1, &tx2)) {
         sc_display_queue_draw(SC_DISPLAY(w->screen), 
                               tx1, y - radius, 
                               tx2 - tx1 + 1, size);
      }
   }

}
