static void _sc_expl_annihilate(sc_config *c, const sc_explosion *e,
                                int radius, bool fill) {
/* sc_expl_annihilate
   Clears (if fill == false) or fills (if fill == true) dirt in the
   explosion specified.  This updates the land buffer but it will NOT
   update the display screen.  */

   /* Dispatch to either wedge code or non-wedge code. */
   if(!SC_EXPL_DEBUG_WEDGES && e->angular_width == 0) {
      _sc_expl_annihilate_circular(c, e, radius, fill);
   } else {
      _sc_expl_annihilate_wedge(c, e, radius, fill);
   }

   /* Update the land buffer */
   sc_window_paint(c->window,
                   e->centerx - radius, e->centery - radius,
                   e->centerx + radius, e->centery + radius,
                   SC_REGENERATE_LAND);

}
