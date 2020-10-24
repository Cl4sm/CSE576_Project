static int
set_disable_w_typing (GsdMouseManager *manager, gboolean state)
{
        if (state && touchpad_is_present ()) {
                GError *error = NULL;
                GPtrArray *args;

                if (manager->priv->syndaemon_spawned)
                        return 0;

                if (!have_program_in_path ("syndaemon"))
                        return 0;

                args = g_ptr_array_new ();

                g_ptr_array_add (args, "syndaemon");
                g_ptr_array_add (args, "-i");
                g_ptr_array_add (args, "1.0");
                g_ptr_array_add (args, "-t");
                g_ptr_array_add (args, "-K");
                g_ptr_array_add (args, "-R");
                g_ptr_array_add (args, NULL);

                /* we must use G_SPAWN_DO_NOT_REAP_CHILD to avoid
                 * double-forking, otherwise syndaemon will immediately get
                 * killed again through (PR_SET_PDEATHSIG when the intermediate
                 * process dies */
                g_spawn_async (g_get_home_dir (), (char **) args->pdata, NULL,
                                G_SPAWN_SEARCH_PATH|G_SPAWN_DO_NOT_REAP_CHILD, setup_syndaemon, NULL,
                                &manager->priv->syndaemon_pid, &error);

                manager->priv->syndaemon_spawned = (error == NULL);
                g_ptr_array_free (args, FALSE);

                if (error) {
                        g_warning ("Failed to launch syndaemon: %s", error->message);
                        g_settings_set_boolean (manager->priv->touchpad_settings, KEY_TOUCHPAD_DISABLE_W_TYPING, FALSE);
                        g_error_free (error);
                } else {
                        g_child_watch_add (manager->priv->syndaemon_pid, syndaemon_died, manager);
                        g_debug ("Launched syndaemon");
                }
        } else if (manager->priv->syndaemon_spawned) {
                kill (manager->priv->syndaemon_pid, SIGHUP);
                g_spawn_close_pid (manager->priv->syndaemon_pid);
                manager->priv->syndaemon_spawned = FALSE;
                g_debug ("Killed syndaemon");
        }

        return 0;
}