static void
handle_rotate_windows (GsdXrandrManager *mgr,
                       GnomeRRRotation rotation,
                       gint64 timestamp)
{
        GsdXrandrManagerPrivate *priv = mgr->priv;
        GnomeRRScreen *screen = priv->rw_screen;
        GnomeRRConfig *current;
        GnomeRROutputInfo *rotatable_output_info;
        int num_allowed_rotations;
        GnomeRRRotation allowed_rotations;
        GnomeRRRotation next_rotation;

        g_debug ("Handling XF86RotateWindows with rotation %d", rotation);

        /* Which output? */

        current = gnome_rr_config_new_current (screen, NULL);

        rotatable_output_info = get_laptop_output_info (screen, current);
        if (rotatable_output_info == NULL) {
                g_debug ("No laptop outputs found to rotate; XF86RotateWindows key will do nothing");
                goto out;
        }

        if (rotation <= GNOME_RR_ROTATION_NEXT) {
                /* Which rotation? */

                get_allowed_rotations_for_output (current, priv->rw_screen, rotatable_output_info, &num_allowed_rotations, &allowed_rotations);
                next_rotation = get_next_rotation (allowed_rotations, gnome_rr_output_info_get_rotation (rotatable_output_info));

                if (next_rotation == gnome_rr_output_info_get_rotation (rotatable_output_info)) {
                        g_debug ("No rotations are supported other than the current one; XF86RotateWindows key will do nothing");
                        goto out;
                }
        } else {
                next_rotation = rotation;
        }

        /* Rotate */

        gnome_rr_output_info_set_rotation (rotatable_output_info, next_rotation);

        apply_configuration (mgr, current, timestamp);

out:
        g_object_unref (current);
}