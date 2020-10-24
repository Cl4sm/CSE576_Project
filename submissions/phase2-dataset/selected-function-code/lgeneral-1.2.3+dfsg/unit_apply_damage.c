static int unit_apply_damage( Unit *unit, int damage, int suppr, Unit *attacker )
{
    unit->str -= damage;
    if ( unit->str < 0 ) {
        unit->str = 0;
        return 0;
    }
    if ( attacker && attacker->sel_prop->flags & TURN_SUPPR ) {
        unit->turn_suppr += suppr;
        if ( unit->str - unit->turn_suppr - unit->suppr < 0 ) {
            unit->turn_suppr = unit->str - unit->suppr;
            return 0;
        }
    }
    else {
        unit->suppr += suppr;
        if ( unit->str - unit->turn_suppr - unit->suppr < 0 ) {
            unit->suppr = unit->str - unit->turn_suppr;
            return 0;
        }
    }
    return unit_get_cur_str( unit );
}