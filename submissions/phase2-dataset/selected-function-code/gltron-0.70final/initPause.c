  SystemHidePointer();
  SystemWarpPointer(MOUSE_ORIG_X, MOUSE_ORIG_Y);

  /* disable game sound effects */
  Audio_DisableEngine();

  /* 
   * TODO: Provide an option to disable game music here. 
   * Game should be totally silent in pause mode. (Nice when 
   * the boss is walking by, phone call, etc...)
   */

  /* set pause flag to suspended */
  if (game->pauseflag != PAUSE_GAME_FINISHED) {
    game->pauseflag = PAUSE_GAME_SUSPENDED;
  }
  
  updateSettingsCache();
}
