void
_mcd_account_maybe_autoconnect (McdAccount *account)
{
    McdAccountPrivate *priv;

    g_return_if_fail (MCD_IS_ACCOUNT (account));
    priv = account->priv;

    if (!mcd_account_would_like_to_connect (account))
    {
        return;
    }

    if (_mcd_account_needs_dispatch (account))
    {
        DEBUG ("Always-dispatchable account %s needs no transport",
            priv->unique_name);
    }
    else if (mcd_connectivity_monitor_is_online (priv->connectivity))
    {
        DEBUG ("Account %s has connectivity, connecting",
            priv->unique_name);
    }
    else
    {
        DEBUG ("Account %s needs connectivity, not connecting",
            priv->unique_name);
    }

    DEBUG ("connecting account %s", priv->unique_name);
    _mcd_account_connect_with_auto_presence (account, FALSE);
}