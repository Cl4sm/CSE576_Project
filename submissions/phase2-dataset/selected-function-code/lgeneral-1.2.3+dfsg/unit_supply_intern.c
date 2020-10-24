int unit_supply_intern( Unit *unit, int type )
{
    int amount_ammo, amount_fuel, max, supply_amount;
    int supplied = 0;
    /* ammo */
    if ( type == UNIT_SUPPLY_AMMO || type == UNIT_SUPPLY_ALL )
    if ( unit_check_supply( unit, UNIT_SUPPLY_AMMO, &amount_ammo, &amount_fuel ) ) {
        max = unit->cur_ammo + amount_ammo ;
        supply_amount = unit->supply_level * max / 100;
        if ( supply_amount == 0 ) supply_amount = 1; /* at least one */
        unit->cur_ammo += supply_amount;
        if ( unit->cur_ammo > max ) unit->cur_ammo = max;
        supplied = 1;
    }
    /* fuel */
    if ( type == UNIT_SUPPLY_FUEL || type == UNIT_SUPPLY_ALL )
    if ( unit_check_supply( unit, UNIT_SUPPLY_FUEL, &amount_ammo, &amount_fuel ) ) {
        max = unit->cur_fuel + amount_fuel ;
        supply_amount = unit->supply_level * max / 100;
        if ( supply_amount == 0 ) supply_amount = 1; /* at least one */
        unit->cur_fuel += supply_amount;
        if ( unit->cur_fuel > max ) unit->cur_fuel = max;
        supplied = 1;
    }
    return supplied;
}