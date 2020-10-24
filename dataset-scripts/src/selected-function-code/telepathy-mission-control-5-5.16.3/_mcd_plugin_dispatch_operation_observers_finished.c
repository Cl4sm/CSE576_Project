void
_mcd_plugin_dispatch_operation_observers_finished (
    McdPluginDispatchOperation *self)
{
  DEBUG ("%p", self);

  switch (self->after_observers)
    {
    case PLUGIN_ACTION_DESTROY:
      DEBUG ("destroying now");
      _mcd_dispatch_operation_destroy_channels (self->real_cdo);
      break;

    case PLUGIN_ACTION_LEAVE:
      DEBUG ("leaving now: %d %s", self->reason, self->message);
      _mcd_dispatch_operation_leave_channels (self->real_cdo,
          self->reason, self->message);
      break;

    case PLUGIN_ACTION_CLOSE:
      DEBUG ("closing now");
      _mcd_dispatch_operation_close_channels (self->real_cdo);
      break;

    case PLUGIN_ACTION_NONE:
      /* nothing to do */
      break;
    }
}