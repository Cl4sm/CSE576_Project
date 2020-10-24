SelectDlg *select_dlg_create(
	SDL_Surface *lbox_frame, 
	SDL_Surface *lbox_buttons, int lbox_button_w, int lbox_button_h, 
	int lbox_cell_count, int lbox_cell_w, int lbox_cell_h,
	void (*lbox_render_cb)(void*, SDL_Surface*),
	SDL_Surface *conf_frame,
	SDL_Surface *conf_buttons, int conf_button_w, int conf_button_h,
	int id_ok, 
	SDL_Surface *surf, int x, int y )
{
	SelectDlg *sdlg = NULL;
	int sx, sy;
	
	sdlg = calloc(1, sizeof(SelectDlg));
	if (sdlg == NULL)
		goto failure;
	
	sdlg->button_group = group_create(conf_frame, 160, conf_buttons,
				conf_button_w, conf_button_h, 2, id_ok, 
				gui->label, sdl.screen, 0, 0);
	if (sdlg->button_group == NULL)
		goto failure;
	sx = group_get_width( sdlg->button_group ) - 60; 
	sy = 5;
	group_add_button( sdlg->button_group, id_ok, sx, sy, 0, tr("Apply") );
	group_add_button( sdlg->button_group, id_ok+1, sx + 30, sy, 0, 
							tr("Cancel") );
	
	sdlg->select_lbox = lbox_create( lbox_frame, 160, 6, 
			lbox_buttons, lbox_button_w, lbox_button_h, gui->label, 
			lbox_cell_count, lbox_cell_count/2, 
			lbox_cell_w, lbox_cell_h, 
			1, 0x0000ff, lbox_render_cb, sdl.screen, 0, 0);
	if (sdlg->select_lbox == NULL)
		goto failure;
	
	return sdlg;
failure:
	fprintf(stderr, tr("Failed to create select dialogue\n"));
	select_dlg_delete(&sdlg);
	return NULL;
}