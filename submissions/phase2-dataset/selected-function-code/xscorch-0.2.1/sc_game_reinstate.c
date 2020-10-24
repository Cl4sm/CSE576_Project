
   #if SC_STATE_TIMER_DEBUG
      struct timeval curtime;
   #endif

   g->timeout.tv_usec += (delay % 1000) * 1000;
   while(g->timeout.tv_usec >= 1000000) {
      g->timeout.tv_usec -= 1000000;
      g->timeout.tv_sec++;
   }
   g->timeout.tv_sec += (delay / 1000);

   #if SC_STATE_TIMER_DEBUG
      sc_game_time(&curtime);
      printf("Next state %8x activates at %ld.%06ld (%+5ld; current %ld.%06ld)\n",
             g->state, g->timeout.tv_sec, g->timeout.tv_usec, delay, curtime.tv_sec, curtime.tv_usec);
   #endif

}
