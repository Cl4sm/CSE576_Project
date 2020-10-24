void sc_player_set_position(const sc_config *c, sc_player *p, int x, int y) {
/* sc_player_set_position
   The coordinate (x, y) indicates the new player position, in virtual
   coordinates.  This function always succeeds and performs no sanity
   checks (i.e. to make sure player can actually OCCUPY said position) */

   /* Sanity checks */
   if(c == NULL || p == NULL) return;

   /* Make sure tank has actually moved */
   if(p->x == x && p->y == y) return;

   /* Update tank position on screen */
   sc_window_undraw_tank(c->window, p);
   p->x = x;
   p->y = y;
   sc_window_draw_tank(c->window, p);

   /* Transmit state by network */
   #if USE_NETWORK
      sc_net_client_send_orders(c, c->client, p->index);
   #endif /* Network? */

}
