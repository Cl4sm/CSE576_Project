void doCameraMovement(void) {
  int i;
  Player *p;
	PlayerVisual *pV;

  for(i = 0; i < game->players; i++) {
    p = game->player + i;
		pV = gPlayerVisuals + i;
      
 
    if(p->data->speed == SPEED_GONE)
      observerCamera(pV, p);
    else
      playerCamera(pV, p);
  }

  /* mouse events consumed */
  gInput.mousex = 0;
  gInput.mousey = 0;
}
