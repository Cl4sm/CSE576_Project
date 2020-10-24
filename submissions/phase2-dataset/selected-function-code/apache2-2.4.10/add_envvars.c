static const char *add_envvars(cmd_parms *cmd, const char *args, sei_entry *new)
{
    const char *feature;
    int beenhere = 0;
    char *var;

    for ( ; ; ) {
        feature = ap_getword_conf(cmd->pool, &args);
        if (!*feature) {
            break;
        }
        beenhere++;

        var = ap_getword(cmd->pool, &feature, '=');
        if (*feature) {
            apr_table_setn(new->features, var, feature);
        }
        else if (*var == '!') {
            apr_table_setn(new->features, var + 1, "!");
        }
        else {
            apr_table_setn(new->features, var, "1");
        }
    }

    if (!beenhere) {
        return apr_pstrcat(cmd->pool, "Missing envariable expression for ",
                           cmd->cmd->name, NULL);
    }

    return NULL;
}