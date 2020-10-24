static int ai_check_hex_type( Unit *unit, int type, int x, int y )
{
    switch ( type ) {
        case AI_FIND_DEPOT:
            if ( map_is_allied_depot( &map[x][y], unit ) )
                return 1;
            break;
        case AI_FIND_ENEMY_OBJ:
            if ( !map[x][y].obj ) return 0;
        case AI_FIND_ENEMY_TOWN:
            if ( map[x][y].player && !player_is_ally( unit->player, map[x][y].player ) )
                return 1;
            break;
        case AI_FIND_OWN_OBJ:
            if ( !map[x][y].obj ) return 0;
        case AI_FIND_OWN_TOWN:
            if ( map[x][y].player && player_is_ally( unit->player, map[x][y].player ) )
                return 1;
            break;
    }
    return 0;
}