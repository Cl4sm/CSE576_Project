gboolean
gsd_sound_manager_start (GsdSoundManager *manager,
                         GError **error)
{
        guint i;
        const gchar * const * dirs;
        char *p;

        g_debug ("Starting sound manager");
        gnome_settings_profile_start (NULL);

        /* We listen for change of the selected theme ... */
        register_config_callback (manager);

        /* ... and we listen to changes of the theme base directories
         * in $HOME ...*/
        p = g_build_filename (g_get_user_data_dir (), "sounds", NULL);
        if (g_mkdir_with_parents(p, 0700) == 0)
                register_directory_callback (manager, p, NULL);
        g_free (p);

        /* ... and globally. */
        dirs = g_get_system_data_dirs ();
        for (i = 0; dirs[i] != NULL; i++) {
                p = g_build_filename (dirs[i], "sounds", NULL);
                if (g_file_test (p, G_FILE_TEST_IS_DIR))
                        register_directory_callback (manager, p, NULL);
                g_free (p);
        }

        gnome_settings_profile_end (NULL);

        return TRUE;
}