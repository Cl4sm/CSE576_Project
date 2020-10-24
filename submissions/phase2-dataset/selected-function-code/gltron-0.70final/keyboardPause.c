void keyboardPause(int state, int key, int x, int y) {
	if(state == SYSTEM_KEYSTATE_UP)
		return;

  switch(key) {
  case 27:
		SystemExitLoop(RETURN_PAUSE_ESCAPE);
    break;
  case SYSTEM_KEY_F1: changeDisplay(0); break;
  case SYSTEM_KEY_F2: changeDisplay(1); break;
  case SYSTEM_KEY_F3: changeDisplay(2); break;
  case SYSTEM_KEY_F4: changeDisplay(3); break;

  case SYSTEM_KEY_F5: saveSettings(); break;

  case SYSTEM_KEY_F10: nextCameraType(); break;

  case SYSTEM_KEY_F11: doBmpScreenShot(gScreen); break;
  case SYSTEM_KEY_F12: doPngScreenShot(gScreen); break;
    
  case SYSTEM_KEY_UP: consoleScrollBackward(1); break;
  case SYSTEM_KEY_DOWN: consoleScrollForward(1); break;

  case SYSTEM_KEY_TAB: 
	// SystemExitLoop(RETURN_MENU_PROMPT);
		break;

	default:
    if(game->pauseflag == PAUSE_GAME_FINISHED)
      initData();
    /* lasttime = SystemGetElapsedTime(); */
    SystemExitLoop(RETURN_GAME_UNPAUSE);
    break;
  }
}
