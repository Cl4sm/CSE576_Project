static void unit_gain_prestige( Unit *unit, const Unit *target, int target_dam )
{
	int gain = 0;
	
	/* I played around with PG a little but did not figure out how the
	 * formula works. Some points seem to hold true though:
	 * - own damage/unit loss does not give prestige penalty
	 *   (therefore we don't consider unit damage here)
	 * - if target is damaged but not destroyed then some random prestige 
	 *   is gained based on cost, experience and damage
	 * - if target is destroyed then more prestige is gained based on
	 *   cost and experience with just a little random modification
	 *
	 * FIXME: the formula used when unit is destroyed seems to be quite
	 * good, but the one for mere damage not so much especially when
	 * experience increases: rolling the dice more times does not increase
	 * chance very much for low cost units so we give less prestige for 
	 * cheap, experienced units compared to PG... */ 

	if (target->str == 0) {
		/* if unit destroyed: ((1/24 of cost) * exp) +- 10% */
		gain = ((target->exp_level + 1) * target->prop.cost * 10 + 
							120/*round up*/) / 240;
		gain = ((111 - DICE(21)) * gain) / 100;
		if (gain == 0)
			gain = 1;
	} else {
		/* if damaged: try half damage multiplied by experience throws
		 * on 50-dice against one tenth of cost. at maximum four times 
		 * experience can be gained */
		int throws = ((target_dam + 1) / 2) * (target->exp_level+1);
		int i, limit = 4 * (target->exp_level+1) - 1;
		for (i = 0; i < throws; i++)
			if( DICE(50) < target->prop.cost/10 ) {
				gain++;
				if (gain == limit)
					break;
			}
	}
	
	unit->player->cur_prestige += gain;
	//printf("%s attacked %s (%d damage%s): prestige +%d\n",
	//			unit->name, target->name, target_dam,
	//			(target->str==0)?", killed":"", gain);
}