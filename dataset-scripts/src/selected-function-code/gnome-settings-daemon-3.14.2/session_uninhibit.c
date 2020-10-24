static void
session_uninhibit (GsdUpdatesManager *manager)
{
        GError *error = NULL;
        GVariant *retval = NULL;

        /* state invalid somehow */
        if (manager->priv->inhibit_cookie == 0) {
                g_warning ("not locked");
                goto out;
        }
        retval = g_dbus_proxy_call_sync (G_DBUS_PROXY (manager->priv->proxy_session),
                                         "Uninhibit",
                                         g_variant_new ("(u)",
                                                        manager->priv->inhibit_cookie),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         -1,
                                         manager->priv->cancellable,
                                         &error);
        if (retval == NULL) {
                g_warning ("failed to uninhibit gnome-session: %s",
                           error->message);
                g_error_free (error);
                goto out;
        }
out:
        manager->priv->inhibit_cookie = 0;
        if (retval != NULL)
                g_variant_unref (retval);
}