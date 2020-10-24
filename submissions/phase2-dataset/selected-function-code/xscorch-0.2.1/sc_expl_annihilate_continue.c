/* sc_expl_annihilate_continue
   Continue to annihilate a section of the screen by drawing a huge explosion
   to it.  This function returns "true" if it needs to be called again to
   continue.  If it returns "false", then the explosion has been completely
   processed and you may call annihilate_clear.  */

   sc_expl_cache *ca;   /* Explosion cache (standard explosions) */
   sc_spill *sp;        /* Spill information (liquid explosions) */

   /* Sanity check */
   if(c == NULL || e == NULL) return(false);

   /* Action depends on weapon type */
   switch(e->type) {
      case SC_EXPLOSION_SPIDER:
         /* nothing special to animate spider explosions */

      case SC_EXPLOSION_NORMAL:
      case SC_EXPLOSION_PLASMA:
         /* Get a new explosion cache ID, and draw it */
         ca = e->cache;
         if(ca != NULL) {
            ca->eradius += SC_EXPL_EXPLOSION_STEP;
            if(ca->eradius >= e->radius) ca->eradius = e->radius;
            sc_expl_cache_draw(c->window, ca->cacheid,
                               e->centerx, e->centery,
                               ca->eradius);
            if(c->physics->walls == SC_WALL_WRAP) {
               if(e->centerx + e->radius >= c->fieldwidth) {
                  sc_expl_cache_draw(c->window, ca->cacheid,
                                     e->centerx - c->fieldwidth,
                                     e->centery, ca->eradius);
               } else if(e->centerx - e->radius < 0) {
                  sc_expl_cache_draw(c->window, ca->cacheid,
                                     e->centerx + c->fieldwidth,
                                     e->centery, ca->eradius);
               } /* Did the explosion wrap off-screen? */
            } /* Were boundaries wrap-around? */

            /* Done animating? */
            if(ca->eradius >= e->radius) {
               free(ca);
               return(false);
            }

            /* We still need to animate */
            return(true);
         } /* Only executed if not in fast mode */
         return(false);

      case SC_EXPLOSION_NAPALM:
         /* Construct the napalm spill */
         if(SC_CONFIG_NO_ANIM(c)) return(false);
         sp = (sc_spill *)e->data;
         if(sp != NULL) {
            if(sp->count > 0) {
               sc_window_draw_napalm_final(c->window, sp->spillx, sp->spilly, sp->size);
               ++sp->count;
               if(sp->count > SC_EXPL_NAPALM_FLAMES) return(false);
            } else if(sp->index < sp->size) {
               sc_window_draw_napalm_frame(c->window,
                                           sp->spillx + sp->index,
                                           sp->spilly + sp->index,
                                           min(sp->size - sp->index, SC_EXPL_LIQUID_STEP));
               sp->index += SC_EXPL_LIQUID_STEP;
            } else {
               sp->count = 1;
            } /* Do we still have business to take care of? */
         } /* Only attempt the draw if spill pointer valid */
         return(true);

      case SC_EXPLOSION_LIQ_DIRT:
         /* Construct the liquid dirt spill */
         if(SC_CONFIG_NO_ANIM(c)) return(false);
         sp = (sc_spill *)e->data;
         if(sp != NULL) {
            if(sp->index < sp->size) {
               sc_land_create_dirt(c, c->land,
                                   sp->spillx + sp->index,
                                   sp->spilly + sp->index,
                                   min(sp->size - sp->index, SC_EXPL_LIQUID_STEP));
               sp->index += SC_EXPL_LIQUID_STEP;
            } else {
               return(false);
            } /* Do we still have business to take care of? */
         } /* Only attempt the draw if spill pointer valid */
         return(true);

      case SC_EXPLOSION_DIRT:
      case SC_EXPLOSION_RIOT:
         /* Continue to clear a circular explosion */
         e->idraw += SC_EXPL_EXPLOSION_STEP;
         if(e->idraw >= e->radius) {
            sc_window_paint(c->window,
                            e->centerx - e->radius,
                            e->centery - e->radius,
                            e->centerx + e->radius,
                            e->centery + e->radius,
                            SC_PAINT_EVERYTHING);
            return(false);
         } else {
            sc_window_paint_circular(c->window,
                                     e->centerx, e->centery,
                                     e->idraw,
                                     SC_PAINT_EVERYTHING);
            return(true);
         }
         break;

   } /* End switch on explosion type */

   return(false);

}
