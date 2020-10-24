McdDispatchOperation *
_mcd_dispatch_operation_new (McdClientRegistry *client_registry,
                             McdHandlerMap *handler_map,
                             gboolean needs_approval,
                             gboolean observe_only,
                             McdChannel *channel,
                             const gchar * const *possible_handlers)
{
    gpointer *obj;

    /* If we're only observing, then the channel was requested "behind MC's
     * back", so they can't need approval (i.e. observe_only implies
     * !needs_approval) */
    g_return_val_if_fail (!observe_only || !needs_approval, NULL);
    g_return_val_if_fail (MCD_IS_CHANNEL (channel), NULL);

    obj = g_object_new (MCD_TYPE_DISPATCH_OPERATION,
                        "client-registry", client_registry,
                        "handler-map", handler_map,
                        "channel", channel,
                        "possible-handlers", possible_handlers,
                        "needs-approval", needs_approval,
                        "observe-only", observe_only,
                        NULL);

    return MCD_DISPATCH_OPERATION (obj);
}