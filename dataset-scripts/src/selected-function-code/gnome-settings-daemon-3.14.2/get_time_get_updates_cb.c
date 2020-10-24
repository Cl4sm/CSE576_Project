static void
get_time_get_updates_cb (GObject *object, GAsyncResult *res, GsdUpdatesRefresh *refresh)
{
        PkControl *control = PK_CONTROL (object);
        GError *error = NULL;
        guint seconds;
        guint thresh;

        /* get the result */
        seconds = pk_control_get_time_since_action_finish (control, res, &error);
        if (seconds == 0) {
                g_warning ("failed to get time: %s", error->message);
                g_error_free (error);
                return;
        }

        /* have we passed the timout? */
        thresh = g_settings_get_int (refresh->priv->settings,
                                     GSD_SETTINGS_FREQUENCY_GET_UPDATES);
        if (seconds < thresh) {
                g_debug ("not before timeout, thresh=%u, now=%u", thresh, seconds);
                return;
        }

        /* send signal */
        g_debug ("emitting get-updates");
        g_signal_emit (refresh, signals [GET_UPDATES], 0);
}