
   bool result;

   /* Process ``init'' queue */
   result = sc_game_expl_queue_do_runnable(c, g, g->expl_init, _sc_state_run_draw_expl);

   /* Process ``draw'' queue */
   result = sc_game_expl_queue_do_runnable(c, g, g->expl_draw, _sc_state_run_draw_expl_cont) | result;

   /* Process ``clear_init'' queue */
   result = sc_game_expl_queue_do_runnable(c, g, g->expl_clear_init, _sc_state_run_clear_expl) | result;

   /* Process ``clear'' queue */
   result = sc_game_expl_queue_do_runnable(c, g, g->expl_clear, _sc_state_run_clear_expl_cont) | result;

   /* Process ``done'' queue */
   result = sc_game_expl_queue_do_runnable(c, g, g->expl_done, _sc_state_run_land_fall) | result;

   /* Were any actions taken? */
   return(result);

}
