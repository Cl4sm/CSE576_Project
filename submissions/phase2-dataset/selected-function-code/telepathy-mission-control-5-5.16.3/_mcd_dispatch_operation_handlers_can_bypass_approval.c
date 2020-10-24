static gboolean
_mcd_dispatch_operation_handlers_can_bypass_approval (
    McdDispatchOperation *self)
{
    gchar **iter;

    /* special case: internally handled request, not subject to approval */
    if (_mcd_dispatch_operation_is_internal (self))
        return TRUE;

    /* special case: we don't have any handlers at all, so we don't want
     * approval - we're just going to fail */
    if (self->priv->possible_handlers == NULL)
        return TRUE;

    for (iter = self->priv->possible_handlers;
         *iter != NULL;
         iter++)
    {
        McdClientProxy *handler = _mcd_client_registry_lookup (
            self->priv->client_registry, *iter);

        /* If the best handler that still exists bypasses approval, then
         * we're going to bypass approval.
         *
         * Also, because handlers are sorted with the best ones first, and
         * handlers with BypassApproval are "better", we can be sure that if
         * we've found a handler that still exists and does not bypass
         * approval, no handler bypasses approval. */
        if (handler != NULL)
        {
            gboolean bypass = _mcd_client_proxy_get_bypass_approval (
                handler);

            DEBUG ("%s has BypassApproval=%c", *iter, bypass ? 'T' : 'F');
            return bypass;
        }
    }

    /* If no handler still exists, we don't bypass approval, although if that
     * happens we're basically doomed anyway. (unless this is an internal
     * request, in which case we should be ok) */
    return FALSE;
}