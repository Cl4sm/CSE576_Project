void unit_get_df_units( Unit *unit, Unit *target, List *units, List *df_units )
{
    Unit *entry;
    list_clear( df_units );
    if ( unit->sel_prop->flags & FLYING ) {
        list_reset( units );
        while ( ( entry = list_next( units ) ) ) {
            if ( entry->killed ) continue;
            if ( entry == target ) continue;
            if ( entry == unit ) continue;
            /* bombers -- intercepting impossibly covered by unit_check_attack() */
            if ( !(target->sel_prop->flags & INTERCEPTOR) )
                if ( unit_is_close( target, entry ) )
                    if ( entry->sel_prop->flags & INTERCEPTOR )
                        if ( player_is_ally( entry->player, target->player ) )
                            if ( entry->cur_ammo > 0 ) {
                                list_add( df_units, entry );
                                continue;
                            }
            /* air-defense */
            if ( entry->sel_prop->flags & AIR_DEFENSE )
                /* FlaK will not give support when an air-to-air attack is
                 * taking place. First, in reality it would prove distastrous,
                 * second, Panzer General doesn't allow it, either.
                 */
                if ( !(target->sel_prop->flags & FLYING) )
                    if ( unit_is_close( target, entry ) ) /* adjacent help only */
                        if ( unit_check_attack( entry, unit, UNIT_DEFENSIVE_ATTACK ) )
                            list_add( df_units, entry );
        }
    }
    else if ( unit->sel_prop->rng==0 ) { 
        /* artillery for melee combat; if unit attacks ranged, there is no 
           support */
        list_reset( units );
        while ( ( entry = list_next( units ) ) ) {
            if ( entry->killed ) continue;
            if ( entry == target ) continue;
            if ( entry == unit ) continue;
            /* HACK: An artillery with range 1 cannot support adjacent units but
               should do so. So we allow to give defensive fire on a range of 2
               like a normal artillery */
            if ( entry->sel_prop->flags & ARTILLERY && entry->sel_prop->rng == 1 )
                if ( unit_is_close( target, entry ) )
                    if ( player_is_ally( entry->player, target->player ) )
                        if ( entry->cur_ammo > 0 ) {
                            list_add( df_units, entry );
                            continue;
                        }
            /* normal artillery */
            if ( entry->sel_prop->flags & ARTILLERY )
                if ( unit_is_close( target, entry ) ) /* adjacent help only */
                    if ( unit_check_attack( entry, unit, UNIT_DEFENSIVE_ATTACK ) )
                        list_add( df_units, entry );
        }
    }
    /* randomly remove all support but one */
    if (df_units->count>0)
    {
        entry = list_get(df_units,rand()%df_units->count);
        list_clear(df_units);
        list_add( df_units, entry );
    }
}