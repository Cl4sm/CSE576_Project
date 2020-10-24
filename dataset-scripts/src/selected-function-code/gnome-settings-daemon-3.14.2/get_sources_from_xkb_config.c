static void
get_sources_from_xkb_config (GsdKeyboardManager *manager)
{
        GsdKeyboardManagerPrivate *priv = manager->priv;
        GVariantBuilder builder;
        GVariant *v;
        gint i, n;
        gchar **layouts = NULL;
        gchar **variants = NULL;
        gboolean have_default_layout = FALSE;

        v = g_dbus_proxy_get_cached_property (priv->localed, "X11Layout");
        if (v) {
                const gchar *s = g_variant_get_string (v, NULL);
                if (*s)
                        layouts = g_strsplit (s, ",", -1);
                g_variant_unref (v);
        }

        if (!layouts)
                return;

        v = g_dbus_proxy_get_cached_property (priv->localed, "X11Variant");
        if (v) {
                const gchar *s = g_variant_get_string (v, NULL);
                if (*s)
                        variants = g_strsplit (s, ",", -1);
                g_variant_unref (v);
        }

        if (variants && variants[0])
                n = MIN (g_strv_length (layouts), g_strv_length (variants));
        else
                n = g_strv_length (layouts);

        init_builder_with_sources (&builder, priv->input_sources_settings);

        for (i = 0; i < n && layouts[i][0]; ++i) {
                gchar *id;

                if (variants && variants[i] && variants[i][0])
                        id = g_strdup_printf ("%s+%s", layouts[i], variants[i]);
                else
                        id = g_strdup (layouts[i]);

                if (g_str_equal (id, DEFAULT_LAYOUT))
                        have_default_layout = TRUE;

                g_variant_builder_add (&builder, "(ss)", INPUT_SOURCE_TYPE_XKB, id);
                g_free (id);
        }

        if (!have_default_layout)
                g_variant_builder_add (&builder, "(ss)", INPUT_SOURCE_TYPE_XKB, DEFAULT_LAYOUT);

        g_settings_set_value (priv->input_sources_settings, KEY_INPUT_SOURCES, g_variant_builder_end (&builder));

        g_strfreev (layouts);
        g_strfreev (variants);
}