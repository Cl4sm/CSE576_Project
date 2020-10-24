static void
session_inhibit (GsdUpdatesManager *manager)
{
        const gchar *reason;
        GError *error = NULL;
        GVariant *retval = NULL;

        /* state invalid somehow */
        if (manager->priv->inhibit_cookie != 0) {
                g_warning ("already locked");
                goto out;
        }

        /* TRANSLATORS: the reason why we've inhibited it */
        reason = _("A transaction that cannot be interrupted is running");
        retval = g_dbus_proxy_call_sync (G_DBUS_PROXY (manager->priv->proxy_session),
                                         "Inhibit",
                                         g_variant_new ("(susu)",
                                                        "gnome-settings-daemon", /* app-id */
                                                        0, /* xid */
                                                        reason, /* reason */
                                                        4 /* flags */),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         -1,
                                         manager->priv->cancellable,
                                         &error);
        if (retval == NULL) {
                g_warning ("failed to inhibit gnome-session: %s",
                           error->message);
                g_error_free (error);
                goto out;
        }

        /* get cookie */
        g_variant_get (retval, "(u)",
                       &manager->priv->inhibit_cookie);
out:
        if (retval != NULL)
                g_variant_unref (retval);
}