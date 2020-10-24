void midend_new_game(midend *me)
{
    midend_free_game(me);

    assert(me->nstates == 0);

    if (me->genmode == GOT_DESC) {
	me->genmode = GOT_NOTHING;
    } else {
        random_state *rs;

        if (me->genmode == GOT_SEED) {
            me->genmode = GOT_NOTHING;
        } else {
            /*
             * Generate a new random seed. 15 digits comes to about
             * 48 bits, which should be more than enough.
             * 
             * I'll avoid putting a leading zero on the number,
             * just in case it confuses anybody who thinks it's
             * processed as an integer rather than a string.
             */
            char newseed[16];
            int i;
            newseed[15] = '\0';
            newseed[0] = '1' + (char)random_upto(me->random, 9);
            for (i = 1; i < 15; i++)
                newseed[i] = '0' + (char)random_upto(me->random, 10);
            sfree(me->seedstr);
            me->seedstr = dupstr(newseed);

	    if (me->curparams)
		me->ourgame->free_params(me->curparams);
	    me->curparams = me->ourgame->dup_params(me->params);
        }

	sfree(me->desc);
	sfree(me->privdesc);
        sfree(me->aux_info);
	me->aux_info = NULL;

        rs = random_new(me->seedstr, strlen(me->seedstr));
	/*
	 * If this midend has been instantiated without providing a
	 * drawing API, it is non-interactive. This means that it's
	 * being used for bulk game generation, and hence we should
	 * pass the non-interactive flag to new_desc.
	 */
        me->desc = me->ourgame->new_desc(me->curparams, rs,
					 &me->aux_info, (me->drawing != NULL));
	me->privdesc = NULL;
        random_free(rs);
    }

    ensure(me);

    /*
     * It might seem a bit odd that we're using me->params to
     * create the initial game state, rather than me->curparams
     * which is better tailored to this specific game and which we
     * always know.
     * 
     * It's supposed to be an invariant in the midend that
     * me->params and me->curparams differ in no aspect that is
     * important after generation (i.e. after new_desc()). By
     * deliberately passing the _less_ specific of these two
     * parameter sets, we provoke play-time misbehaviour in the
     * case where a game has failed to encode a play-time parameter
     * in the non-full version of encode_params().
     */
    me->states[me->nstates].state =
	me->ourgame->new_game(me, me->params, me->desc);

    /*
     * As part of our commitment to self-testing, test the aux
     * string to make sure nothing ghastly went wrong.
     */
    if (me->ourgame->can_solve && me->aux_info) {
	game_state *s;
	char *msg, *movestr;

	msg = NULL;
	movestr = me->ourgame->solve(me->states[0].state,
				     me->states[0].state,
				     me->aux_info, &msg);
	assert(movestr && !msg);
	s = me->ourgame->execute_move(me->states[0].state, movestr);
	assert(s);
	me->ourgame->free_game(s);
	sfree(movestr);
    }

    me->states[me->nstates].movestr = NULL;
    me->states[me->nstates].movetype = NEWGAME;
    me->nstates++;
    me->statepos = 1;
    me->drawstate = me->ourgame->new_drawstate(me->drawing,
					       me->states[0].state);
    midend_size_new_drawstate(me);
    me->elapsed = 0.0F;
    if (me->ui)
        me->ourgame->free_ui(me->ui);
    me->ui = me->ourgame->new_ui(me->states[0].state);
    midend_set_timer(me);
    me->pressed_mouse_button = 0;

    if (me->game_id_change_notify_function)
        me->game_id_change_notify_function(me->game_id_change_notify_ctx);
}