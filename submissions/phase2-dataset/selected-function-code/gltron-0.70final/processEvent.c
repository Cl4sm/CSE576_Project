  int value = 0;

#ifdef RECORD
  if(game2->mode == GAME_SINGLE_RECORD) {
    writeEvent(e);
  }
#endif
  switch(e->type) {
  case EVENT_TURN_LEFT:
    doTurn(e, TURN_LEFT);
    break;
  case EVENT_TURN_RIGHT:
    doTurn(e, TURN_RIGHT);
    break;
  case EVENT_CRASH: 
    displayMessage(TO_CONSOLE, "player %d crashed", e->player + 1);
    doCrashPlayer(e);
    break;
  case EVENT_STOP:
    // displayMessage(TO_STDOUT, "game stopped");
#ifdef RECORD
    if(game2->mode == GAME_SINGLE_RECORD) {
      stopRecording();
      game2->mode = GAME_SINGLE;
    } else if(game2->mode == GAME_PLAY) {
      stopPlaying();
      game2->mode = GAME_SINGLE;
    }
#endif
    if(e->player<PLAYERS && game->player[e->player].ai->active != AI_NONE) {
      game->winner = e->player;
      displayMessage(TO_CONSOLE, "winner: %d", game->winner + 1);
    } else {
      game->winner = -2;
      displayMessage(TO_CONSOLE, "everyone died! no one wins!");
    }
		SystemExitLoop(RETURN_GAME_END);
    game->pauseflag = PAUSE_GAME_FINISHED;
    value = 1;
    break;
  }
  free(e);
  return value;
}
