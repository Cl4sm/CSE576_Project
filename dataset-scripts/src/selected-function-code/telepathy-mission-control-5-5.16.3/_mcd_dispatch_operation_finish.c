static void
_mcd_dispatch_operation_finish (McdDispatchOperation *operation,
                                GQuark domain, gint code,
                                const gchar *format, ...)
{
    McdDispatchOperationPrivate *priv = operation->priv;
    Approval *approval;
    const gchar *successful_handler = NULL;
    va_list ap;

    if (priv->successful_handler != NULL)
    {
        successful_handler = tp_proxy_get_bus_name (priv->successful_handler);
    }

    if (priv->result != NULL)
    {
        DEBUG ("already finished (or about to): %s", priv->result->message);
        return;
    }

    va_start (ap, format);
    priv->result = g_error_new_valist (domain, code, format, ap);
    va_end (ap);
    DEBUG ("Result: %s", priv->result->message);

    for (approval = g_queue_pop_head (priv->approvals);
         approval != NULL;
         approval = g_queue_pop_head (priv->approvals))
    {
        switch (approval->type)
        {
            case APPROVAL_TYPE_CLAIM:
                /* someone else got it - either another Claim() or a handler */
                g_assert (approval->context != NULL);
                DEBUG ("denying Claim call from %s",
                       dbus_g_method_get_sender (approval->context));
                dbus_g_method_return_error (approval->context, priv->result);
                approval->context = NULL;
                break;

            case APPROVAL_TYPE_HANDLE_WITH:
                g_assert (approval->context != NULL);

                if (successful_handler != NULL)
                {
                    /* Some Handler got it. If this Approver would have been
                     * happy with that Handler, then it succeeds, otherwise,
                     * it loses. */
                    if (approval->client_bus_name == NULL ||
                        !tp_strdiff (approval->client_bus_name,
                                     successful_handler))
                    {
                        DEBUG ("successful HandleWith, channel went to %s",
                               successful_handler);

                        /* HandleWith and HandleWithTime both return void, so
                         * it's OK to not distinguish */
                        tp_svc_channel_dispatch_operation_return_from_handle_with (
                            approval->context);
                        approval->context = NULL;
                    }
                    else
                    {
                        DEBUG ("HandleWith -> NotYours: wanted %s but "
                               "%s got it instead", approval->client_bus_name,
                               successful_handler);
                        dbus_g_method_return_error (approval->context,
                                                    priv->result);
                        approval->context = NULL;
                    }
                }
                else
                {
                    /* Handling finished for some other reason: perhaps the
                     * channel was claimed, or perhaps it closed or we were
                     * told to forget about it.
                     */
                    DEBUG ("HandleWith -> error: %s %d: %s",
                           g_quark_to_string (priv->result->domain),
                           priv->result->code, priv->result->message);
                    dbus_g_method_return_error (approval->context, priv->result);
                    approval->context = NULL;
                }

                break;

            default:
                {   /* there shouldn't be a dbus context for these: */
                    g_assert (approval->context == NULL);
                }
        }

        approval_free (approval);
    }

    if (mcd_dispatch_operation_may_signal_finished (operation))
    {
        DEBUG ("%s/%p has finished", priv->unique_name, operation);
        mcd_dispatch_operation_actually_finish (operation);
    }
    else
    {
        DEBUG ("%s/%p not finishing just yet: "
               "waiting for %" G_GSIZE_FORMAT " observers, "
               "%" G_GSIZE_FORMAT " approvers",
               priv->unique_name, operation,
               priv->observers_pending, priv->ado_pending);
    }
}