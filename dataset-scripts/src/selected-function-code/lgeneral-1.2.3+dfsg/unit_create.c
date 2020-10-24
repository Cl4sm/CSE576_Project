Unit *unit_create( Unit_Lib_Entry *prop, Unit_Lib_Entry *trsp_prop, Unit *base )
{
    Unit *unit = 0;
    if ( prop == 0 ) return 0;
    unit = calloc( 1, sizeof( Unit ) );
    /* shallow copy of properties */
    memcpy( &unit->prop, prop, sizeof( Unit_Lib_Entry ) );
    unit->sel_prop = &unit->prop; 
    unit->embark = EMBARK_NONE;
    /* assign the passed transporter without any check */
    if ( trsp_prop && !( prop->flags & FLYING ) && !( prop->flags & SWIMMING ) ) {
        memcpy( &unit->trsp_prop, trsp_prop, sizeof( Unit_Lib_Entry ) );
        /* a sea/air ground transporter is active per default */
        if ( trsp_prop->flags & SWIMMING ) {
            unit->embark = EMBARK_SEA;
            unit->sel_prop = &unit->trsp_prop;
        }
        if ( trsp_prop->flags & FLYING ) {
            unit->embark = EMBARK_AIR;
            unit->sel_prop = &unit->trsp_prop;
        }
    }
    /* copy the base values */
    unit->delay = base->delay;
    unit->x = base->x; unit->y = base->y;
    unit->str = base->str; unit->entr = base->entr;
    unit->player = base->player;
    unit->nation = base->nation;
    strcpy_lt( unit->name, base->name, 20 );
    unit_add_exp( unit, base->exp_level * 100 );
    unit->orient = base->orient;
    unit_adjust_icon( unit );
    unit->unused = 1;
    unit->supply_level = 100;
    unit->cur_ammo = unit->prop.ammo;
    unit->cur_fuel = unit->prop.fuel;
    if ( unit->cur_fuel == 0 && unit->trsp_prop.id && unit->trsp_prop.fuel > 0 )
        unit->cur_fuel = unit->trsp_prop.fuel;
    strcpy_lt( unit->tag, base->tag, 31 );
    /* update life bar properties */
    update_bar( unit );
    /* allocate backup mem */
    unit->backup = calloc( 1, sizeof( Unit ) );
    return unit;
}