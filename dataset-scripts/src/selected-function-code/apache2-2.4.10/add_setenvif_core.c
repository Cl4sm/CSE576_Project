static const char *add_setenvif_core(cmd_parms *cmd, void *mconfig,
                                     char *fname, const char *args)
{
    char *regex;
    const char *simple_pattern;
    sei_cfg_rec *sconf;
    sei_entry *new;
    sei_entry *entries;
    int i;
    int icase;

    /*
     * Determine from our context into which record to put the entry.
     * cmd->path == NULL means we're in server-wide context; otherwise,
     * we're dealing with a per-directory setting.
     */
    sconf = (cmd->path != NULL)
      ? (sei_cfg_rec *) mconfig
      : (sei_cfg_rec *) ap_get_module_config(cmd->server->module_config,
                                               &setenvif_module);
    entries = (sei_entry *) sconf->conditionals->elts;
    /* get regex */
    regex = ap_getword_conf(cmd->pool, &args);
    if (!*regex) {
        return apr_pstrcat(cmd->pool, "Missing regular expression for ",
                           cmd->cmd->name, NULL);
    }

    /*
     * If we've already got a sei_entry with the same name we want to
     * just copy the name pointer... so that later on we can compare
     * two header names just by comparing the pointers.
     */
    for (i = 0; i < sconf->conditionals->nelts; ++i) {
        new = &entries[i];
        if (new->name && !strcasecmp(new->name, fname)) {
            fname = new->name;
            break;
        }
    }

    /* if the last entry has an identical headername and regex then
     * merge with it
     */
    i = sconf->conditionals->nelts - 1;
    icase = cmd->info == ICASE_MAGIC;
    if (i < 0
        || entries[i].name != fname
        || entries[i].icase != icase
        || strcmp(entries[i].regex, regex)) {

        /* no match, create a new entry */
        new = apr_array_push(sconf->conditionals);
        new->name = fname;
        new->regex = regex;
        new->icase = icase;
        if ((simple_pattern = non_regex_pattern(cmd->pool, regex))) {
            new->pattern = apr_strmatch_precompile(cmd->pool,
                                                   simple_pattern, !icase);
            if (new->pattern == NULL) {
                return apr_pstrcat(cmd->pool, cmd->cmd->name,
                                   " pattern could not be compiled.", NULL);
            }
            new->preg = NULL;
        }
        else {
            new->preg = ap_pregcomp(cmd->pool, regex,
                                    (AP_REG_EXTENDED | (icase ? AP_REG_ICASE : 0)));
            if (new->preg == NULL) {
                return apr_pstrcat(cmd->pool, cmd->cmd->name,
                                   " regex could not be compiled.", NULL);
            }
            new->pattern = NULL;
        }
        new->features = apr_table_make(cmd->pool, 2);

        if (!strcasecmp(fname, "remote_addr")) {
            new->special_type = SPECIAL_REMOTE_ADDR;
        }
        else if (!strcasecmp(fname, "remote_host")) {
            new->special_type = SPECIAL_REMOTE_HOST;
        }
        else if (!strcasecmp(fname, "request_uri")) {
            new->special_type = SPECIAL_REQUEST_URI;
        }
        else if (!strcasecmp(fname, "request_method")) {
            new->special_type = SPECIAL_REQUEST_METHOD;
        }
        else if (!strcasecmp(fname, "request_protocol")) {
            new->special_type = SPECIAL_REQUEST_PROTOCOL;
        }
        else if (!strcasecmp(fname, "server_addr")) {
            new->special_type = SPECIAL_SERVER_ADDR;
        }
        else {
            new->special_type = SPECIAL_NOT;
            /* Handle fname as a regular expression.
             * If fname a simple header string, identify as such
             * (new->pnamereg = NULL) to avoid the overhead of searching
             * through headers_in for a regex match.
             */
            if (is_header_regex(cmd->temp_pool, fname)) {
                new->pnamereg = ap_pregcomp(cmd->pool, fname,
                                            (AP_REG_EXTENDED | AP_REG_NOSUB
                                             | (icase ? AP_REG_ICASE : 0)));
                if (new->pnamereg == NULL)
                    return apr_pstrcat(cmd->pool, cmd->cmd->name,
                                       "Header name regex could not be "
                                       "compiled.", NULL);
            }
            else {
                new->pnamereg = NULL;
            }
        }
    }
    else {
        new = &entries[i];
    }

    return add_envvars(cmd, args, new);
}