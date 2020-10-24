static void
get_options_from_xkb_config (GsdKeyboardManager *manager)
{
        GsdKeyboardManagerPrivate *priv = manager->priv;
        GVariant *v;
        gchar **options = NULL;

        v = g_dbus_proxy_get_cached_property (priv->localed, "X11Options");
        if (v) {
                const gchar *s = g_variant_get_string (v, NULL);
                if (*s)
                        options = g_strsplit (s, ",", -1);
                g_variant_unref (v);
        }

        if (!options)
                return;

        g_settings_set_strv (priv->input_sources_settings, KEY_KEYBOARD_OPTIONS, (const gchar * const*) options);

        g_strfreev (options);
}