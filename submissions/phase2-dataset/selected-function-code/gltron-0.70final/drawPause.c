  char pause[] = "Game is paused";
  char winner[] = "Player %d wins!";
  char nowinner[] = "No one wins!";
  char buf[100];
  char *message;
  static float d = 0;
  static float lt = 0;
  float delta;
  int now;

  now = SystemGetElapsedTime();
  delta = now - lt;
  lt = now;
  delta /= 500.0;
  d += delta;
  /* printf("%.5f\n", delta); */
  
  if (d > 2 * PI) { 
    d -= 2 * PI;
  }

  if ((game->pauseflag & PAUSE_GAME_FINISHED) && game->winner != -1) {
    if (game->winner >= -1) {

      float* player_color = gPlayerVisuals[game->winner].pColorAlpha;

      /* 
         make the 'Player wins' message oscillate between 
         white and the winning bike's color 
       */
      glColor3f((player_color[0] + ((sinf(d) + 1) / 2) * (1 - player_color[0])),
                (player_color[1] + ((sinf(d) + 1) / 2) * (1 - player_color[1])),
                (player_color[2] + ((sinf(d) + 1) / 2) * (1 - player_color[2]))); 

      message = buf;
      sprintf(message, winner, game->winner + 1);
    } else {
      glColor3d(1.0, (sin(d) + 1) / 2, (sin(d) + 1) / 2);
      message = nowinner;
    }
  } else {
    glColor3d(1.0, (sin(d) + 1) / 2, (sin(d) + 1) / 2);
    message = pause;
  }

  rasonly(gScreen);
  drawText(gameFtx, display->vp_w / 6, 20, 
	   display->vp_w / (6.0f / 4.0f * strlen(message)), message);
}
