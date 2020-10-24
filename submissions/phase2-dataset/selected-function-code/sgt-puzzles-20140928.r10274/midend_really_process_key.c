static int midend_really_process_key(midend *me, int x, int y, int button)
{
    game_state *oldstate =
        me->ourgame->dup_game(me->states[me->statepos - 1].state);
    int type = MOVE, gottype = FALSE, ret = 1;
    float anim_time;
    game_state *s;
    char *movestr;
	
    movestr =
	me->ourgame->interpret_move(me->states[me->statepos-1].state,
				    me->ui, me->drawstate, x, y, button);

    if (!movestr) {
	if (button == 'n' || button == 'N' || button == '\x0E') {
	    midend_stop_anim(me);
	    midend_new_game(me);
	    midend_redraw(me);
	    goto done;		       /* never animate */
	} else if (button == 'u' || button == 'u' ||
		   button == '\x1A' || button == '\x1F') {
	    midend_stop_anim(me);
	    type = me->states[me->statepos-1].movetype;
	    gottype = TRUE;
	    if (!midend_undo(me))
		goto done;
	} else if (button == 'r' || button == 'R' ||
		   button == '\x12' || button == '\x19') {
	    midend_stop_anim(me);
	    if (!midend_redo(me))
		goto done;
	} else if (button == '\x13' && me->ourgame->can_solve) {
	    if (midend_solve(me))
		goto done;
	} else if (button == 'q' || button == 'Q' || button == '\x11') {
	    ret = 0;
	    goto done;
	} else
	    goto done;
    } else {
	if (!*movestr)
	    s = me->states[me->statepos-1].state;
	else {
	    s = me->ourgame->execute_move(me->states[me->statepos-1].state,
					  movestr);
	    assert(s != NULL);
	}

        if (s == me->states[me->statepos-1].state) {
            /*
             * make_move() is allowed to return its input state to
             * indicate that although no move has been made, the UI
             * state has been updated and a redraw is called for.
             */
            midend_redraw(me);
            midend_set_timer(me);
            goto done;
        } else if (s) {
	    midend_stop_anim(me);
            midend_purge_states(me);
            ensure(me);
            assert(movestr != NULL);
            me->states[me->nstates].state = s;
            me->states[me->nstates].movestr = movestr;
            me->states[me->nstates].movetype = MOVE;
            me->statepos = ++me->nstates;
            me->dir = +1;
	    if (me->ui)
		me->ourgame->changed_state(me->ui,
					   me->states[me->statepos-2].state,
					   me->states[me->statepos-1].state);
        } else {
            goto done;
        }
    }

    if (!gottype)
        type = me->states[me->statepos-1].movetype;

    /*
     * See if this move requires an animation.
     */
    if (special(type) && !(type == SOLVE &&
			   (me->ourgame->flags & SOLVE_ANIMATES))) {
        anim_time = 0;
    } else {
        anim_time = me->ourgame->anim_length(oldstate,
                                             me->states[me->statepos-1].state,
                                             me->dir, me->ui);
    }

    me->oldstate = oldstate; oldstate = NULL;
    if (anim_time > 0) {
        me->anim_time = anim_time;
    } else {
        me->anim_time = 0.0;
	midend_finish_move(me);
    }
    me->anim_pos = 0.0;

    midend_redraw(me);

    midend_set_timer(me);

    done:
    if (oldstate) me->ourgame->free_game(oldstate);
    return ret;
}