char *midend_solve(midend *me)
{
    game_state *s;
    char *msg, *movestr;

    if (!me->ourgame->can_solve)
	return "This game does not support the Solve operation";

    if (me->statepos < 1)
	return "No game set up to solve";   /* _shouldn't_ happen! */

    msg = NULL;
    movestr = me->ourgame->solve(me->states[0].state,
				 me->states[me->statepos-1].state,
				 me->aux_info, &msg);
    if (!movestr) {
	if (!msg)
	    msg = "Solve operation failed";   /* _shouldn't_ happen, but can */
	return msg;
    }
    s = me->ourgame->execute_move(me->states[me->statepos-1].state, movestr);
    assert(s);

    /*
     * Now enter the solved state as the next move.
     */
    midend_stop_anim(me);
    midend_purge_states(me);
    ensure(me);
    me->states[me->nstates].state = s;
    me->states[me->nstates].movestr = movestr;
    me->states[me->nstates].movetype = SOLVE;
    me->statepos = ++me->nstates;
    if (me->ui)
        me->ourgame->changed_state(me->ui,
                                   me->states[me->statepos-2].state,
                                   me->states[me->statepos-1].state);
    me->dir = +1;
    if (me->ourgame->flags & SOLVE_ANIMATES) {
	me->oldstate = me->ourgame->dup_game(me->states[me->statepos-2].state);
        me->anim_time =
	    me->ourgame->anim_length(me->states[me->statepos-2].state,
				     me->states[me->statepos-1].state,
				     +1, me->ui);
        me->anim_pos = 0.0;
    } else {
	me->anim_time = 0.0;
	midend_finish_move(me);
    }
    if (me->drawing)
        midend_redraw(me);
    midend_set_timer(me);
    return NULL;
}