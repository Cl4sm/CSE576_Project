config_item *midend_get_config(midend *me, int which, char **wintitle)
{
    char *titlebuf, *parstr, *rest;
    config_item *ret;
    char sep;

    assert(wintitle);
    titlebuf = snewn(40 + strlen(me->ourgame->name), char);

    switch (which) {
      case CFG_SETTINGS:
	sprintf(titlebuf, "%s configuration", me->ourgame->name);
	*wintitle = titlebuf;
	return me->ourgame->configure(me->params);
      case CFG_SEED:
      case CFG_DESC:
        if (!me->curparams) {
          sfree(titlebuf);
          return NULL;
        }
        sprintf(titlebuf, "%s %s selection", me->ourgame->name,
                which == CFG_SEED ? "random" : "game");
        *wintitle = titlebuf;

	ret = snewn(2, config_item);

	ret[0].type = C_STRING;
        if (which == CFG_SEED)
            ret[0].name = "Game random seed";
        else
            ret[0].name = "Game ID";
	ret[0].ival = 0;
        /*
         * For CFG_DESC the text going in here will be a string
         * encoding of the restricted parameters, plus a colon,
         * plus the game description. For CFG_SEED it will be the
         * full parameters, plus a hash, plus the random seed data.
         * Either of these is a valid full game ID (although only
         * the former is likely to persist across many code
         * changes).
         */
        parstr = me->ourgame->encode_params(me->curparams, which == CFG_SEED);
        assert(parstr);
        if (which == CFG_DESC) {
            rest = me->desc ? me->desc : "";
            sep = ':';
        } else {
            rest = me->seedstr ? me->seedstr : "";
            sep = '#';
        }
        ret[0].sval = snewn(strlen(parstr) + strlen(rest) + 2, char);
        sprintf(ret[0].sval, "%s%c%s", parstr, sep, rest);
        sfree(parstr);

	ret[1].type = C_END;
	ret[1].name = ret[1].sval = NULL;
	ret[1].ival = 0;

	return ret;
    }

    assert(!"We shouldn't be here");
    return NULL;
}