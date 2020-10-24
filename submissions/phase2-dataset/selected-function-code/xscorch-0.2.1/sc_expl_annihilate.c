/* sc_expl_annihilate
   Annihilate a section of the screen by drawing a huge explosion to it.
   This function is called to initiate animation of the explosion and
   update the landmass, indicating the area destroyed by the explosion.
   If it returns "true", then the caller should call annihilate_continue
   at some point to continue processing the explosion in an "animated"
   manner.  If it returns "false", then the explosion has been completely
   dealt with, and you are ready to call annihilate_clear.  */

   sc_expl_cache *ca;   /* Explosion cache (standard explosions) */
   sc_spill *sp;        /* Spill information (liquid explosions) */

   /* Sanity check */
   if(c == NULL || e == NULL) return(false);

   /* Action depends on weapon type */
   switch(e->type) {
      case SC_EXPLOSION_SPIDER:
         /* if there is an arc to draw, draw it */
         if(e->data != NULL)
            sc_window_draw_arc(c->window, e->data, e->playerid);
         else
            fprintf(stderr, "BUG: spider with null leg arc.\n");
         /* fall through to explosion drawing */

      case SC_EXPLOSION_NORMAL:
      case SC_EXPLOSION_PLASMA:
         /* Get a new explosion cache ID, and draw it */
         if(!SC_CONFIG_GFX_FAST(c)) {
            ca = (sc_expl_cache *)malloc(sizeof(sc_expl_cache));
            if(ca == NULL) return(false);
            if(SC_CONFIG_ANIM(c)) ca->eradius = 0;
            else ca->eradius = e->radius;
            ca->cacheid = sc_expl_cache_new(c->window, e->radius, e->type);
            e->cache = ca;
            return(true);
         } /* Only executed if not in fast mode */
         return(false);

      case SC_EXPLOSION_NAPALM:
         /* Retrieve the napalm spill */
         sp = (sc_spill *)e->data;
         if(!SC_CONFIG_GFX_FAST(c) && sp != NULL) {
            if(SC_CONFIG_ANIM(c)) {
               sc_window_draw_napalm_frame(c->window,
                                           sp->spillx, sp->spilly,
                                           min(sp->size, SC_EXPL_LIQUID_STEP));
               sp->index = SC_EXPL_LIQUID_STEP;
            } else {
               sc_window_draw_napalm_frame(c->window, sp->spillx, sp->spilly, sp->size);
               sp->index = sp->size + 1;
            }
            return(true);
         } /* Only attempt the draw if spill pointer valid */
         return(false);

      case SC_EXPLOSION_LIQ_DIRT:
         /* Retrieve the liquid dirt spill */
         sp = (sc_spill *)e->data;
         if(!SC_CONFIG_GFX_FAST(c) && sp != NULL) {
            if(SC_CONFIG_ANIM(c)) {
               sc_land_create_dirt(c, c->land,
                                   sp->spillx, sp->spilly,
                                   min(sp->size, SC_EXPL_LIQUID_STEP));
               sp->index = SC_EXPL_LIQUID_STEP;
            } else {
               sc_land_create_dirt(c, c->land, sp->spillx, sp->spilly, sp->size);
               sp->index = sp->size + 1;
            }
            return(true);
         } /* Only attempt the draw if spill pointer valid */
         return(false);

      case SC_EXPLOSION_DIRT:
         /* Animate a circular explosion that is filling up */
         _sc_expl_annihilate_fill(c, e, e->radius);
         if(!SC_CONFIG_GFX_FAST(c)) {
            /* Only do slow animation if not in fast mode */
            e->idraw = 0;
            return(true);
         } else {
            /* Just update everything at once */
            sc_window_paint(c->window,
                            e->centerx - e->radius,
                            e->centery - e->radius,
                            e->centerx + e->radius,
                            e->centery + e->radius,
                            SC_PAINT_EVERYTHING);
         } /* Only executed if not in fast mode */
         break;

      case SC_EXPLOSION_RIOT:
         /* Animate a circular explosion that is clearing out */
         _sc_expl_annihilate_clear(c, e, e->radius);
         if(!SC_CONFIG_GFX_FAST(c)) {
            /* Only do slow animation if not in fast mode */
            e->idraw = 0;
            return(true);
         } else {
            /* Just update everything at once */
            sc_window_paint(c->window,
                            e->centerx - e->radius,
                            e->centery - e->radius,
                            e->centerx + e->radius,
                            e->centery + e->radius,
                            SC_PAINT_EVERYTHING);
         } /* Only executed if not in fast mode */
         break;

   } /* End switch on explosion type */

   return(false);

}
