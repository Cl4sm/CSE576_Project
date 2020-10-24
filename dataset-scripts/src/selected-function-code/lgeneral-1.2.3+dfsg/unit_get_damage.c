void unit_get_damage( Unit *aggressor, Unit *unit, Unit *target, 
                      int type, 
                      int real, int rugged_def,
                      int *damage, int *suppr )
{
    int atk_strength, max_roll, min_roll, die_mod;
    int atk_grade, def_grade, diff, result;
    float suppr_chance, kill_chance;
    /* use PG's formula to compute the attack/defense grade*/
    /* basic attack */
    atk_grade = abs( unit->sel_prop->atks[target->sel_prop->trgt_type] );
#ifdef DEBUG_ATTACK
    if ( real ) printf( "\n%s attacks:\n", unit->name );
    if ( real ) printf( "  base:   %2i\n", atk_grade );
    if ( real ) printf( "  exp:    +%i\n", unit->exp_level);
#endif
    /* experience */
    atk_grade += unit->exp_level;
    /* target on a river? */
    if ( !(target->sel_prop->flags & FLYING ) )
    if ( target->terrain->flags[cur_weather] & RIVER ) {
        atk_grade += 4;
#ifdef DEBUG_ATTACK
        if ( real ) printf( "  river:  +4\n" );
#endif
    }
    /* counterattack of rugged defense unit? */
    if ( type == UNIT_PASSIVE_ATTACK && rugged_def ) {
        atk_grade += 4;
#ifdef DEBUG_ATTACK
            if ( real ) printf( "  rugdef: +4\n" );
#endif
    }
#ifdef DEBUG_ATTACK
    if ( real ) printf( "---\n%s defends:\n", target->name );
#endif
    /* basic defense */
    if ( unit->sel_prop->flags & FLYING )
        def_grade = target->sel_prop->def_air;
    else {
        def_grade = target->sel_prop->def_grnd;
        /* apply close defense? */
        if ( unit->sel_prop->flags & INFANTRY )
            if ( !( target->sel_prop->flags & INFANTRY ) )
                if ( !( target->sel_prop->flags & FLYING ) )
                    if ( !( target->sel_prop->flags & SWIMMING ) )
                    {
                        if ( target == aggressor )
                        if ( unit->terrain->flags[cur_weather]&INF_CLOSE_DEF )
                            def_grade = target->sel_prop->def_cls;
                        if ( unit == aggressor )
                        if ( target->terrain->flags[cur_weather]&INF_CLOSE_DEF )
                            def_grade = target->sel_prop->def_cls;
                    }
    }
#ifdef DEBUG_ATTACK
    if ( real ) printf( "  base:   %2i\n", def_grade );
    if ( real ) printf( "  exp:    +%i\n", target->exp_level );
#endif
    /* experience */
    def_grade += target->exp_level;
    /* attacker on a river or swamp? */
    if ( !(unit->sel_prop->flags & FLYING) )
    if ( !(unit->sel_prop->flags & SWIMMING) )
    if ( !(target->sel_prop->flags & FLYING) )
    {
        if ( unit->terrain->flags[cur_weather] & SWAMP ) 
        {
            def_grade += 4;
#ifdef DEBUG_ATTACK
            if ( real ) printf( "  swamp:  +4\n" );
#endif
        } else
        if ( unit->terrain->flags[cur_weather] & RIVER ) {
            def_grade += 4;
#ifdef DEBUG_ATTACK
            if ( real ) printf( "  river:  +4\n" );
#endif
        }
    }
    /* rugged defense? */
    if ( type == UNIT_ACTIVE_ATTACK && rugged_def ) {
        def_grade += 4;
#ifdef DEBUG_ATTACK
        if ( real ) printf( "  rugdef: +4\n" );
#endif
    }
    /* entrenchment */
    if ( unit->sel_prop->flags & IGNORE_ENTR )
        def_grade += 0;
    else {
        if ( unit->sel_prop->flags & INFANTRY )
            def_grade += target->entr / 2;
        else
            def_grade += target->entr;
#ifdef DEBUG_ATTACK
        if ( real ) printf( "  entr:   +%i\n", 
                (unit->sel_prop->flags & INFANTRY) ? target->entr / 2 : target->entr );
#endif
    }
    /* naval vs ground unit */
    if ( !(unit->sel_prop->flags & SWIMMING ) )
        if ( !(unit->sel_prop->flags & FLYING) )
            if ( target->sel_prop->flags & SWIMMING ) {
                def_grade += 8;
#ifdef DEBUG_ATTACK
                if ( real ) printf( "  naval: +8\n" );
#endif
            }
    /* bad weather? */
    if ( unit->sel_prop->rng > 0 )
        if ( weather_types[cur_weather].flags & BAD_SIGHT ) {
            def_grade += 3;
#ifdef DEBUG_ATTACK
            if ( real ) printf( "  sight: +3\n" );
#endif
        }
    /* initiating attack against artillery? */
    if ( type == UNIT_PASSIVE_ATTACK )
        if ( unit->sel_prop->flags & ARTILLERY ) {
            def_grade += 3;
#ifdef DEBUG_ATTACK
            if ( real ) printf( "  def vs art: +3\n" );
#endif
        }
    /* infantry versus anti_tank? */
    if ( target->sel_prop->flags & INFANTRY )
        if ( unit->sel_prop->flags & ANTI_TANK ) {
            def_grade += 2;
#ifdef DEBUG_ATTACK
            if ( real ) printf( "  antitnk:+2\n" );
#endif
        }
    /* no fuel makes attacker less effective */
    if ( unit_check_fuel_usage( unit ) && unit->cur_fuel == 0 )
    {
        def_grade += 4;
#ifdef DEBUG_ATTACK
            if ( real ) printf( "  lowfuel:+4\n" );
#endif
    }
    /* attacker strength */
    atk_strength = unit_get_cur_str( unit );
#ifdef DEBUG_ATTACK
    if ( real && atk_strength != unit_get_cur_str( unit ) )
        printf( "---\n%s with half strength\n", unit->name );
#endif
    /*  PG's formula:
        get difference between attack and defense
        strike for each strength point with 
          if ( diff <= 4 ) 
              D20 + diff
          else
              D20 + 4 + 0.4 * ( diff - 4 )
        suppr_fire flag set: 1-10 miss, 11-18 suppr, 19+ kill
        normal: 1-10 miss, 11-12 suppr, 13+ kill */
    diff = atk_grade - def_grade; if ( diff < -7 ) diff = -7;
    *damage = 0; *suppr = 0;
#ifdef DEBUG_ATTACK
    if ( real ) {
        printf( "---\n%i x %i --> %i x %i\n", 
                atk_strength, atk_grade, unit_get_cur_str( target ), def_grade );
    }
#endif
    /* get the chances for suppression and kills (computed here
       to use also for debug info */
    suppr_chance = kill_chance = 0;
    die_mod = ( diff <= 4 ? diff : 4 + 2 * ( diff - 4 ) / 5 );
    min_roll = 1 + die_mod; max_roll = 20 + die_mod;
    /* get chances for suppression and kills */
    if ( unit->sel_prop->flags & SUPPR_FIRE ) {
        int limit = (type==UNIT_DEFENSIVE_ATTACK)?20:18;
        if (limit-min_roll>=0)
            suppr_chance = 0.05*(MINIMUM(limit,max_roll)-MAXIMUM(11,min_roll)+1);
        if (max_roll>limit)
            kill_chance = 0.05*(max_roll-MAXIMUM(limit+1,min_roll)+1);
    }
    else {
        if (12-min_roll>=0)
            suppr_chance = 0.05*(MINIMUM(12,max_roll)-MAXIMUM(11,min_roll)+1);
        if (max_roll>12)
            kill_chance = 0.05*(max_roll-MAXIMUM(13,min_roll)+1);
    }
    if (suppr_chance<0) suppr_chance=0; if (kill_chance<0) kill_chance=0;
    if ( real ) {
#ifdef DEBUG_ATTACK
        printf( "Roll: D20 + %i (Kill: %i%%, Suppr: %i%%)\n", 
                diff <= 4 ? diff : 4 + 2 * ( diff - 4 ) / 5,
                (int)(100 * kill_chance), (int)(100 * suppr_chance) );
#endif
        while ( atk_strength-- > 0 ) {
            if ( diff <= 4 )
                result = DICE(20) + diff;
            else
                result = DICE(20) + 4 + 2 * ( diff - 4 ) / 5;
            if ( unit->sel_prop->flags & SUPPR_FIRE ) {
                int limit = (type==UNIT_DEFENSIVE_ATTACK)?20:18;
                if ( result >= 11 && result <= limit )
                    (*suppr)++;
                else
                    if ( result >= limit+1 )
                        (*damage)++;
            }
            else {
                if ( result >= 11 && result <= 12 )
                    (*suppr)++;
                else
                    if ( result >= 13 )
                        (*damage)++;
            }
        }
#ifdef DEBUG_ATTACK
        printf( "Kills: %i, Suppression: %i\n\n", *damage, *suppr );
#endif
    }
    else {
        *suppr = (int)(suppr_chance * atk_strength);
        *damage = (int)(kill_chance * atk_strength);
    }
}