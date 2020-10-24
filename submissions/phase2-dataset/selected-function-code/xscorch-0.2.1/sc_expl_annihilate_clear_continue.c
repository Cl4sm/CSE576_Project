bool sc_expl_annihilate_clear_continue(sc_config *c, sc_explosion *e) {
/* sc_expl_annihilate_clear_continue
   Continue to clear someone else's explosions.  If this function returns
   "true", then you need to call annihilate_clear_continue to continue
   processing the clearing of the explosion; if it returns "false", then
   the explosion is completely done.  */

   /* Sanity check */
   if(c == NULL || e == NULL) return(false);

   /* Action depends on weapon type */
   switch(e->type) {
      case SC_EXPLOSION_SPIDER:
      case SC_EXPLOSION_NORMAL:
      case SC_EXPLOSION_PLASMA:
         /* Continue to clear the circular explosion */
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

      case SC_EXPLOSION_NAPALM:
      case SC_EXPLOSION_LIQ_DIRT:
      case SC_EXPLOSION_DIRT:
      case SC_EXPLOSION_RIOT:
         /* Nothing to do */
         break;

   } /* End switch on explosion type */

   return(false);

}
