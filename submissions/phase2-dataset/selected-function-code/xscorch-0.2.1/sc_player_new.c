sc_player *sc_player_new(int index, const sc_tank_profile *tank) {
/* sc_player_new
   Create a new player */

   sc_player *p;

   /* Allocate player */
   p = (sc_player *)malloc(sizeof(sc_player));
   if(p == NULL) return(NULL);

   /* Initialize player name and ID number */
   sbprintf(p->name, sizeof(p->name), "Player %d", index + 1);
   p->name[SC_PLAYER_NAME_LENGTH - 1] = '\0';
   p->index  = index;
   p->aitype = SC_AI_HUMAN;
   p->tank   = tank;

   /* No weapons or accessories, by default */
   p->armed = false;
   p->weapons = NULL;
   p->shield = NULL;
   p->turret = 0;
   p->power  = 0;

   /* Setup AI state */
   p->ai = sc_ai_new();

   return(p);

}
