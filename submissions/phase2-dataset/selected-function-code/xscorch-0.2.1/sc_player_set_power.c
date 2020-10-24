/* sc_player_set_power */

   int life;

   /* Sanity checks */
   if(c == NULL || p == NULL) return;
   
   /* Figure out current life */
   life = INT_ROUND_DIV(p->life, p->tank->hardness);

   /* Update power; return if no change */
   if(power < 0) power = 0;
   if(power > SC_PLAYER_MAX_POWER) power = SC_PLAYER_MAX_POWER;
   if(power > life) power = life;
   if(p->power == power) return;
   p->power = power;

   /* Update state */
   #if USE_NETWORK
      sc_net_client_send_orders(c, c->client, p->index);
   #endif
   sc_status_update(c->window, p);

}
