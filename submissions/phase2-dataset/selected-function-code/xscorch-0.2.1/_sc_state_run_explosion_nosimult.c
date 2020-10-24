static bool _sc_state_run_explosion_nosimult(sc_config *c, sc_game *g) {

   /* Check ``done'' queue */
   if(g->expl_done != NULL) return(sc_game_expl_queue_run_first(c, g, g->expl_done, _sc_state_run_land_fall));

   /* Check ``clear'' queue */
   if(g->expl_clear != NULL) return(sc_game_expl_queue_run_first(c, g, g->expl_clear, _sc_state_run_clear_expl_cont));

   /* Check ``clear_init'' queue */
   if(g->expl_clear_init != NULL) return(sc_game_expl_queue_run_first(c, g, g->expl_clear_init, _sc_state_run_clear_expl));

   /* Check ``draw'' queue */
   if(g->expl_draw != NULL) return(sc_game_expl_queue_run_first(c, g, g->expl_draw, _sc_state_run_draw_expl_cont));

   /* Check ``init'' queue */
   if(g->expl_init != NULL) return(sc_game_expl_queue_run_first(c, g, g->expl_init, _sc_state_run_draw_expl));

   /* No actions taken */
   return(false);

}
