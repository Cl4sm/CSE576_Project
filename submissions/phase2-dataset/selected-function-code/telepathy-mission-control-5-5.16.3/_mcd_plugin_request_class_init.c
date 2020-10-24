static void
_mcd_plugin_request_class_init (
    McdPluginRequestClass *cls)
{
  GObjectClass *object_class = (GObjectClass *) cls;

  object_class->set_property = plugin_req_set_property;
  object_class->dispose = plugin_req_dispose;

  g_object_class_install_property (object_class, PROP_REAL_REQUEST,
      g_param_spec_object ("real-request", "Real channel request",
          "The underlying McdRequest",
          MCD_TYPE_REQUEST,
          G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_ACCOUNT,
      g_param_spec_object ("account", "Account",
          "The underlying McdAccount",
          MCD_TYPE_ACCOUNT,
          G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS));
}