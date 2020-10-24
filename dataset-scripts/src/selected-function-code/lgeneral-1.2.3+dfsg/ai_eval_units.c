static void ai_eval_units()
{
    Unit *unit;
    list_reset( units );
    while ( ( unit = list_next( units ) ) ) {
        if ( unit->killed ) continue;
        unit->eval_score = 0;
        if ( unit->prop.ammo > 0 ) {
            if ( unit->cur_ammo >= 5 )
                /* it's extremly unlikely that there'll be more than
                   five attacks on a unit within one turn so we
                   can consider a unit with 5+ ammo 100% ready for 
                   battle */
                unit->eval_score += 1000;
            else
                unit->eval_score += get_rel( unit->cur_ammo, 
                                             MINIMUM( 5, unit->prop.ammo ) );
        }
        if ( unit->prop.fuel > 0 ) {
            if ( (  (unit->sel_prop->flags & FLYING) && unit->cur_fuel >= 20 ) ||
                 ( !(unit->sel_prop->flags & FLYING) && unit->cur_fuel >= 10 ) )
                /* a unit with this range is considered 100% operable */
                unit->eval_score += 1000;
            else {
                if ( unit->sel_prop->flags & FLYING )
                    unit->eval_score += get_rel( unit->cur_fuel, MINIMUM( 20, unit->prop.fuel ) );
                else
                    unit->eval_score += get_rel( unit->cur_fuel, MINIMUM( 10, unit->prop.fuel ) );
            }
        }
        unit->eval_score += unit->exp_level * 250;
        unit->eval_score += unit->str * 200; /* strength is counted doubled */
        /* the unit experience is not counted as normal but gives a bonus
           that may increase the evaluation */
        unit->eval_score /= 2 + (unit->prop.ammo > 0) + (unit->prop.fuel > 0);
        /* this value between 0 and 1000 indicates the readiness of the unit
           and therefore the permillage of eval_score */
        unit->eval_score = unit->eval_score * unit->prop.eval_score / 1000;
    }
}