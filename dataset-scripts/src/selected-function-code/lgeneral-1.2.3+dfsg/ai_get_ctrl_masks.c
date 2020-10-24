void ai_get_ctrl_masks()
{
    CtrlCtx ctx;
    int i, j;
    Unit *unit;
    map_clear_mask( F_CTRL_GRND | F_CTRL_AIR | F_CTRL_SEA );
    list_reset( units );
    while ( ( unit = list_next( units ) ) ) {
        if ( unit->killed ) continue;
        map_get_unit_move_mask( unit );
        /* build context */
        ctx.unit = unit;
        ctx.score = (player_is_ally( unit->player, cur_player ))?unit->eval_score:-unit->eval_score;
        ctx.op_region = (unit->sel_prop->flags&FLYING)?1:(unit->sel_prop->flags&SWIMMING)?2:0;
        /* work through move mask and modify ctrl mask by adding score
           for all tiles in movement and attack range once */
        for ( i  = MAXIMUM( 0, unit->x - unit->cur_mov ); 
              i <= MINIMUM( map_w - 1, unit->x + unit->cur_mov );
              i++ )
            for ( j  = MAXIMUM( 0, unit->y - unit->cur_mov ); 
                  j <= MINIMUM( map_h - 1, unit->y + unit->cur_mov ); 
                  j++ )
                if ( mask[i][j].in_range ) {
                    switch ( ctx.op_region ) {
                        case 0: mask[i][j].ctrl_grnd += ctx.score; break;
                        case 1: mask[i][j].ctrl_air += ctx.score; break;
                        case 2: mask[i][j].ctrl_sea += ctx.score; break;
                    }
                    ai_eval_hexes( i, j, MAXIMUM( 1, unit->sel_prop->rng ), 
                                   ai_eval_ctrl, &ctx );
                }
    }
}