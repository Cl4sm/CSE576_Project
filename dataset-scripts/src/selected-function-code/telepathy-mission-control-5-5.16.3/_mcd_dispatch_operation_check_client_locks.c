static void
_mcd_dispatch_operation_check_client_locks (McdDispatchOperation *self)
{
    Approval *approval;
    guint approver_event_id = 0;

    if (!self->priv->invoked_observers_if_needed)
    {
        DEBUG ("waiting for Observers to be called");
        return;
    }

    if (self->priv->plugins_pending > 0)
    {
        DEBUG ("waiting for plugins to stop delaying");
        return;
    }

    /* Check whether plugins' requests to close channels later should be
     * honoured. We want to do this before running Approvers (if any). */
    if (self->priv->observers_pending == 0 &&
        !self->priv->did_post_observer_actions)
    {
        _mcd_plugin_dispatch_operation_observers_finished (
            self->priv->plugin_api);
        self->priv->did_post_observer_actions = TRUE;
    }

    /* If nobody is bypassing approval, then we want to run approvers as soon
     * as possible, without waiting for observers, to improve responsiveness.
     * (The regression test dispatcher/exploding-bundles.py asserts that we
     * do this.)
     *
     * However, if a handler bypasses approval, we must wait til the observers
     * return, then run that handler, then proceed with the other handlers. */
    if (!self->priv->tried_handlers_before_approval &&
        !_mcd_dispatch_operation_handlers_can_bypass_approval (self)
        && self->priv->delay_approver_observers_pending == 0
        && self->priv->channel != NULL &&
        ! _mcd_plugin_dispatch_operation_will_terminate (
            self->priv->plugin_api))
    {
        self->priv->tried_handlers_before_approval = TRUE;

        approver_event_id = g_idle_add_full (G_PRIORITY_HIGH,
                         mcd_dispatch_operation_idle_run_approvers,
                         g_object_ref (self), g_object_unref);
    }

    /* we may not continue until we've called all the Observers, and they've
     * all replied "I'm ready" */
    if (self->priv->observers_pending > 0)
    {
        return;
    }

    /* if we've called the first Approver, we may not continue until we've
     * called them all, and they all replied "I'm ready" */
    if (self->priv->ado_pending > 0)
    {
        DEBUG ("waiting for AddDispatchOperation to return");
        return;
    }

    /* if we've called one Handler, we may not continue until it responds
     * with an error */
    if (self->priv->trying_handler != NULL)
    {
        DEBUG ("waiting for handler_is_suitable or HandleChannels to return");
        return;
    }

    /* if a handler has claimed or accepted the channel, we have nothing to
     * do */
    if (self->priv->result != NULL)
    {
        DEBUG ("already finished (or finishing): %s",
               self->priv->result->message);
        return;
    }

    /* If we're only meant to be observing, do nothing */
    if (self->priv->observe_only)
    {
        DEBUG ("only observing");
        return;
    }

    if (_mcd_dispatch_operation_is_internal (self))
    {
        DEBUG ("Invoking internal handlers for requests");
        if (self->priv->channel != NULL)
        {
            McdChannel *channel = self->priv->channel;
            McdRequest *request = _mcd_channel_get_request (channel);

            if (request != NULL)
            {
                DEBUG ("Internal handler for request channel");
                _mcd_handler_map_set_channel_handled_internally (
                    self->priv->handler_map,
                    mcd_channel_get_tp_channel (channel),
                    _mcd_dispatch_operation_get_account_path (self));
                _mcd_request_handle_internally (request, channel, TRUE);
            }
        }

        /* The rest of this function deals with externally handled requests: *
         * Since these requests were internal, we need not trouble ourselves *
         * further (and infact would probably trigger errors if we tried)    */
        return;
    }

    /* If there are no potential handlers, the story ends here: we don't
     * want to run approvers in this case */
    if (self->priv->possible_handlers == NULL)
    {
        GError incapable = { TP_ERROR, TP_ERROR_NOT_CAPABLE,
            "No possible handlers, giving up" };

        DEBUG ("%s", incapable.message);
        _mcd_dispatch_operation_close_as_undispatchable (self, &incapable);
        return;
    }

    approval = g_queue_peek_head (self->priv->approvals);

    /* if we've been claimed, respond, then do not call HandleChannels */
    if (approval != NULL && approval->type == APPROVAL_TYPE_CLAIM)
    {
        /* this needs to be copied because we don't use it til after we've
         * freed approval->context */
        gchar *caller = g_strdup (dbus_g_method_get_sender (
            approval->context));

        /* remove this approval from the list, so it won't be treated as a
         * failure */
        g_queue_pop_head (self->priv->approvals);

        if (self->priv->channel != NULL)
        {
            McdChannel *channel = self->priv->channel;

            mcd_dispatch_operation_set_channel_handled_by (self, channel,
                caller, NULL);
        }

        DEBUG ("Replying to Claim call from %s", caller);

        tp_svc_channel_dispatch_operation_return_from_claim (
            approval->context);
        approval->context = NULL;

        _mcd_dispatch_operation_finish (self, TP_ERROR, TP_ERROR_NOT_YOURS,
                                        "Channel successfully claimed by %s",
                                        caller);
        g_free (caller);

        if (approver_event_id > 0)
        {
            DEBUG ("Cancelling call to approvers as dispatch operation has been Claimed");
            g_source_remove (approver_event_id);
        }

        approval_free (approval);
        return;
    }
    else if (approval != NULL && approval->type == APPROVAL_TYPE_HANDLE_WITH)
    {
        /* We set this to TRUE so that the handlers are called. */
        self->priv->invoked_approvers_if_needed = TRUE;

        if (approver_event_id > 0)
        {
            DEBUG ("Cancelling call to approvers as dispatch operation has been HandledWith'd");
            g_source_remove (approver_event_id);
        }
    }

    if (self->priv->invoked_approvers_if_needed)
    {
        if (_mcd_dispatch_operation_is_approved (self))
        {
            DEBUG ("trying next handler");

            if (!_mcd_dispatch_operation_try_next_handler (self))
            {
                GError incapable = { TP_ERROR, TP_ERROR_NOT_CAPABLE,
                    "No possible handler still exists, giving up" };

                DEBUG ("ran out of handlers");
                _mcd_dispatch_operation_close_as_undispatchable (self,
                                                                 &incapable);
            }
        }
        else
        {
            DEBUG ("waiting for approval");
        }
    }
    else if (!self->priv->tried_handlers_before_approval)
    {
        DEBUG ("trying next pre-approval handler");

        if (!_mcd_dispatch_operation_try_next_handler (self))
        {
            DEBUG ("ran out of pre-approval handlers");

            self->priv->tried_handlers_before_approval = TRUE;

            g_idle_add_full (G_PRIORITY_HIGH,
                             mcd_dispatch_operation_idle_run_approvers,
                             g_object_ref (self), g_object_unref);
        }
    }
}