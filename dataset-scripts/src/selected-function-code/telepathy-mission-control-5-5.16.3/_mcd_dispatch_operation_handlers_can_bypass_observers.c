static gboolean
_mcd_dispatch_operation_handlers_can_bypass_observers (
    McdDispatchOperation *self)
{
    gchar **iter;

    for (iter = self->priv->possible_handlers;
         iter != NULL && *iter != NULL;
         iter++)
    {
        McdClientProxy *handler = _mcd_client_registry_lookup (
            self->priv->client_registry, *iter);

        if (handler != NULL)
        {
            gboolean bypass = _mcd_client_proxy_get_bypass_observers (
                handler);

            DEBUG ("%s has BypassObservers=%c", *iter, bypass ? 'T' : 'F');
            return bypass;
        }
    }

    return FALSE;
}