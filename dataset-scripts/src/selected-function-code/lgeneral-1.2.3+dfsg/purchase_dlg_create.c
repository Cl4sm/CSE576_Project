PurchaseDlg *purchase_dlg_create( const char *theme_path )
{
	PurchaseDlg *pdlg = NULL;
	char path[512];
	int sx, sy;
	
	pdlg = calloc( 1, sizeof(PurchaseDlg) );
	if (pdlg == NULL)
		return NULL;
	
	/* create main group (= main window) */
	snprintf( path, 512, "%s/confirm_buttons.bmp", theme_path );
	pdlg->main_group = group_create( gui_create_frame( 300, 320 ), 160, 
				load_surf( path, SDL_SWSURFACE ),
				20, 20, 2, ID_PURCHASE_OK,
				gui->label, sdl.screen, 0, 0 );
	if (pdlg->main_group == NULL)
		goto failure;
	sx = group_get_width( pdlg->main_group ) - 60; 
	sy = group_get_height( pdlg->main_group ) - 25;
	group_add_button( pdlg->main_group, ID_PURCHASE_OK, sx, sy, 0, 
							tr("Ok") );
	group_add_button( pdlg->main_group, ID_PURCHASE_EXIT, sx + 30, sy, 0, 
							tr("Exit") );
	
	/* create unit limit info frame */
	pdlg->ulimit_frame = frame_create( gui_create_frame( 112, 40 ), 160,
							sdl.screen, 0, 0);
	if (pdlg->ulimit_frame == NULL)
		goto failure;
	
	/* create nation listbox */
	snprintf( path, 512, "%s/scroll_buttons.bmp", theme_path );
	pdlg->nation_lbox = lbox_create( gui_create_frame( 112, 74 ), 160, 6,
			load_surf( path, SDL_SWSURFACE ), 24, 24, gui->label,
			3, 1, 100, 12, 1, 0x0000ff,
			render_lbox_nation, sdl.screen, 0, 0);
	if (pdlg->nation_lbox == NULL)
		goto failure;
	
	/* create class listbox */
	snprintf( path, 512, "%s/scroll_buttons.bmp", theme_path );
	pdlg->uclass_lbox = lbox_create( gui_create_frame( 112, 166 ), 160, 6,
			load_surf( path, SDL_SWSURFACE ), 24, 24, gui->label,
			10, 2, 100, 12, 1, 0x0000ff,
			render_lbox_uclass, sdl.screen, 0, 0);
	if (pdlg->uclass_lbox == NULL)
		goto failure;
	
	/* create units listbox */
	snprintf( path, 512, "%s/scroll_buttons.bmp", theme_path );
	pdlg->unit_lbox = lbox_create( gui_create_frame( 112, 200 ), 160, 6,
			load_surf( path, SDL_SWSURFACE ), 24, 24, gui->label,
			4, 3, 100, 40, 1, 0x0000ff,
			render_lbox_unit, sdl.screen, 0, 0);
	if (pdlg->unit_lbox == NULL)
		goto failure;
	
	/* create transporters listbox */
	snprintf( path, 512, "%s/scroll_buttons.bmp", theme_path );
	pdlg->trsp_lbox = lbox_create( gui_create_frame( 112, 120 ), 160, 6,
			load_surf( path, SDL_SWSURFACE ), 24, 24, gui->label,
			2, 1, 100, 40, 1, 0x0000ff,
			render_lbox_unit, sdl.screen, 0, 0);
	if (pdlg->trsp_lbox == NULL)
		goto failure;
	
	/* create reinforcements listbox */
	snprintf( path, 512, "%s/scroll_buttons.bmp", theme_path );
	pdlg->reinf_lbox = lbox_create( gui_create_frame( 112, 280 ), 160, 6,
			load_surf( path, SDL_SWSURFACE ), 24, 24, gui->label,
			6, 3, 100, 40, 1, 0x0000ff,
			render_lbox_reinf, sdl.screen, 0, 0);
	if (pdlg->reinf_lbox == NULL)
		goto failure;
	
	return pdlg;
failure:
	fprintf(stderr, tr("Failed to create purchase dialogue\n"));
	purchase_dlg_delete(&pdlg);
	return NULL;
}