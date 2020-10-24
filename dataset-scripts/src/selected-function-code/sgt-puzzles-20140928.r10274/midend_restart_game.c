void midend_restart_game(midend *me)
{
    game_state *s;

    midend_stop_anim(me);

    assert(me->statepos >= 1);
    if (me->statepos == 1)
        return;                        /* no point doing anything at all! */

    /*
     * During restart, we reconstruct the game from the (public)
     * game description rather than from states[0], because that
     * way Mines gets slightly more sensible behaviour (restart
     * goes to _after_ the first click so you don't have to
     * remember where you clicked).
     */
    s = me->ourgame->new_game(me, me->params, me->desc);

    /*
     * Now enter the restarted state as the next move.
     */
    midend_stop_anim(me);
    midend_purge_states(me);
    ensure(me);
    me->states[me->nstates].state = s;
    me->states[me->nstates].movestr = dupstr(me->desc);
    me->states[me->nstates].movetype = RESTART;
    me->statepos = ++me->nstates;
    if (me->ui)
        me->ourgame->changed_state(me->ui,
                                   me->states[me->statepos-2].state,
                                   me->states[me->statepos-1].state);
    me->anim_time = 0.0;
    midend_finish_move(me);
    midend_redraw(me);
    midend_set_timer(me);
}