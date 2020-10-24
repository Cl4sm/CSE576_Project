void
gsd_housekeeping_manager_stop (GsdHousekeepingManager *manager)
{
        GsdHousekeepingManagerPrivate *p = manager->priv;

        g_debug ("Stopping housekeeping manager");

        g_clear_object (&p->bus_cancellable);
        g_clear_pointer (&p->introspection_data, g_dbus_node_info_unref);
        g_clear_object (&p->connection);

        if (p->short_term_cb) {
                g_source_remove (p->short_term_cb);
                p->short_term_cb = 0;
        }

        if (p->long_term_cb) {
                g_source_remove (p->long_term_cb);
                p->long_term_cb = 0;

                /* Do a clean-up on shutdown if and only if the size or age
                   limits have been set to paranoid levels (zero) */
                if ((g_settings_get_int (p->settings, THUMB_AGE_KEY) == 0) ||
                    (g_settings_get_int (p->settings, THUMB_SIZE_KEY) == 0)) {
                        do_cleanup (manager);
                }

        }

        g_clear_object (&p->settings);
        gsd_ldsm_clean ();
}