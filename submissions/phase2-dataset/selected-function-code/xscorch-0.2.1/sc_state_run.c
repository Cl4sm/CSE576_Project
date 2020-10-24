
   struct timeval curtime;

   /* Check network activity */
   #if USE_NETWORK
      sc_net_client_run(c, c->client);
      sc_net_server_run(c, c->server);
      if(sc_net_client_death(&c->client)) {
         sc_window_message(c->window, "Client Disconnected", "Client was disconnected from the network");
         sc_window_update(c->window);
      }
      if(c->client != NULL && !SC_CONN_IS_OKAY(c->client->server)) {
         /* We deferred this cycle */
         return;
      }
   #endif /* Network activity? */

   sc_game_time(&curtime);
   if(curtime.tv_sec > g->timeout.tv_sec ||
     (curtime.tv_sec == g->timeout.tv_sec && curtime.tv_usec >= g->timeout.tv_usec)) {

      #if SC_STATE_TIMER_DEBUG
         printf("...  state %8x accepted %ld.%06ld\n", g->state, curtime.tv_sec, curtime.tv_usec);
      #endif /* SC_STATE_TIMER_DEBUG */

      #if SC_STATE_DEBUG
         printf("State %8x    ", g->state);
         SC_PROFILE_BEGIN("game_run")
      #endif /* SC_STATE_DEBUG */

      switch(g->state) {



         /***  PRELUDE  ***/


         /* PRELUDE_BEGIN
               -> PRELUDE_IDLE
            Initialise for a new game; display main menu.
         */
         case SC_STATE_PRELUDE_BEGIN:
         case SC_STATE_GAME_END_DONE:
            _sc_state_prelude_begin(c, g);
            break;



         /***  GAME  ***/


         /* GAME_BEGIN
               -> INVENTORY_BEGIN   (immediate)
            When user first presses Enter on the main menu.  Initialize
            configuration: Initialize config, player game data, and just
            about everything else relevant for game state.
         */
         case SC_STATE_GAME_BEGIN:
            /* Initialize configuration for a new GAME */
            sc_config_init_game(c);
            sc_status_message(c->window, "Starting a new game ...");
            #if USE_NETWORK
               if(c->client != NULL) sc_status_message(c->window, "Waiting for other players to begin game ...");
               sc_net_client_sync(c->client, SC_CONN_SYNC_GAME, c->server != NULL);
            #endif /* Network? */
            sc_game_set_state_asap(g, SC_STATE_INVENTORY_BEGIN);
            break;


         /* GAME_END
               -> GAME_END_IDLE
            All rounds have been played, display the final winners.  Pause
            for user input.
         */
         case SC_STATE_GAME_END:
            _sc_state_game_end(c, g);
            break;



         /***  INVENTORY   ***/


         /* INVENTORY_BEGIN
               -> INVENTORY_PL_BEGIN   (immediate)
            Begin inventory; prepare to take inventory for one player.
         */
         case SC_STATE_INVENTORY_BEGIN:
         case SC_STATE_ROUND_END_DONE:
            _sc_state_inventory_begin(c, g);
            break;


         /* INVENTORY_PL_BEGIN
             Human:
               -> INVENTORY_PL_IDLE
             AI:
               -> INVENTORY_PL_DONE
            If this state is entered, then curplayer must be a valid player.
            Purchases for the player indicated.  Loops back to self until all
            players have bought items.  If this is an AI, purchases are made
            now.
         */
         case SC_STATE_INVENTORY_PL_BEGIN:
            _sc_state_inventory_pl_begin(c, g);
            break;


         /* INVENTORY_PL_DONE
               -> INVENTORY_PL_BEGIN   (loop)
               t> AUTO_DEFENSE_LOOP    (terminating condition)
         */
         case SC_STATE_INVENTORY_PL_DONE:
            _sc_state_inventory_pl_done(c, g);
            break;


         /***  PRE-ROUND   ***/


         /* AUTO DEFENSE LOOP */
         case SC_STATE_AUTO_DEFENSE_LOOP:
            _sc_state_auto_defense(c, g);
            break;


         /* LOTTERY_DISPLAY */
         case SC_STATE_LOTTERY_DISPLAY:
            _sc_state_lottery_display(c, g);
            break;


         /***  ROUND    ***/


         /* ROUND_BEGIN
               -> TURN_BEGIN     (immediate)
            Begin a new round:  Inventory has been taken; generate the land,
            place tanks as appropriate, and prepare for Scorch.
         */
         case SC_STATE_ROUND_BEGIN:
            _sc_state_round_begin(c, g);
            break;


         /* ROUND_END
               -> ROUND_END_IDLE (end of round, will loop)
               t> GAME_END       (end of all rounds)
            Round has ended; advance round, and go to inventory or final
            winnings (latter occurs only if all rounds have been played).
            In former, we identify the victor, do economics and final
            winning calculations, and then notify the user who won.
         */
         case SC_STATE_ROUND_END:
            _sc_state_round_end(c, g);
            break;



         /***  TURNS    ***/


         /* TURN_BEGIN
               -> TURN_PL_BEGIN  (normal progression)
               t> ROUND_END      (no one/1 person alive)
            Player turn begin: Setup for each player to take a turn.  Make
            sure to check if the game has already ended `tho (is there a
            victor?)  Make sure times are in sync, and init the turn.
         */
         case SC_STATE_TURN_BEGIN:
         case SC_STATE_TURN_END:
            _sc_state_turn_begin(c, g);
            break;


         /* TURN_PL_BEGIN
             Dead:
               -> TURN_PL_DONE   (cannot play)
             Human:
               -> TURN_PL_IDLE   (take orders)
             AI:
               -> TURN_PL_DONE   (AI actions taken)
            If we get here, we must have a valid curplayer value (it may be
            a dead player, however).  If dead, then this cycle is basically
            skipped; otherwise, humans are given the opportunity to enter
            orders, and AI actions are taken now.
         */
         case SC_STATE_TURN_PL_BEGIN:
            _sc_state_turn_pl_begin(c, g);
            break;


         /* TURN_PL_DONE

            ==Sequential Mode==
               -> RUN_TALK
            This player has given orders; execute those orders if the player
            is living.  If the player is dead, RUN_TALK will fall through
            and proceed to the next player.  The player's weapon is armed
            here.

            ==Synchronous Mode==
               -> TURN_PL_NEXT
            This player has given orders; take orders from the next player.
            If no more players, the next state will fall to RUN_TALK loop.
            The player's weapon is armed here.
         */
         case SC_STATE_TURN_PL_DONE:
            _sc_state_turn_pl_done(c, g);
            break;


         /* TURN_PL_NEXT

            ==Sequential Mode==
               -> TURN_PL_BEGIN     (immediate)
               t> TURN_END          (terminal, immediate)

            ==Synchronous Mode==
               -> TURN_PL_BEGIN     (immediate)
               t> RUN_TALK          (terminal, immediate)
         */
         case SC_STATE_TURN_PL_NEXT:
            _sc_state_turn_pl_next(c, g);
            break;



         /***  RUN   ***/


         /* RUN_TALK

            ==Sequential Mode==
               -> RUN_CREATION      (might be 2s delay)
            This state DOES NOT LOOP.  If this player is not dead and armed,
            then we might allow them to speak (2s delay) or might launch
            their weapon now (immediate).

            ==Synchronous Mode==
               -> RUN_TALK          (might delay, loops)
               t> RUN_CREATION      (out of players)
            This state loops through all players.  If the player is not dead
            and is armed, they may speak at this time (2s delay) or might
            not (loop is immediate).  Once complete, this will yield to the
            weapon creation state.
         */
         case SC_STATE_RUN_TALK:
            _sc_state_run_talk(c, g);
            break;


         /* RUN_CREATION
               -> RUN_WEAPONS          (immediate; no explosions yet)
               -> RUN_EXPLOSION        (immediate; explosions waiting)
            Create all player weapons.  If the weapon creation created
            an explosion immediately, then process the explosion before
            sending any weapons into flight.
         */
         case SC_STATE_RUN_CREATION:
            _sc_state_run_creation(c, g);
            break;


         /* RUN_WEAPONS (TEMP OBS DOC)
               -> RUN_WEAPONS          (weapons still flying)
               -> RUN_EXPLOSION        (might be flying; detonation)
               t> RUN_END              (terminal case; no weapons)
            Simulate the weapons in-flight.
         */
         case SC_STATE_RUN_WEAPONS:
            _sc_state_run_weapons(c, g);
            break;


         /* RUN_EXPLOSION (TEMP OBS DOC)
               -> RUN_EXPLOSION        (this explosion chain not done; wait)
               t> RUN_PLAYER_DROP      (immediate)
            Run an explosion in progress.  The weapon explosion might opt to
            end at this point, or it may decide to continue its trend of
            annihilation for a few more cycles.  This loop terminates when
            there are no explosions remaining to be drawn; then we will
            start clearing explosions after a respectable pause.
         */
         case SC_STATE_RUN_EXPLOSION:
            _sc_state_run_explosion(c, g);
            break;


         /* RUN_PLAYER_DROP (TEMP OBS DOC)
               -> RUN_PLAYER_DROP   (loop)
               t> RUN_PLAYER_DEATH  (terminal)
         */
         case SC_STATE_RUN_PLAYER_DROP:
            _sc_state_run_player_drop(c, g);
            break;


         /* RUN_PLAYER_DEATH (TEMP OBS DOC)
               -> RUN_PLAYER_DEATH
               t> RUN_END
            Check if any players just died.
         */
         case SC_STATE_RUN_PLAYER_DEATH:
            _sc_state_run_player_death(c, g);
            break;


         /* RUN_END

            ==Sequential Mode==
               -> TURN_PL_NEXT      (immediate)

            ==Synchronous Mode==
               -> TURN_END          (immediate)
         */
         case SC_STATE_RUN_END:
            _sc_state_run_end(c, g);
            break;


         /*    End of (known) game states    */


         default:
            sc_game_reinstate(g, SC_DELAY_LONG);
            break;

      }

      #if SC_STATE_DEBUG
         SC_PROFILE_END
      #endif /* SC_STATE_DEBUG */

   }

}
