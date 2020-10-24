static void
mcd_account_request_presence_int (McdAccount *account,
                                  TpConnectionPresenceType type,
                                  const gchar *status,
                                  const gchar *message,
                                  gboolean user_initiated)
{
    McdAccountPrivate *priv = account->priv;
    gboolean changed = FALSE;

    if (priv->req_presence_type != type)
    {
        priv->req_presence_type = type;
        changed = TRUE;
    }

    if (tp_strdiff (priv->req_presence_status, status))
    {
        g_free (priv->req_presence_status);
        priv->req_presence_status = g_strdup (status);
        changed = TRUE;
    }

    if (tp_strdiff (priv->req_presence_message, message))
    {
        g_free (priv->req_presence_message);
        priv->req_presence_message = g_strdup (message);
        changed = TRUE;
    }

    if (changed)
    {
        GValue value = G_VALUE_INIT;

        g_value_init (&value, TP_STRUCT_TYPE_SIMPLE_PRESENCE);
        g_value_take_boxed (&value,
                            tp_value_array_build (3,
                                G_TYPE_UINT, type,
                                G_TYPE_STRING, status,
                                G_TYPE_STRING, message,
                                G_TYPE_INVALID));
        mcd_account_changed_property (account, "RequestedPresence", &value);
        g_value_unset (&value);
    }

    DEBUG ("Requested presence: %u %s %s",
        priv->req_presence_type,
        priv->req_presence_status,
        priv->req_presence_message);

    if (type >= TP_CONNECTION_PRESENCE_TYPE_AVAILABLE)
    {
        if (!priv->enabled)
        {
            DEBUG ("%s not Enabled", priv->unique_name);
            return;
        }

        if (!mcd_account_is_valid (account))
        {
            DEBUG ("%s not Valid", priv->unique_name);
            return;
        }
    }

    if (priv->connection == NULL)
    {
        if (type >= TP_CONNECTION_PRESENCE_TYPE_AVAILABLE)
        {
            if (changed)
            {
                _mcd_account_set_changing_presence (account, TRUE);
            }

            _mcd_account_connection_begin (account, user_initiated);
        }
    }
    else
    {
        if (changed)
        {
            _mcd_account_set_changing_presence (account, TRUE);
        }

        _mcd_connection_request_presence (priv->connection,
                                          priv->req_presence_type,
					  priv->req_presence_status,
					  priv->req_presence_message);
    }
}