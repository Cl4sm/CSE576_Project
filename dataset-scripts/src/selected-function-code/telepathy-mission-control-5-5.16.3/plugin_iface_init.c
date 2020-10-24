static void
plugin_iface_init (McpDispatchOperationIface *iface,
    gpointer unused G_GNUC_UNUSED)
{
  DEBUG ("called");

  iface->get_account_path = plugin_do_get_account_path;
  iface->get_connection_path = plugin_do_get_connection_path;
  iface->get_protocol = plugin_do_get_protocol;
  iface->get_cm_name = plugin_do_get_cm_name;

  iface->get_n_channels = plugin_do_get_n_channels;
  iface->get_nth_channel_path = plugin_do_get_nth_channel_path;
  iface->ref_nth_channel_properties = plugin_do_ref_nth_channel_properties;

  iface->start_delay = plugin_do_start_delay;
  iface->end_delay = plugin_do_end_delay;

  iface->leave_channels = plugin_do_leave_channels;
  iface->close_channels = plugin_do_close_channels;
  iface->destroy_channels = plugin_do_destroy_channels;
}