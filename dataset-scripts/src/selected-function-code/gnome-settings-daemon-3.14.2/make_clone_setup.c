static GnomeRRConfig *
make_clone_setup (GsdXrandrManager *manager, GnomeRRScreen *screen)
{
        GnomeRRConfig *result;
        GnomeRROutputInfo **outputs;
        int width, height;
        int i;

        if (!get_clone_size (screen, &width, &height))
                return NULL;

        result = gnome_rr_config_new_current (screen, NULL);
        gnome_rr_config_set_clone (result, TRUE);

        outputs = gnome_rr_config_get_outputs (result);

        for (i = 0; outputs[i] != NULL; ++i) {
                GnomeRROutputInfo *info = outputs[i];

                gnome_rr_output_info_set_active (info, FALSE);
                if (!is_laptop_with_closed_lid (manager, screen, info) && gnome_rr_output_info_is_connected (info)) {
                        GnomeRROutput *output =
                                gnome_rr_screen_get_output_by_name (screen, gnome_rr_output_info_get_name (info));
                        GnomeRRMode **modes = gnome_rr_output_list_modes (output);
                        int j;
                        int best_rate = 0;

                        for (j = 0; modes[j] != NULL; ++j) {
                                GnomeRRMode *mode = modes[j];
                                int w, h;

                                w = gnome_rr_mode_get_width (mode);
                                h = gnome_rr_mode_get_height (mode);

                                if (w == width && h == height) {
                                        int r = gnome_rr_mode_get_freq (mode);
                                        if (r > best_rate)
                                                best_rate = r;
                                }
                        }

                        if (best_rate > 0) {
                                gnome_rr_output_info_set_active (info, TRUE);
                                gnome_rr_output_info_set_rotation (info, GNOME_RR_ROTATION_0);
                                gnome_rr_output_info_set_refresh_rate (info, best_rate);
                                gnome_rr_output_info_set_geometry (info, 0, 0, width, height);
                        }
                }
        }

        if (config_is_all_off (result)) {
                g_object_unref (G_OBJECT (result));
                result = NULL;
        }

        print_configuration (result, "clone setup");

        return result;
}