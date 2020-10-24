static void
plugin_do_leave_channels (McpDispatchOperation *obj,
    gboolean wait_for_observers, TpChannelGroupChangeReason reason,
    const gchar *message)
{
  McdPluginDispatchOperation *self = MCD_PLUGIN_DISPATCH_OPERATION (obj);

  DEBUG ("%p (wait=%c reason=%d message=%s)", self,
      wait_for_observers ? 'T' : 'F', reason, message);

  g_return_if_fail (self != NULL);

  if (wait_for_observers)
    {
      if (self->after_observers < PLUGIN_ACTION_LEAVE)
        {
          DEBUG ("Remembering for later");
          self->after_observers = PLUGIN_ACTION_LEAVE;
          self->reason = reason;
          g_free (self->message);
          self->message = g_strdup (message);
        }
    }
  else
    {
      DEBUG ("Leaving now");
      _mcd_dispatch_operation_leave_channels (self->real_cdo, reason, message);
    }
}