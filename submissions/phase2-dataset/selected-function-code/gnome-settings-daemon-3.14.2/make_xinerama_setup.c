static GnomeRRConfig *
make_xinerama_setup (GsdXrandrManager *manager, GnomeRRScreen *screen)
{
        /* Turn on everything that has a preferred mode, and
         * position it from left to right
         */
        GnomeRRConfig *result = gnome_rr_config_new_current (screen, NULL);
        GnomeRROutputInfo **outputs = gnome_rr_config_get_outputs (result);
        int i;
        int x;

        gnome_rr_config_set_clone (result, FALSE);

        x = 0;
        for (i = 0; outputs[i] != NULL; ++i) {
                GnomeRROutputInfo *info = outputs[i];

                if (is_laptop (screen, info)) {
                        if (laptop_lid_is_closed (manager) && follow_laptop_lid (manager))
                                gnome_rr_output_info_set_active (info, FALSE);
                        else {
                                gnome_rr_output_info_set_primary (info, TRUE);
                                x = turn_on_and_get_rightmost_offset (screen, info, x);
                        }
                }
        }

        for (i = 0; outputs[i] != NULL; ++i) {
                GnomeRROutputInfo *info = outputs[i];

                if (gnome_rr_output_info_is_connected (info) && !is_laptop (screen, info)) {
                        gnome_rr_output_info_set_primary (info, FALSE);
                        x = turn_on_and_get_rightmost_offset (screen, info, x);
                }
        }

        if (!trim_rightmost_outputs_that_dont_fit_in_framebuffer (screen, result)) {
                g_object_unref (G_OBJECT (result));
                result = NULL;
        }

        print_configuration (result, "xinerama setup");

        return result;
}