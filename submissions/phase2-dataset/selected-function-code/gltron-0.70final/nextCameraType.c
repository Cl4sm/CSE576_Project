  int i;
  int current_cam_type = getSettingi("camType");
  int new_cam_type = (current_cam_type + 1) % CAM_COUNT;
  
  setSettingi("camType", new_cam_type);

  /* update the cached setting */
  gSettingsCache.camType = new_cam_type;
  
  for (i = 0; i < game->players; i++) {
    if (game->player[i].ai->active == AI_HUMAN) {
      initCamera(game->player[i].camera, game->player[i].data, new_cam_type);
    }
  }

  if (getSettingi("debug_output")) {
    switch (new_cam_type) {
      case 0 :
        displayMessage(TO_CONSOLE, "[camera] Circling Camera");
        break;
      case 1 :
        displayMessage(TO_CONSOLE, "[camera] Behind Camera");
        break;
      case 2 :
        displayMessage(TO_CONSOLE, "[camera] Cockpit Camera");
        break;
      case 3 :
        displayMessage(TO_CONSOLE, "[camera] Mouse Camera");
        break;
    }
  }
}
