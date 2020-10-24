/* sc_expl_annihilate_clear
   Clears someone else's explosions.  If this function returns "true", then
   you need to call annihilate_clear_continue to continue processing the
   clearing of the explosion; if it returns "false", then the explosion is
   completely done.  */

   sc_trajectory *tr;
   sc_spill *sp;        /* Spill information (liquid explosions) */

   /* Sanity check */
   if(c == NULL || e == NULL) return(false);

   /* Action depends on weapon type */
   switch(e->type) {
      case SC_EXPLOSION_SPIDER:
        /* if there is an arc to clear, clear it */
        if(e->data != NULL) {
           sc_window_clear_arc(c->window, e->data);
           tr = (sc_trajectory *)e->data;
           sc_traj_landfall(c, tr);
           sc_traj_free(&tr);
        }
        /* fall through to explosion clearing */

      case SC_EXPLOSION_NORMAL:
      case SC_EXPLOSION_PLASMA:
         /* Clear the circular explosion */
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

      case SC_EXPLOSION_NAPALM:
         /* Clean up the napalm spill */
         sp = (sc_spill *)e->data;
         if(!SC_CONFIG_GFX_FAST(c)) {
            sc_window_clear_napalm(c->window, sp->spillx, sp->spilly, sp->size);
         }
         sc_spill_free((sc_spill **)&e->data);
         break;

      case SC_EXPLOSION_LIQ_DIRT:
      case SC_EXPLOSION_RIOT:
      case SC_EXPLOSION_DIRT:
         /* Nothing to do */
         break;

   } /* End switch on explosion type */

   return(false);

}
