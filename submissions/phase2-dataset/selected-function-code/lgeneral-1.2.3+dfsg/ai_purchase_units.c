static void ai_purchase_units()
{
	List *nlist = NULL; /* list of player's nations */
	List *ulist = NULL; /* list of purchasable unit lib entries */
	Unit_Lib_Entry *e = NULL;
	Nation *n = NULL;
	int ulimit = player_get_purchase_unit_limit(cur_player);
	struct {
		Unit_Lib_Entry *unit; /* unit to be bought */
		Unit_Lib_Entry *trsp; /* its transporter if not NULL */
		int weight; /* relative weight to other units in list */
	} buy_options[5];
	int num_buy_options;
	int maxeyes, i, j;
	
	AI_DEBUG( 1, "Prestige: %d, Units available: %d\n", 
					cur_player->cur_prestige, ulimit);
	
	/* if no capacity, return */
	if (ulimit == 0)
		return;
	
	ulist = get_purchasable_unit_lib_entries( NULL, NULL, 
						&scen_info->start_date);
	nlist = get_purchase_nations();
	
	/* remove all entries from ulist that do not have one of our nations */
	list_reset(ulist);
	while ((e = list_next(ulist))) {
		if (e->nation == -1) {
			list_delete_current(ulist); /* not purchasable */
			continue;
		}
		list_reset(nlist);
		while ((n = list_next(nlist)))
			if (&nations[e->nation] == n)
				break;
		if (n == NULL) {
			list_delete_current(ulist); /* not our nation */
			continue;
		}
		AI_DEBUG(2, "%s can be purchased (score: %d, cost: %d)\n",
						e->name,e->eval_score,e->cost);
	}
	
	memset(buy_options,0,sizeof(buy_options));
	num_buy_options = 0;
	
	/* FIXME: this certainly has to be more detailled and complex but for
	 * now, keep it simple:
	 * 
	 * for defense: buy cheapiest infantry (30%), antitank (30%), 
	 * airdefense (20%) or tank (20%) 
	 * for attack: buy good tank (40%), infantry with transporter (15%),
	 * good artillery (15%), fighter (10%) or bomber (20%) */
	if (cur_player->strat < 0) {
		buy_options[0].unit = get_cannonfodder( ulist, INFANTRY );
		buy_options[0].weight = 30;
		buy_options[1].unit = get_cannonfodder( ulist, ANTI_TANK );
		buy_options[1].weight = 30;
		buy_options[2].unit = get_cannonfodder( ulist, AIR_DEFENSE );
		buy_options[2].weight = 20;
		buy_options[3].unit = get_cannonfodder( ulist, TANK );
		buy_options[3].weight = 20;
		num_buy_options = 4;
	} else {
		buy_options[0].unit = get_good_unit( ulist, INFANTRY );
		buy_options[0].weight = 15;
		buy_options[1].unit = get_good_unit( ulist, TANK );
		buy_options[1].weight = 40;
		buy_options[2].unit = get_good_unit( ulist, ARTILLERY );
		buy_options[2].weight = 15;
		buy_options[3].unit = get_good_unit( ulist, INTERCEPTOR );
		buy_options[3].weight = 10;
		buy_options[4].unit = get_good_unit( ulist, BOMBER );
		buy_options[4].weight = 20;
		num_buy_options = 5;
	}
	
	/* get "size of dice" :-) */
	maxeyes = 0;
	for (i = 0; i < num_buy_options; i++) {
		maxeyes += buy_options[i].weight;
		AI_DEBUG(1, "Purchase option %d (w=%d): %s%s\n", i+1,
					buy_options[i].weight, 
					buy_options[i].unit?
					buy_options[i].unit->name:"empty",
					buy_options[i].trsp?
					buy_options[i].trsp->name:"");
		if (buy_options[i].unit == NULL) {
			/* this is only the case for the very first scenarios
			 * to catch it, simply fall back to infantry */
			AI_DEBUG(1, "Option %d empty (use option 1)\n", i+1);
			buy_options[i].unit = buy_options[0].unit;
		}
	}
	
	/* try to buy units; if not possible (cost exceeds prestige) do nothing
	 * (= save prestige and try to buy next turn) */
	for (i = 0; i < ulimit; i++) {
		int roll = DICE(maxeyes);
		for (j = 0; j < num_buy_options; j++)
			if (roll > buy_options[j].weight)
				roll -= buy_options[j].weight;
			else 
				break;
		AI_DEBUG(1, "Choosing option %d\n",j+1);
		if (buy_options[j].unit && player_can_purchase_unit( cur_player,
						buy_options[j].unit,
						buy_options[j].trsp)) {
			player_purchase_unit( cur_player, 
				&nations[buy_options[j].unit->nation],
				buy_options[j].unit,buy_options[j].trsp);
			AI_DEBUG(1, "Prestige remaining: %d\n",
						cur_player->cur_prestige);
		} else
			AI_DEBUG(1, "Could not purchase unit?!?\n");
	}
		
	list_delete( nlist );
	list_delete( ulist );
}