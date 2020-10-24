int player_get_purchase_unit_limit( Player *player )
{
	int limit = player->unit_limit;
	Unit *u = NULL;
	
	if (limit == 0)
		return -1;
	
	/* units placed on map */
	list_reset( units );
	while ((u = list_next( units )))
		if (u->player == player && u->killed == 0)
			limit--;
	/* units ordered, not yet arrived */	
	list_reset( reinf );
	while ((u = list_next( reinf )))
		if (u->player == player)
			limit--;
	/* units arrived, not yet placed */
	limit -= avail_units->count;
	
	return limit;
}