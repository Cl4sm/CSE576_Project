void sc_player_advance_power(const sc_config *c, sc_player *p, int delta) {
/* sc_player_advance_power
   Modifies power output by the delta given. */

   int life;

   /* Sanity checks */
   if(c == NULL || p == NULL) return;
   
   /* Figure out current life */
   life = INT_ROUND_DIV(p->life, p->tank->hardness);

   /* Update power */
   p->power += delta;
   if(p->power > SC_PLAYER_MAX_POWER) p->power = SC_PLAYER_MAX_POWER;
   if(p->power > life) p->power = life;
   if(p->power < 0) p->power = 0;

   /* Update state */
   #if USE_NETWORK
      sc_net_client_send_orders(c, c->client, p->index);
   #endif
   sc_status_update(c->window, p);

}
