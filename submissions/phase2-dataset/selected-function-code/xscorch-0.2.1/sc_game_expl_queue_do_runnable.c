bool sc_game_expl_queue_do_runnable(sc_config *c, sc_game *g, sc_explosion *queue, sc_game_run_fn action) {
/* sc_game_expl_queue_do_runnable
   For each item in the queue whose timestamp is at most the current game
   time, invoke the procedure given exactly once.  Action may remove e
   from the queue but must not take any action that would be destructive
   to the rest of the queue.  */

   sc_explosion *cur;
   bool acted = false;

   if(c == NULL || g == NULL || action == NULL) return(false);

   while(queue != NULL) {
      cur = queue;
      queue = cur->chain;
      if(cur->counter == 0) {
         /* Run this explosion through the action method */
         acted = action(c, g, cur) | acted;
      }
   }

   return(acted);

}
