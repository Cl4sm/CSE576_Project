static GnomeRRConfig *
make_other_setup (GnomeRRScreen *screen)
{
        /* Turn off all laptops, and make all external monitors clone
         * from (0, 0)
         */

        GnomeRRConfig *result = gnome_rr_config_new_current (screen, NULL);
        GnomeRROutputInfo **outputs = gnome_rr_config_get_outputs (result);
        int i;

        gnome_rr_config_set_clone (result, FALSE);

        for (i = 0; outputs[i] != NULL; ++i) {
                GnomeRROutputInfo *info = outputs[i];

                if (is_laptop (screen, info)) {
                        gnome_rr_output_info_set_active (info, FALSE);
                }
                else {
                        if (gnome_rr_output_info_is_connected (info))
                                turn_on (screen, info, 0, 0);
               }
        }

        if (!trim_rightmost_outputs_that_dont_fit_in_framebuffer (screen, result)) {
                g_object_unref (G_OBJECT (result));
                result = NULL;
        }

        print_configuration (result, "other setup");

        return result;
}