static void
plugin_do_close_channels (McpDispatchOperation *obj,
    gboolean wait_for_observers)
{
  McdPluginDispatchOperation *self = MCD_PLUGIN_DISPATCH_OPERATION (obj);

  DEBUG ("%p (wait=%c)", self, wait_for_observers ? 'T' : 'F');

  g_return_if_fail (self != NULL);

  if (wait_for_observers)
    {
      if (self->after_observers < PLUGIN_ACTION_CLOSE)
        {
          DEBUG ("Remembering for later");
          self->after_observers = PLUGIN_ACTION_CLOSE;
        }
    }
  else
    {
      DEBUG ("Closing now");
      _mcd_dispatch_operation_close_channels (self->real_cdo);
    }
}