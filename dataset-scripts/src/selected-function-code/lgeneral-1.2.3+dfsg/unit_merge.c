void unit_merge( Unit *unit, Unit *source )
{
    /* units relative weight */
    float weight1, weight2, total;
    int i, neg;
    /* compute weight */
    weight1 = unit->str; weight2 = source->str;
    total = unit->str + source->str;
    /* adjust so weight1 + weigth2 = 1 */
    weight1 /= total; weight2 /= total;
    /* no other actions allowed */
    unit->unused = 0; unit->cur_mov = 0; unit->cur_atk_count = 0;
    /* update cost since used for gaining prestige */
    unit->prop.cost = (unit->prop.cost * unit->str + 
					source->prop.cost * source->str) / 
					(unit->str + source->str);
    /* repair damage */
    unit->str += source->str;
    /* reorganization costs some entrenchment: the new units are assumed to have
       entrenchment 0 since they come. new entr is rounded weighted sum */
    unit->entr = floor((float)unit->entr*weight1+0.5); /* + 0 * weight2 */
    /* update experience */
    i = (int)( weight1 * unit->exp + weight2 * source->exp );
    unit->exp = 0; unit_add_exp( unit, i );
    /* update unit::prop */
    /* related initiative */
    unit->prop.ini = (int)( weight1 * unit->prop.ini + weight2 * source->prop.ini );
    /* minimum movement */
    if ( source->prop.mov < unit->prop.mov )
        unit->prop.mov = source->prop.mov;
    /* maximum spotting */
    if ( source->prop.spt > unit->prop.spt )
        unit->prop.spt = source->prop.spt;
    /* maximum range */
    if ( source->prop.rng > unit->prop.rng )
        unit->prop.rng = source->prop.rng;
    /* relative attack count */
    unit->prop.atk_count = (int)( weight1 * unit->prop.atk_count + weight2 * source->prop.atk_count );
    if ( unit->prop.atk_count == 0 ) unit->prop.atk_count = 1;
    /* relative attacks */
    /* if attack is negative simply use absolute value; only restore negative if both units are negative */
    for ( i = 0; i < trgt_type_count; i++ ) {
        neg = ( unit->prop.atks[i] < 0 && source->prop.atks[i] < 0 );
        unit->prop.atks[i] = (int)( weight1 * abs( unit->prop.atks[i] ) + weight2 * ( source->prop.atks[i] ) );
        if ( neg ) unit->prop.atks[i] *= -1;
    }
    /* relative defence */
    unit->prop.def_grnd = (int)( weight1 * unit->prop.def_grnd + weight2 * source->prop.def_grnd );
    unit->prop.def_air = (int)( weight1 * unit->prop.def_air + weight2 * source->prop.def_air );
    unit->prop.def_cls = (int)( weight1 * unit->prop.def_cls + weight2 * source->prop.def_cls );
    /* relative ammo */
    unit->prop.ammo = (int)( weight1 * unit->prop.ammo + weight2 * source->prop.ammo );
    unit->cur_ammo = (int)( weight1 * unit->cur_ammo + weight2 * source->cur_ammo );
    /* relative fuel */
    unit->prop.fuel = (int)( weight1 * unit->prop.fuel + weight2 * source->prop.fuel );
    unit->cur_fuel = (int)( weight1 * unit->cur_fuel + weight2 * source->cur_fuel );
    /* merge flags */
    unit->prop.flags |= source->prop.flags;
    /* sounds, picture are kept */
    /* unit::trans_prop isn't updated so far: */
    /* transporter of first unit is kept if any else second unit's transporter is used */
    if ( unit->trsp_prop.id == 0 && source->trsp_prop.id ) {
        memcpy( &unit->trsp_prop, &source->trsp_prop, sizeof( Unit_Lib_Entry ) );
        /* as this must be a ground transporter copy current fuel value */
        unit->cur_fuel = source->cur_fuel;
    }
    update_bar( unit );
}