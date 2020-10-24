void player_purchase_unit( Player *player, Nation *nation,
			Unit_Lib_Entry *unit_prop, Unit_Lib_Entry *trsp_prop )
{
	Unit *unit = NULL, unit_base;
	
	/* build unit base info */
	memset( &unit_base, 0, sizeof( Unit ) );
	unit_base.nation = nation;
	unit_base.player = player;
	/* FIXME: no global counter to number unit so use plain name */
	snprintf(unit_base.name, sizeof(unit_base.name), "%s", unit_prop->name);
	unit_base.delay = turn + 1; /* available next turn */
	unit_base.str = 10;
	unit_base.orient = cur_player->orient;
	
	/* create unit and push to reinf list */
	if ((unit = unit_create( unit_prop, trsp_prop, &unit_base )) == NULL) {
		fprintf( stderr, tr("Out of memory\n") );
		return;
	}
	list_add( reinf, unit );
	
	/* pay for it */
	player->cur_prestige -= unit_prop->cost;
	if (trsp_prop)
		player->cur_prestige -= trsp_prop->cost;
}