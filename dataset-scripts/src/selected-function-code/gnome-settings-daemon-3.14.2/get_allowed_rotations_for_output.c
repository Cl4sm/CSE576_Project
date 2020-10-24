static void
get_allowed_rotations_for_output (GnomeRRConfig *config,
                                  GnomeRRScreen *rr_screen,
                                  GnomeRROutputInfo *output,
                                  int *out_num_rotations,
                                  GnomeRRRotation *out_rotations)
{
        GnomeRRRotation current_rotation;
        int i;

        *out_num_rotations = 0;
        *out_rotations = 0;

        current_rotation = gnome_rr_output_info_get_rotation (output);

        /* Yay for brute force */

        for (i = 0; i < G_N_ELEMENTS (possible_rotations); i++) {
                GnomeRRRotation rotation_to_test;

                rotation_to_test = possible_rotations[i];

                gnome_rr_output_info_set_rotation (output, rotation_to_test);

                if (gnome_rr_config_applicable (config, rr_screen, NULL)) { /* NULL-GError */
                        (*out_num_rotations)++;
                        (*out_rotations) |= rotation_to_test;
                }
        }

        gnome_rr_output_info_set_rotation (output, current_rotation);

        if (*out_num_rotations == 0 || *out_rotations == 0) {
                g_warning ("Huh, output %p says it doesn't support any rotations, and yet it has a current rotation?", output);
                *out_num_rotations = 1;
                *out_rotations = gnome_rr_output_info_get_rotation (output);
        }
}