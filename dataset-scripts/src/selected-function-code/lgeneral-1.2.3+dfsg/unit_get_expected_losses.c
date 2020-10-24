void unit_get_expected_losses( Unit *unit, Unit *target, int *unit_damage, int *target_damage )
{
    int damage, suppr;
    Unit *df;
    List *df_units = list_create( LIST_NO_AUTO_DELETE, LIST_NO_CALLBACK );
#ifdef DEBUG_ATTACK
    printf( "***********************\n" );
#endif    
    unit_get_df_units( unit, target, vis_units, df_units );
    unit_backup( unit ); unit_backup( target );
    /* let defensive fire go to work (no chance to defend against this) */
    list_reset( df_units );
    while ( ( df = list_next( df_units ) ) ) {
        unit_get_damage( unit, df, unit, UNIT_DEFENSIVE_ATTACK, 0, 0, &damage, &suppr );
        if ( !unit_apply_damage( unit, damage, suppr, 0 ) ) break;
    }
    /* actual fight if attack has strength remaining */
    if ( unit_get_cur_str( unit ) > 0 )
        unit_attack( unit, target, UNIT_ACTIVE_ATTACK, 0, 0 );
    /* get done damage */
    *unit_damage = unit->str;
    *target_damage = target->str;
    unit_restore( unit ); unit_restore( target );
    *unit_damage = unit->str - *unit_damage;
    *target_damage = target->str - *target_damage;
    list_delete( df_units );
}