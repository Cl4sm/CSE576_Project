void initGameStructures(void) { /* called only once */
  int i;

  /* initialize some global variables */
  game2 = &main_game2;
  game = &main_game;
  game->pauseflag = PAUSE_NO_GAME;

  game->winner = -1;

  game->players = PLAYERS;
  game->player = (Player *) malloc(MAX_PLAYERS * sizeof(Player));

  for(i = 0; i < game->players; i++) {
		Player *p = game->player + i;

    p->ai = (AI*) malloc(sizeof(AI));
    p->data = (Data*) malloc(sizeof(Data));
    p->data->trails = (segment2*) malloc(MAX_TRAIL * sizeof(segment2));
		p->data->trailOffset = 0;
		p->camera = (Camera*) malloc(sizeof(Camera));
   }

  game2->events.next = NULL;
  game2->mode = GAME_SINGLE;
}
