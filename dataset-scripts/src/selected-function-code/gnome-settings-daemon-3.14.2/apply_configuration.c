static gboolean
apply_configuration (GsdXrandrManager *manager, GnomeRRConfig *config, gint64 timestamp)
{
        GsdXrandrManagerPrivate *priv = manager->priv;
        GError *error;
        gboolean success;

        gnome_rr_config_ensure_primary (config);

        print_configuration (config, "Applying Configuration");

        error = NULL;
        success = gnome_rr_config_apply (config, priv->rw_screen, &error);

        if (!success) {
                log_msg ("Could not switch to the following configuration (timestamp %" G_GINT64_FORMAT "): %s\n", timestamp, error->message);
                log_configuration (config);
                g_error_free (error);
        }

        return success;
}