gboolean
gsd_a11y_settings_manager_start (GsdA11ySettingsManager *manager,
                                 GError                **error)
{
        g_debug ("Starting a11y_settings manager");
        gnome_settings_profile_start (NULL);

	manager->priv->interface_settings = g_settings_new ("org.gnome.desktop.interface");
	manager->priv->a11y_apps_settings = g_settings_new ("org.gnome.desktop.a11y.applications");

	g_signal_connect (G_OBJECT (manager->priv->a11y_apps_settings), "changed",
			  G_CALLBACK (apps_settings_changed), manager);

	/* If any of the screen reader or on-screen keyboard are enabled,
	 * make sure a11y is enabled for the toolkits.
	 * We don't do the same thing for the reverse so it's possible to
	 * enable AT-SPI for the toolkits without using an a11y app */
	if (g_settings_get_boolean (manager->priv->a11y_apps_settings, "screen-keyboard-enabled") ||
	    g_settings_get_boolean (manager->priv->a11y_apps_settings, "screen-reader-enabled"))
		g_settings_set_boolean (manager->priv->interface_settings, "toolkit-accessibility", TRUE);

        gnome_settings_profile_end (NULL);
        return TRUE;
}