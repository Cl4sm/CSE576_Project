static gboolean
ldsm_purge_trash_and_temp (gpointer data)
{
        GDateTime *now, *old;

        now = g_date_time_new_now_local ();
        old = g_date_time_add_days (now, - purge_after);

        if (purge_trash) {
                g_debug ("housekeeping: purge trash older than %u days", purge_after);
                gsd_ldsm_purge_trash (old);
        }
        if (purge_temp_files) {
                g_debug ("housekeeping: purge temp files older than %u days", purge_after);
                gsd_ldsm_purge_temp_files (old);
        }

        g_date_time_unref (old);
        g_date_time_unref (now);

        return G_SOURCE_CONTINUE;
}