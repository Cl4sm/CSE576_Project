static void
get_properties_cb (GObject *object,
                   GAsyncResult *res,
                   GsdUpdatesRefresh *refresh)
{
        PkNetworkEnum state;
        GError *error = NULL;
        PkControl *control = PK_CONTROL(object);
        gboolean ret;

        /* get the result */
        ret = pk_control_get_properties_finish (control, res, &error);
        if (!ret) {
                /* TRANSLATORS: backend is broken, and won't tell us what it supports */
                g_warning ("could not get properties");
                g_error_free (error);
                goto out;
        }

        /* get values */
        g_object_get (control,
                      "network-state", &state,
                      NULL);
        refresh->priv->network_active = convert_network_state (refresh, state);
out:
        return;
}