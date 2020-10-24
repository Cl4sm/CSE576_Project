static void
gnome_session_shutdown (GsdMediaKeysManager *manager)
{
	GError *error = NULL;
	GVariant *variant;
        GDBusProxy *proxy;

        proxy = G_DBUS_PROXY (gnome_settings_bus_get_session_proxy ());
	variant = g_dbus_proxy_call_sync (proxy,
					  "Shutdown",
					  NULL,
					  G_DBUS_CALL_FLAGS_NONE,
					  -1,
					  NULL,
					  &error);
	if (variant == NULL) {
		g_warning ("Failed to call Shutdown on session manager: %s", error->message);
		g_error_free (error);
		return;
	}
	g_variant_unref (variant);
        g_object_unref (proxy);
}