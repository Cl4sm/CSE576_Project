  gScreen = (Visual*) malloc(sizeof(Visual));
  gViewportType = getSettingi("display_type"); 
  
	{
		Visual *d = gScreen;
		d->w = getSettingi("width"); 
		d->h = getSettingi("height"); 
		d->vp_x = 0; d->vp_y = 0;
		d->vp_w = d->w; d->vp_h = d->h;
		d->onScreen = -1;
		d->textures = (unsigned int*) malloc(game_textures * sizeof(unsigned int));
	}

	gPlayerVisuals = (PlayerVisual*) malloc(MAX_PLAYERS * sizeof(PlayerVisual));

	loadModels();

  changeDisplay(-1);
}
