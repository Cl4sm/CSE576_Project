static void
maybe_convert_old_settings (GSettings *settings)
{
        GVariant *sources;
        gchar **options;
        gchar *stamp_dir_path = NULL;
        gchar *stamp_file_path = NULL;
        GError *error = NULL;

        stamp_dir_path = g_build_filename (g_get_user_data_dir (), PACKAGE_NAME, NULL);
        if (g_mkdir_with_parents (stamp_dir_path, 0755)) {
                g_warning ("Failed to create directory %s: %s", stamp_dir_path, g_strerror (errno));
                goto out;
        }

        stamp_file_path = g_build_filename (stamp_dir_path, "input-sources-converted", NULL);
        if (g_file_test (stamp_file_path, G_FILE_TEST_EXISTS))
                goto out;

        sources = g_settings_get_value (settings, KEY_INPUT_SOURCES);
        if (g_variant_n_children (sources) < 1) {
                convert_libgnomekbd_layouts (settings);
        }
        g_variant_unref (sources);

        options = g_settings_get_strv (settings, KEY_KEYBOARD_OPTIONS);
        if (g_strv_length (options) < 1)
                convert_libgnomekbd_options (settings);
        g_strfreev (options);

        if (!g_file_set_contents (stamp_file_path, "", 0, &error)) {
                g_warning ("%s", error->message);
                g_error_free (error);
        }
out:
        g_free (stamp_file_path);
        g_free (stamp_dir_path);
}