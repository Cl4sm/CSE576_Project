static const char *add_setenvifexpr(cmd_parms *cmd, void *mconfig,
                                    const char *args)
{
    char *expr;
    sei_cfg_rec *sconf;
    sei_entry *new;
    const char *err;

    /*
     * Determine from our context into which record to put the entry.
     * cmd->path == NULL means we're in server-wide context; otherwise,
     * we're dealing with a per-directory setting.
     */
    sconf = (cmd->path != NULL)
      ? (sei_cfg_rec *) mconfig
      : (sei_cfg_rec *) ap_get_module_config(cmd->server->module_config,
                                               &setenvif_module);
    /* get expr */
    expr = ap_getword_conf(cmd->pool, &args);
    if (!*expr) {
        return apr_pstrcat(cmd->pool, "Missing expression for ",
                           cmd->cmd->name, NULL);
    }

    new = apr_array_push(sconf->conditionals);
    new->features = apr_table_make(cmd->pool, 2);
    new->name = NULL;
    new->regex = NULL;
    new->pattern = NULL;
    new->preg = NULL;
    new->expr = ap_expr_parse_cmd(cmd, expr, 0, &err, NULL);
    if (err)
        return apr_psprintf(cmd->pool, "Could not parse expression \"%s\": %s",
                            expr, err);

    return add_envvars(cmd, args, new);
}