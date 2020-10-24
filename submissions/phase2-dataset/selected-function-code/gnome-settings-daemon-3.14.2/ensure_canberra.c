static void
ensure_canberra (GsdMediaKeysManager *manager)
{
	char *theme_name;

	if (manager->priv->ca != NULL)
		return;

        ca_context_create (&manager->priv->ca);
        ca_context_set_driver (manager->priv->ca, "pulse");
        ca_context_change_props (manager->priv->ca, 0,
                                 CA_PROP_APPLICATION_ID, "org.gnome.VolumeControl",
                                 NULL);
        manager->priv->gtksettings = gtk_settings_get_for_screen (gdk_screen_get_default ());
        g_object_get (G_OBJECT (manager->priv->gtksettings), "gtk-sound-theme-name", &theme_name, NULL);
        if (theme_name)
                ca_context_change_props (manager->priv->ca, CA_PROP_CANBERRA_XDG_THEME_NAME, theme_name, NULL);
        g_free (theme_name);
        g_signal_connect (manager->priv->gtksettings, "notify::gtk-sound-theme-name",
                          G_CALLBACK (sound_theme_changed), manager);
}