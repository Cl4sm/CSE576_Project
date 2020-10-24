int unit_check_supply( Unit *unit, int type, int *missing_ammo, int *missing_fuel )
{
    int ret = 0;
    int max_fuel = unit->sel_prop->fuel;
    if ( missing_ammo )
        *missing_ammo = 0;
    if ( missing_fuel )
        *missing_fuel = 0;
    /* no supply near or already moved? */
    if ( unit->embark == EMBARK_SEA || unit->embark == EMBARK_AIR ) return 0;
    if ( unit->supply_level == 0 ) return 0;
    if ( !unit->unused ) return 0;
    /* supply ammo? */
    if ( type == UNIT_SUPPLY_AMMO || type == UNIT_SUPPLY_ANYTHING )
        if ( unit->cur_ammo < unit->prop.ammo ) {
            ret = 1;
            if ( missing_ammo )
                *missing_ammo = unit->prop.ammo - unit->cur_ammo;
        }
    if ( type == UNIT_SUPPLY_AMMO ) return ret;
    /* if we have a ground transporter assigned we need to use it's fuel as max */
    if ( unit_check_fuel_usage( unit ) && max_fuel == 0 )
        max_fuel = unit->trsp_prop.fuel;
    /* supply fuel? */
    if ( type == UNIT_SUPPLY_FUEL || type == UNIT_SUPPLY_ANYTHING )
        if ( unit->cur_fuel < max_fuel ) {
            ret = 1;
            if ( missing_fuel )
                *missing_fuel = max_fuel - unit->cur_fuel;
        }
    return ret;
}