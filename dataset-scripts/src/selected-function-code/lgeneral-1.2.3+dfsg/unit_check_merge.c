int unit_check_merge( Unit *unit, Unit *source )
{
    /* units must not be sea/air embarked */
    if ( unit->embark != EMBARK_NONE || source->embark != EMBARK_NONE ) return 0;
    /* same class */
    if ( unit->prop.class != source->prop.class ) return 0;
    /* same player */
    if ( !player_is_ally( unit->player, source->player ) ) return 0;
    /* first unit must not have moved so far */
    if ( !unit->unused ) return 0;
    /* both units must have same movement type */
    if ( unit->prop.mov_type != source->prop.mov_type ) return 0;
    /* the unit strength must not exceed limit */
    if ( unit->str + source->str > 13 ) return 0;
    /* fortresses (unit-class 7) could not merge */
    if ( unit->prop.class == 7 ) return 0;
    /* artillery with different ranges may not merge */
    if (unit->prop.flags&ARTILLERY && unit->prop.rng!=source->prop.rng) return 0;
    /* not failed so far: allow merge */
    return 1;
}