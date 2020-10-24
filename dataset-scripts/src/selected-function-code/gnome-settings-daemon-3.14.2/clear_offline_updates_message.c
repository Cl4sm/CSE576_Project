static void
clear_offline_updates_message (void)
{
        gboolean ret;
        GError *error = NULL;
        gchar *argv[3];
        GPid pid;

        argv[0] = "pkexec";
        argv[1] = LIBEXECDIR "/pk-clear-offline-update";
        argv[2] = NULL;
        ret = g_spawn_async (NULL,
                             argv,
                             NULL,
                             G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH,
                             NULL,
                             NULL,
                             &pid,
                             &error);
        if (!ret) {
                g_warning ("Failure clearing offline update message: %s",
                           error->message);
                g_error_free (error);
                return;
        }
        g_child_watch_add (pid, child_exit_cb, NULL);
}