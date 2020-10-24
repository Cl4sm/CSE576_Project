static void
log_output (GnomeRROutputInfo *output)
{
        gchar *name = gnome_rr_output_info_get_name (output);
        gchar *display_name = gnome_rr_output_info_get_display_name (output);

        log_msg ("        %s: ", name ? name : "unknown");

        if (gnome_rr_output_info_is_connected (output)) {
                if (gnome_rr_output_info_is_active (output)) {
                        int x, y, width, height;
                        gnome_rr_output_info_get_geometry (output, &x, &y, &width, &height);
                        log_msg ("%dx%d@%d +%d+%d",
                                 width,
                                 height,
                                 gnome_rr_output_info_get_refresh_rate (output),
                                 x,
                                 y);
                } else
                        log_msg ("off");
        } else
                log_msg ("disconnected");

        if (display_name)
                log_msg (" (%s)", display_name);

        if (gnome_rr_output_info_get_primary (output))
                log_msg (" (primary output)");

        log_msg ("\n");
}