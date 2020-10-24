static void
signal_cb (GDBusProxy *proxy,
    gchar *sender_name,
    gchar *signal_name,
    GVariant *parameters,
    gpointer user_data)
{
  McdSlacker *self = user_data;
  GVariant *prop;

  if (tp_strdiff (signal_name, SERVICE_SIG_NAME))
    return;

  if (!g_variant_is_of_type (parameters, G_VARIANT_TYPE ("(u)")))
    {
      WARNING ("%s.%s arguments are of type %s and we expected (u)",
          SERVICE_INTERFACE, SERVICE_PROP_NAME,
          g_variant_get_type_string (parameters));
      return;
    }

  prop = g_variant_get_child_value (parameters, 0);
  status_changed (self, prop);
  g_variant_unref (prop);
}