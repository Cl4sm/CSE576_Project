void ai_run( void )
{
    Unit *partners[MAP_MERGE_UNIT_LIMIT];
    int partner_count;
    int i, j, x, y, dx, dy, dist, found;
    Unit *unit = 0, *best = 0;
    switch ( ai_status ) {
        case AI_STATUS_DEPLOY:
            /* deploy unit? */
            if ( avail_units->count > 0 && ( unit = list_next( avail_units ) ) ) {
                if ( deploy_turn ) {
                    x = unit->x; y = unit->y;
                    assert( x >= 0 && y >= 0 );
                    map_remove_unit( unit );
                    found = 1;
                } else {
                    map_get_deploy_mask(cur_player,unit,0);
                    map_clear_mask( F_AUX );
                    for ( i = 1; i < map_w - 1; i++ )
                        for ( j = 1; j < map_h - 1; j++ )
                            if ( mask[i][j].deploy )
                                if ( ai_get_dist( unit, i, j, AI_FIND_ENEMY_OBJ, &x, &y, &dist ) )
                                    mask[i][j].aux = dist + 1;
                    dist = 1000; found = 0;
                    for ( i = 1; i < map_w - 1; i++ )
                        for ( j = 1; j < map_h - 1; j++ )
                            if ( mask[i][j].aux > 0 && mask[i][j].aux < dist ) {
                                dist = mask[i][j].aux;
                                x = i; y = j;
                                found = 1; /* deploy close to enemy */
                            }
                }
                if ( found ) {
                    action_queue_deploy( unit, x, y );
                    list_reset( avail_units );
                    list_add( ai_units, unit );
                    AI_DEBUG( 1, "%s deployed to %i,%i\n", unit->name, x, y );
                    return;
                }
            }
            else {
                ai_status = deploy_turn ? AI_STATUS_END : AI_STATUS_MERGE;
                list_reset( ai_units );
                AI_DEBUG( 0, deploy_turn ? "*** END TURN ***\n" : 
							"*** MERGE ***\n" );
            }
            break;
        case AI_STATUS_SUPPLY:
            /* get next unit */
            if ( ( unit = list_next( ai_units ) ) == 0 ) {
                ai_status = AI_STATUS_GROUP;
                /* build a group with all units, -1,-1 as destination means it will
                   simply attack/defend the nearest target. later on this should
                   split up into several groups with different target and strategy */
                ai_group = ai_group_create( cur_player->strat, -1, -1 );
                list_reset( ai_units );
                while ( ( unit = list_next( ai_units ) ) )
                    ai_group_add_unit( ai_group, unit );
                AI_DEBUG( 0, "*** MOVE & ATTACK ***\n" );
            }
            else {
                /* check if unit needs supply and remove 
                   it from ai_units if so */
                if ( ( unit_low_fuel( unit ) || unit_low_ammo( unit ) ) ) {
                    if ( unit->supply_level > 0 ) {
                        action_queue_supply( unit );
                        list_delete_item( ai_units, unit );
                        AI_DEBUG( 1, "%s supplies\n", unit->name );
                        break;
                    }
                    else {
                        AI_DEBUG( 1, "%s searches depot\n", unit->name );
                        if ( ai_get_dist( unit, unit->x, unit->y, AI_FIND_DEPOT,
                                          &dx, &dy, &dist ) )
                            if ( ai_approximate( unit, dx, dy, &x, &y ) ) {
                                action_queue_move( unit, x, y );
                                list_delete_item( ai_units, unit );
                                AI_DEBUG( 1, "%s moves to %i,%i\n", unit->name, x, y );
                                break;
                            }
                    }
                }
            }
            break;
        case AI_STATUS_MERGE:
            if ( ( unit = list_next( ai_units ) ) ) {
                map_get_merge_units( unit, partners, &partner_count );
                best = 0; /* merge with the one that has the most strength points */
                for ( i = 0; i < partner_count; i++ )
                    if ( best == 0 )
                        best = partners[i];
                    else
                        if ( best->str < partners[i]->str )
                            best = partners[i];
                if ( best ) {
                    AI_DEBUG( 1, "%s merges with %s\n", unit->name, best->name );
                    action_queue_merge( unit, best );
                    /* both units are handled now */
                    list_delete_item( ai_units, unit );
                    list_delete_item( ai_units, best ); 
                }
            }
            else {
                ai_status = AI_STATUS_SUPPLY;
                list_reset( ai_units );
                AI_DEBUG( 0, "*** SUPPLY ***\n" );
            }
            break;
        case AI_STATUS_GROUP:
            if ( !ai_group_handle_next_unit( ai_group ) ) {
                ai_group_delete( ai_group );
                if (config.purchase) {
                    ai_status = AI_STATUS_PURCHASE;
                    AI_DEBUG( 0, "*** PURCHASE ***\n" );
                } else {
                    ai_status = AI_STATUS_END;
                    AI_DEBUG( 0, "*** END TURN ***\n" );
                }
            }
            break;
        case AI_STATUS_PURCHASE:
            ai_purchase_units();
            ai_status = AI_STATUS_END;
            AI_DEBUG( 0, "*** END TURN ***\n" );
            break;
        case AI_STATUS_END:
            action_queue_end_turn();
            ai_status = AI_STATUS_FINALIZE;
            break;
    }
}