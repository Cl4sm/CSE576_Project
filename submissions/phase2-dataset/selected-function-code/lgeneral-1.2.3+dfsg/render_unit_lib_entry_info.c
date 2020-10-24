static void render_unit_lib_entry_info( Unit_Lib_Entry *entry,
					SDL_Surface *buffer, int x, int y )
{
	char str[128];
	int i, start_y = y;
	
	gui->font_std->align = ALIGN_X_LEFT | ALIGN_Y_TOP;
	
	/* first column: name, type, ammo, fuel, spot, mov, ini, range */
	write_line( buffer, gui->font_std, entry->name, x, &y );
	write_line( buffer, gui->font_std, 
				unit_classes[entry->class].name, x, &y );
	y += 5;
	sprintf( str, tr("Cost:       %3i"), entry->cost );
	write_line( buffer, gui->font_std, str, x, &y );
	if ( entry->ammo == 0 )
		sprintf( str, tr("Ammo:         --") );
	else
		sprintf( str, tr("Ammo:        %2i"), entry->ammo );
	write_line( buffer, gui->font_std, str, x, &y );
	if ( entry->fuel == 0 )
		sprintf( str, tr("Fuel:        --") );
	else
		sprintf( str, tr("Fuel:        %2i"), entry->fuel );
	write_line( buffer, gui->font_std, str, x, &y );
	y += 5;
	sprintf( str, tr("Spotting:    %2i"), entry->spt );
	write_line( buffer, gui->font_std, str, x, &y );
	sprintf( str, tr("Movement:    %2i"), entry->mov );
	write_line( buffer, gui->font_std, str, x, &y );
	sprintf( str, tr("Initiative:  %2i"), entry->ini );
	write_line( buffer, gui->font_std, str, x, &y );
	sprintf( str, tr("Range:       %2i"), entry->rng );
	write_line( buffer, gui->font_std, str, x, &y );

	/* second column: move type, target type, attack, defense */
	y = start_y;
	x += 135;
	sprintf( str, tr("%s Movement"), mov_types[entry->mov_type].name );
	write_line( buffer, gui->font_std, str, x, &y );
	sprintf( str, tr("%s Target"), trgt_types[entry->trgt_type].name );
	write_line( buffer, gui->font_std, str, x, &y );
	y += 5;
	for ( i = 0; i < trgt_type_count; i++ ) {
		char valstr[8];
		int j;
		
		sprintf( str, tr("%s Attack:"), trgt_types[i].name );
		for (j = strlen(str); j < 14; j++)
			strcat( str, " " );
		if ( entry->atks[i] < 0 )
			snprintf( valstr, 8, "[%2i]", -entry->atks[i] );
		else
			snprintf( valstr, 8, "  %2i", entry->atks[i] );
		strcat(str, valstr);
		write_line( buffer, gui->font_std, str, x, &y );
	}
	y += 5;
	sprintf( str, tr("Ground Defense: %2i"), entry->def_grnd );
	write_line( buffer, gui->font_std, str, x, &y );
	sprintf( str, tr("Air Defense:    %2i"), entry->def_air );
	write_line( buffer, gui->font_std, str, x, &y );
	sprintf( str, tr("Close Defense:  %2i"), entry->def_cls );
	write_line( buffer, gui->font_std, str, x, &y );
}