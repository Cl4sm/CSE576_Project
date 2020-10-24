int unit_check_attack( Unit *unit, Unit *target, int type )
{
    if ( target == 0 || unit == target ) return 0;
    if ( player_is_ally( unit->player, target->player ) ) return 0;
    if ( unit->sel_prop->flags & FLYING && !( target->sel_prop->flags & FLYING ) )
        if ( unit->sel_prop->rng == 0 )
            if ( unit->x != target->x || unit->y != target->y )
                return 0; /* range 0 means above unit for an aircraft */
    /* if the target flys and the unit is ground with a range of 0 the aircraft
       may only be harmed when above unit */
    if ( !(unit->sel_prop->flags & FLYING) && ( target->sel_prop->flags & FLYING ) )
        if ( unit->sel_prop->rng == 0 )
            if ( unit->x != target->x || unit->y != target->y )
                return 0;
    /* only destroyers may harm submarines */
    if ( target->sel_prop->flags & DIVING && !( unit->sel_prop->flags & DESTROYER ) ) return 0;
    if ( weather_types[cur_weather].flags & NO_AIR_ATTACK ) {
        if ( unit->sel_prop->flags & FLYING ) return 0;
        if ( target->sel_prop->flags & FLYING ) return 0;
    }
    if ( type == UNIT_ACTIVE_ATTACK ) {
        /* agressor */
        if ( unit->cur_ammo <= 0 ) return 0;
        if ( unit->sel_prop->atks[target->sel_prop->trgt_type] <= 0 ) return 0;
        if ( unit->cur_atk_count == 0 ) return 0;
        if ( !unit_is_close( unit, target ) && get_dist( unit->x, unit->y, target->x, target->y ) > unit->sel_prop->rng ) return 0;
    }
    else
    if ( type == UNIT_DEFENSIVE_ATTACK ) {
        /* defensive fire */
        if ( unit->sel_prop->atks[target->sel_prop->trgt_type] <= 0 ) return 0;
        if ( unit->cur_ammo <= 0 ) return 0;
        if ( ( unit->sel_prop->flags & ( INTERCEPTOR | ARTILLERY | AIR_DEFENSE ) ) == 0 ) return 0;
        if ( target->sel_prop->flags & ( ARTILLERY | AIR_DEFENSE | SWIMMING ) ) return 0;
        if ( unit->sel_prop->flags & INTERCEPTOR ) {
            /* the interceptor is propably not beside the attacker so the range check is different
             * can't be done here because the unit the target attacks isn't passed so 
             *  unit_get_df_units() must have a look itself 
             */
        }
        else
            if ( get_dist( unit->x, unit->y, target->x, target->y ) > unit->sel_prop->rng ) return 0;
    }
    else {
        /* counter-attack */
        if ( unit->cur_ammo <= 0 ) return 0;
        if ( !unit_is_close( unit, target ) && get_dist( unit->x, unit->y, target->x, target->y ) > unit->sel_prop->rng ) return 0;
        if ( unit->sel_prop->atks[target->sel_prop->trgt_type] == 0 ) return 0;
        /* artillery may only defend against close units */
        if ( unit->sel_prop->flags & ARTILLERY )
            if ( !unit_is_close( unit, target ) )
                return 0;
        /* you may defend against artillery only when close */
        if ( target->sel_prop->flags & ARTILLERY )
            if ( !unit_is_close( unit, target ) )
                return 0;
    }
    return 1;
}