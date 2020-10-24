void ai_init( void )
{
    List *list; /* used to speed up the creation of ai_units */
    Unit *unit;
	
    AI_DEBUG(0, "AI Turn: %s\n", cur_player->name );
    if ( ai_status != AI_STATUS_INIT ) {
        AI_DEBUG(0, "Aborted: Bad AI Status: %i\n", ai_status );
        return;
    }
    
    finalized = 0;
    /* get all cur_player units, those with defensive fire come first */
    list = list_create( LIST_NO_AUTO_DELETE, LIST_NO_CALLBACK );
    list_reset( units );
    while ( ( unit = list_next( units ) ) )
        if ( unit->player == cur_player )
            list_add( list, unit );
    ai_units = list_create( LIST_NO_AUTO_DELETE, LIST_NO_CALLBACK );
    list_reset( list );
    while ( ( unit = list_next( list ) ) )
        if ( unit->sel_prop->flags & ARTILLERY || unit->sel_prop->flags & AIR_DEFENSE ) {
            list_add( ai_units, unit );
            list_delete_item( list, unit );
        }
    list_reset( list );
    while ( ( unit = list_next( list ) ) ) {
        if ( unit->killed ) 
		AI_DEBUG( 0, "!!Unit %s is dead!!\n", unit->name );
        list_add( ai_units, unit );
    }
    list_delete( list );
    list_reset( ai_units );
    AI_DEBUG(1, "Units: %i\n", ai_units->count );
    /* evaluate all units for strategic computations */
    AI_DEBUG( 1, "Evaluating units...\n" );
    ai_eval_units();
    /* build control masks */
    AI_DEBUG( 1, "Building control mask...\n" );
    //ai_get_ctrl_masks();
    /* check new units first */
    ai_status = AI_STATUS_DEPLOY; 
    list_reset( avail_units );
    AI_DEBUG( 0, "AI Initialized\n" );
    AI_DEBUG( 0, "*** DEPLOY ***\n" );
}