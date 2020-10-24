static int unit_attack( Unit *unit, Unit *target, int type, int real, int force_rugged )
{
    int unit_old_str = unit->str;//, target_old_str = target->str;
    int unit_old_ini = unit->sel_prop->ini, target_old_ini = target->sel_prop->ini;
    int unit_dam = 0, unit_suppr = 0, target_dam = 0, target_suppr = 0;
    int rugged_chance, rugged_def = 0;
    int exp_mod;
    int ret = AR_NONE; /* clear flags */
    int strike;
    /* check if rugged defense occurs */
    if ( real && type == UNIT_ACTIVE_ATTACK )
        if ( unit_check_rugged_def( unit, target ) || ( force_rugged && unit_is_close( unit, target ) ) ) {
            rugged_chance = unit_get_rugged_def_chance( unit, target );
            if ( DICE(100) <= rugged_chance || force_rugged )
                rugged_def = 1;
        }
    /* PG's formula for initiative is
       min { base initiative, terrain max initiative } +
       ( exp_level + 1 ) / 2 + D3 */
    /* against aircrafts the initiative is used since terrain does not matter */
    /* target's terrain is used for fight */
    if ( !(unit->sel_prop->flags & FLYING) && !(target->sel_prop->flags & FLYING) )
    {
        unit->sel_prop->ini = MINIMUM( unit->sel_prop->ini, target->terrain->max_ini );
        target->sel_prop->ini = MINIMUM( target->sel_prop->ini, target->terrain->max_ini );
    }
    unit->sel_prop->ini += ( unit->exp_level + 1  ) / 2;
    target->sel_prop->ini += ( target->exp_level + 1  ) / 2;
    /* special initiative rules:
       antitank inits attack tank|recon: atk 0, def 99
       tank inits attack against anti-tank: atk 0, def 99
       defensive fire: atk 99, def 0
       submarine attacks: atk 99, def 0
       ranged attack: atk 99, def 0
       rugged defense: atk 0
       air unit attacks air defense: atk = def 
       non-art vs art: atk 0, def 99 */
    if ( unit->sel_prop->flags & ANTI_TANK )
        if ( target->sel_prop->flags & TANK ) {
            unit->sel_prop->ini = 0;
            target->sel_prop->ini = 99;
        }
    if ( (unit->sel_prop->flags&DIVING) || 
         (unit->sel_prop->flags&ARTILLERY) || 
         (unit->sel_prop->flags&AIR_DEFENSE) || 
         type == UNIT_DEFENSIVE_ATTACK
    ) {
        unit->sel_prop->ini = 99;
        target->sel_prop->ini = 0;
    }
    if ( unit->sel_prop->flags & FLYING )
        if ( target->sel_prop->flags & AIR_DEFENSE )
            unit->sel_prop->ini = target->sel_prop->ini;
    if ( rugged_def )
        unit->sel_prop->ini = 0;
    if ( force_rugged )
        target->sel_prop->ini = 99;
    /* the dice is rolled after these changes */
    if ( real ) {
        unit->sel_prop->ini += DICE(3);
        target->sel_prop->ini += DICE(3);
    }
#ifdef DEBUG_ATTACK
    if ( real ) {
        printf( "%s Initiative: %i\n", unit->name, unit->sel_prop->ini );
        printf( "%s Initiative: %i\n", target->name, target->sel_prop->ini );
        if ( unit_check_rugged_def( unit, target ) )
            printf( "\nRugged Defense: %s (%i%%)\n",
                    rugged_def ? "yes" : "no",
                    unit_get_rugged_def_chance( unit, target ) );
    }
#endif
    /* in a real combat a submarine may evade */
    if ( real && type == UNIT_ACTIVE_ATTACK && ( target->sel_prop->flags & DIVING ) ) { 
        if ( DICE(10) <= 7 + ( target->exp_level - unit->exp_level ) / 2 )
        {
            strike = ATK_NO_STRIKE;
            ret |= AR_EVADED;
        }
        else
            strike = ATK_UNIT_FIRST;
#ifdef DEBUG_ATTACK
        printf ( "\nSubmarine Evasion: %s (%i%%)\n", 
                 (strike==ATK_NO_STRIKE)?"yes":"no",
                 10 * (7 + ( target->exp_level - unit->exp_level ) / 2) );
#endif
    }
    else
    /* who is first? */
    if ( unit->sel_prop->ini == target->sel_prop->ini )
        strike = ATK_BOTH_STRIKE;
    else
        if ( unit->sel_prop->ini > target->sel_prop->ini )
            strike = ATK_UNIT_FIRST;
        else
            strike = ATK_TARGET_FIRST;
    /* the one with the highest initiative begins first if not defensive fire or artillery */
    if ( strike == ATK_BOTH_STRIKE ) {
        /* both strike at the same time */
        unit_get_damage( unit, unit, target, type, real, rugged_def, &target_dam, &target_suppr );
        if ( unit_check_attack( target, unit, UNIT_PASSIVE_ATTACK ) )
            unit_get_damage( unit, target, unit, UNIT_PASSIVE_ATTACK, real, rugged_def, &unit_dam, &unit_suppr );
        unit_apply_damage( target, target_dam, target_suppr, unit );
        unit_apply_damage( unit, unit_dam, unit_suppr, target );
    }
    else
        if ( strike == ATK_UNIT_FIRST ) {
            /* unit strikes first */
            unit_get_damage( unit, unit, target, type, real, rugged_def, &target_dam, &target_suppr );
            if ( unit_apply_damage( target, target_dam, target_suppr, unit ) )
                if ( unit_check_attack( target, unit, UNIT_PASSIVE_ATTACK ) && type != UNIT_DEFENSIVE_ATTACK ) {
                    unit_get_damage( unit, target, unit, UNIT_PASSIVE_ATTACK, real, rugged_def, &unit_dam, &unit_suppr );
                    unit_apply_damage( unit, unit_dam, unit_suppr, target );
                }
        }
        else 
            if ( strike == ATK_TARGET_FIRST ) {
                /* target strikes first */
                if ( unit_check_attack( target, unit, UNIT_PASSIVE_ATTACK ) ) {
                    unit_get_damage( unit, target, unit, UNIT_PASSIVE_ATTACK, real, rugged_def, &unit_dam, &unit_suppr );
                    if ( !unit_apply_damage( unit, unit_dam, unit_suppr, target ) )
                        ret |= AR_UNIT_ATTACK_BROKEN_UP;
                }
                if ( unit_get_cur_str( unit ) > 0 ) {
                    unit_get_damage( unit, unit, target, type, real, rugged_def, &target_dam, &target_suppr );
                    unit_apply_damage( target, target_dam, target_suppr, unit );
                }
            }
    /* check return value */
    if ( unit->str == 0 )
        ret |= AR_UNIT_KILLED;
    else
        if ( unit_get_cur_str( unit ) == 0 )
            ret |= AR_UNIT_SUPPRESSED;
    if ( target->str == 0 )
        ret |= AR_TARGET_KILLED;
    else
        if ( unit_get_cur_str( target ) == 0 )
            ret |= AR_TARGET_SUPPRESSED;
    if ( rugged_def )
        ret |= AR_RUGGED_DEFENSE;
    if ( real ) {
        /* cost ammo */
        if ( config.supply ) {
            //if (DICE(10)<=target_old_str)
                unit->cur_ammo--;
            if ( unit_check_attack( target, unit, UNIT_PASSIVE_ATTACK ) && target->cur_ammo > 0 )
                //if (DICE(10)<=unit_old_str)
                    target->cur_ammo--;
        }
        /* costs attack */
        if ( unit->cur_atk_count > 0 ) unit->cur_atk_count--;
        /* target: loose entrenchment if damage was taken or with a unit->str*10% chance */
        if ( target->entr > 0 ) 
            if (target_dam > 0 || DICE(10)<=unit_old_str)
                target->entr--;
        /* attacker looses entrenchment if it got hurt */
        if ( unit->entr > 0 && unit_dam > 0 )
            unit->entr--;
        /* gain experience */
        exp_mod = target->exp_level - unit->exp_level;
        if ( exp_mod < 1 ) exp_mod = 1;
        unit_add_exp( unit, exp_mod * target_dam + unit_dam );
        exp_mod = unit->exp_level - target->exp_level;
        if ( exp_mod < 1 ) exp_mod = 1;
        unit_add_exp( target, exp_mod * unit_dam + target_dam );
        if ( unit_is_close( unit, target ) ) {
            unit_add_exp( unit, 10 );
            unit_add_exp( target, 10 );
        }
	/* gain prestige */
	unit_gain_prestige( unit, target, target_dam );
	unit_gain_prestige( target, unit, unit_dam );
        /* adjust life bars */
        update_bar( unit );
        update_bar( target );
    }
    unit->sel_prop->ini = unit_old_ini;
    target->sel_prop->ini = target_old_ini;
    return ret;
}