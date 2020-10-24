char *midend_set_config(midend *me, int which, config_item *cfg)
{
    char *error;
    game_params *params;

    switch (which) {
      case CFG_SETTINGS:
	params = me->ourgame->custom_params(cfg);
	error = me->ourgame->validate_params(params, TRUE);

	if (error) {
	    me->ourgame->free_params(params);
	    return error;
	}

	me->ourgame->free_params(me->params);
	me->params = params;
	break;

      case CFG_SEED:
      case CFG_DESC:
        error = midend_game_id_int(me, cfg[0].sval,
                                   (which == CFG_SEED ? DEF_SEED : DEF_DESC));
	if (error)
	    return error;
	break;
    }

    return NULL;
}