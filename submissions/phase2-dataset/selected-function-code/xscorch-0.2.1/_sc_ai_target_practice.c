static void _sc_ai_target_practice(const sc_config *c, sc_player **playerlist) {
/* sc_ai_target_practice
   I feel kind of sorry for the poor, poor humans...
   Actually, no, I don't :)  */

   sc_player *tmp;
   int i;
   int j;

   assert(c != NULL && playerlist != NULL);

   if(!c->aicontrol->humantargets) return;

   for(i = 0; i < c->numplayers; ++i) {
      if(playerlist[i]->aitype == SC_AI_HUMAN) {
         tmp = playerlist[i];
         for(j = 0; j < i && tmp != NULL; ++j) {
            if(playerlist[j]->aitype != SC_AI_HUMAN) {
               playerlist[i] = playerlist[j];
               playerlist[j] = tmp;
               tmp = NULL;
            }
         }
      }
   }

}
