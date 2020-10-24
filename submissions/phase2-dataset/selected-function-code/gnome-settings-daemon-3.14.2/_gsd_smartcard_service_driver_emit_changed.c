static gboolean
_gsd_smartcard_service_driver_emit_changed (gpointer user_data)
{
  GsdSmartcardServiceDriverSkeleton *skeleton = GSD_SMARTCARD_SERVICE_DRIVER_SKELETON (user_data);
  GList *l;
  GVariantBuilder builder;
  GVariantBuilder invalidated_builder;
  guint num_changes;

  g_mutex_lock (&skeleton->priv->lock);
  g_variant_builder_init (&builder, G_VARIANT_TYPE ("a{sv}"));
  g_variant_builder_init (&invalidated_builder, G_VARIANT_TYPE ("as"));
  for (l = skeleton->priv->changed_properties, num_changes = 0; l != NULL; l = l->next)
    {
      ChangedProperty *cp = l->data;
      GVariant *variant;
      const GValue *cur_value;

      cur_value = &skeleton->priv->properties[cp->prop_id - 1];
      if (!_g_value_equal (cur_value, &cp->orig_value))
        {
          variant = g_dbus_gvalue_to_gvariant (cur_value, G_VARIANT_TYPE (cp->info->parent_struct.signature));
          g_variant_builder_add (&builder, "{sv}", cp->info->parent_struct.name, variant);
          g_variant_unref (variant);
          num_changes++;
        }
    }
  if (num_changes > 0)
    {
      GList *connections, *ll;
      GVariant *signal_variant;
      signal_variant = g_variant_ref_sink (g_variant_new ("(sa{sv}as)", "org.gnome.SettingsDaemon.Smartcard.Driver",
                                           &builder, &invalidated_builder));
      connections = g_dbus_interface_skeleton_get_connections (G_DBUS_INTERFACE_SKELETON (skeleton));
      for (ll = connections; ll != NULL; ll = ll->next)
        {
          GDBusConnection *connection = ll->data;

          g_dbus_connection_emit_signal (connection,
                                         NULL, g_dbus_interface_skeleton_get_object_path (G_DBUS_INTERFACE_SKELETON (skeleton)),
                                         "org.freedesktop.DBus.Properties",
                                         "PropertiesChanged",
                                         signal_variant,
                                         NULL);
        }
      g_variant_unref (signal_variant);
      g_list_free_full (connections, g_object_unref);
    }
  else
    {
      g_variant_builder_clear (&builder);
      g_variant_builder_clear (&invalidated_builder);
    }
  g_list_free_full (skeleton->priv->changed_properties, (GDestroyNotify) _changed_property_free);
  skeleton->priv->changed_properties = NULL;
  skeleton->priv->changed_properties_idle_source = NULL;
  g_mutex_unlock (&skeleton->priv->lock);
  return FALSE;
}