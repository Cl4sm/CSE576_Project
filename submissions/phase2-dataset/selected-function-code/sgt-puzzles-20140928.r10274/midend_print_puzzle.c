char *midend_print_puzzle(midend *me, document *doc, int with_soln)
{
    game_state *soln = NULL;

    if (me->statepos < 1)
	return "No game set up to print";/* _shouldn't_ happen! */

    if (with_soln) {
	char *msg, *movestr;

	if (!me->ourgame->can_solve)
	    return "This game does not support the Solve operation";

	msg = "Solve operation failed";/* game _should_ overwrite on error */
	movestr = me->ourgame->solve(me->states[0].state,
				     me->states[me->statepos-1].state,
				     me->aux_info, &msg);
	if (!movestr)
	    return msg;
	soln = me->ourgame->execute_move(me->states[me->statepos-1].state,
					 movestr);
	assert(soln);

	sfree(movestr);
    } else
	soln = NULL;

    /*
     * This call passes over ownership of the two game_states and
     * the game_params. Hence we duplicate the ones we want to
     * keep, and we don't have to bother freeing soln if it was
     * non-NULL.
     */
    document_add_puzzle(doc, me->ourgame,
			me->ourgame->dup_params(me->curparams),
			me->ourgame->dup_game(me->states[0].state), soln);

    return NULL;
}