static const char *header_cmd(cmd_parms *cmd, void *indirconf,
                              const char *args)
{
    const char *action;
    const char *hdr;
    const char *val;
    const char *envclause;
    const char *subs;

    action = ap_getword_conf(cmd->temp_pool, &args);
    if (cmd->info == &hdr_out_onsuccess) {
        if (!strcasecmp(action, "always")) {
            cmd->info = &hdr_out_always;
            action = ap_getword_conf(cmd->temp_pool, &args);
        }
        else if (!strcasecmp(action, "onsuccess")) {
            action = ap_getword_conf(cmd->temp_pool, &args);
        }
    }
    hdr = ap_getword_conf(cmd->pool, &args);
    val = *args ? ap_getword_conf(cmd->pool, &args) : NULL;
    subs = *args ? ap_getword_conf(cmd->pool, &args) : NULL;
    envclause = *args ? ap_getword_conf(cmd->pool, &args) : NULL;

    if (*args) {
        return apr_pstrcat(cmd->pool, cmd->cmd->name,
                           " has too many arguments", NULL);
    }

    return header_inout_cmd(cmd, indirconf, action, hdr, val, subs, envclause);
}