static void
mcd_account_check_parameters (McdAccount *account,
                              CheckParametersCb callback,
                              gpointer user_data)
{
    McdAccountPrivate *priv = account->priv;
    TpProtocol *protocol;
    GList *params = NULL;
    GList *iter;
    GError *error = NULL;

    g_return_if_fail (callback != NULL);

    DEBUG ("called for %s", priv->unique_name);
    protocol = _mcd_manager_dup_protocol (priv->manager, priv->protocol_name);

    if (protocol == NULL)
    {
        g_set_error (&error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
            "CM '%s' doesn't implement protocol '%s'", priv->manager_name,
            priv->protocol_name);
        goto out;
    }

    params = tp_protocol_dup_params (protocol);

    for (iter = params; iter != NULL; iter = iter->next)
    {
        TpConnectionManagerParam *param = iter->data;
        const gchar *param_name = tp_connection_manager_param_get_name (param);

        if (!tp_connection_manager_param_is_required ((param)))
            continue;

        if (!mcd_account_get_parameter (account, param_name, NULL, NULL))
        {
            g_set_error (&error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                "missing required parameter '%s'", param_name);
            goto out;
        }
    }

out:
    if (error != NULL)
    {
        DEBUG ("%s", error->message);
    }

    callback (account, error, user_data);
    g_clear_error (&error);
    g_list_free_full (params,
                      (GDestroyNotify) tp_connection_manager_param_free);
    g_clear_object (&protocol);
}