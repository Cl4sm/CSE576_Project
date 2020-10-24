static const char *proxylist_read(cmd_parms *cmd, void *cfg,
                                  const char *filename)
{
    char lbuf[MAX_STRING_LEN];
    char *arg;
    const char *args;
    const char *errmsg;
    ap_configfile_t *cfp;
    apr_status_t rv;

    filename = ap_server_root_relative(cmd->temp_pool, filename);
    rv = ap_pcfg_openfile(&cfp, cmd->temp_pool, filename);
    if (rv != APR_SUCCESS) {
        return apr_psprintf(cmd->pool, "%s: Could not open file %s: %pm",
                            cmd->cmd->name, filename, &rv);
    }

    while (!(ap_cfg_getline(lbuf, MAX_STRING_LEN, cfp))) {
        args = lbuf;
        while (*(arg = ap_getword_conf(cmd->temp_pool, &args)) != '\0') {
            if (*arg == '#') {
                break;
            }
            errmsg = proxies_set(cmd, cfg, arg);
            if (errmsg) {
                ap_cfg_closefile(cfp);
                errmsg = apr_psprintf(cmd->pool, "%s at line %d of %s",
                                      errmsg, cfp->line_number, filename);
                return errmsg;
            }
        }
    }

    ap_cfg_closefile(cfp);
    return NULL;
}