static void
maybe_create_initial_settings (GsdKeyboardManager *manager)
{
        GSettings *settings;
        GVariant *sources;
        gchar **options;

        settings = manager->priv->input_sources_settings;

        if (g_getenv ("RUNNING_UNDER_GDM")) {
                GVariantBuilder builder;
                /* clean the settings and get them from the "system" */
                g_variant_builder_init (&builder, G_VARIANT_TYPE ("a(ss)"));
                g_settings_set_value (settings, KEY_INPUT_SOURCES, g_variant_builder_end (&builder));
                get_sources_from_xkb_config (manager);

                g_settings_set_strv (settings, KEY_KEYBOARD_OPTIONS, NULL);
                get_options_from_xkb_config (manager);
                return;
        }

        maybe_convert_old_settings (settings);

        /* if we still don't have anything do some educated guesses */
        sources = g_settings_get_value (settings, KEY_INPUT_SOURCES);
        if (g_variant_n_children (sources) < 1)
                get_sources_from_xkb_config (manager);
        g_variant_unref (sources);

        options = g_settings_get_strv (settings, KEY_KEYBOARD_OPTIONS);
        if (g_strv_length (options) < 1)
                get_options_from_xkb_config (manager);
        g_strfreev (options);
}