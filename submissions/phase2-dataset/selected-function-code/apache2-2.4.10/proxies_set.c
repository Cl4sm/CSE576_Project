static const char *proxies_set(cmd_parms *cmd, void *cfg,
                               const char *arg)
{
    remoteip_config_t *config = ap_get_module_config(cmd->server->module_config,
                                                     &remoteip_module);
    remoteip_proxymatch_t *match;
    apr_status_t rv;
    char *ip = apr_pstrdup(cmd->temp_pool, arg);
    char *s = ap_strchr(ip, '/');
    if (s) {
        *s++ = '\0';
    }

    if (!config->proxymatch_ip) {
        config->proxymatch_ip = apr_array_make(cmd->pool, 1, sizeof(*match));
    }
    match = (remoteip_proxymatch_t *) apr_array_push(config->proxymatch_ip);
    match->internal = cmd->info;

    if (looks_like_ip(ip)) {
        /* Note s may be null, that's fine (explicit host) */
        rv = apr_ipsubnet_create(&match->ip, ip, s, cmd->pool);
    }
    else
    {
        apr_sockaddr_t *temp_sa;

        if (s) {
            return apr_pstrcat(cmd->pool, "RemoteIP: Error parsing IP ", arg,
                               " the subnet /", s, " is invalid for ",
                               cmd->cmd->name, NULL);
        }

        rv = apr_sockaddr_info_get(&temp_sa,  ip, APR_UNSPEC, 0,
                                   APR_IPV4_ADDR_OK, cmd->temp_pool);
        while (rv == APR_SUCCESS)
        {
            apr_sockaddr_ip_get(&ip, temp_sa);
            rv = apr_ipsubnet_create(&match->ip, ip, NULL, cmd->pool);
            if (!(temp_sa = temp_sa->next)) {
                break;
            }
            match = (remoteip_proxymatch_t *)
                    apr_array_push(config->proxymatch_ip);
            match->internal = cmd->info;
        }
    }

    if (rv != APR_SUCCESS) {
        return apr_psprintf(cmd->pool,
                            "RemoteIP: Error parsing IP %s (%pm error) for %s",
                            arg, &rv, cmd->cmd->name);
    }

    return NULL;
}