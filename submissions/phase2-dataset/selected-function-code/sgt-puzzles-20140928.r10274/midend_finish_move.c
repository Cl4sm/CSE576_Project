static void midend_finish_move(midend *me)
{
    float flashtime;

    /*
     * We do not flash if the later of the two states is special.
     * This covers both forward Solve moves and backward (undone)
     * Restart moves.
     */
    if ((me->oldstate || me->statepos > 1) &&
        ((me->dir > 0 && !special(me->states[me->statepos-1].movetype)) ||
         (me->dir < 0 && me->statepos < me->nstates &&
          !special(me->states[me->statepos].movetype)))) {
	flashtime = me->ourgame->flash_length(me->oldstate ? me->oldstate :
					      me->states[me->statepos-2].state,
					      me->states[me->statepos-1].state,
					      me->oldstate ? me->dir : +1,
					      me->ui);
	if (flashtime > 0) {
	    me->flash_pos = 0.0F;
	    me->flash_time = flashtime;
	}
    }

    if (me->oldstate)
	me->ourgame->free_game(me->oldstate);
    me->oldstate = NULL;
    me->anim_pos = me->anim_time = 0;
    me->dir = 0;

    midend_set_timer(me);
}