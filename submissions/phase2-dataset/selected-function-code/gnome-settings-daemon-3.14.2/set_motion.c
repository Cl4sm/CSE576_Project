static void
set_motion (GsdMouseManager *manager,
            GdkDevice       *device)
{
        XDevice *xdevice;
        XPtrFeedbackControl feedback;
        XFeedbackState *states, *state;
        int num_feedbacks;
        int numerator, denominator;
        gfloat motion_acceleration;
        int motion_threshold;
        GSettings *settings;
        guint i;

        xdevice = open_gdk_device (device);
        if (xdevice == NULL)
                return;

	g_debug ("setting motion on %s", gdk_device_get_name (device));

        if (device_is_touchpad (xdevice))
                settings = manager->priv->touchpad_settings;
        else
                settings = manager->priv->mouse_settings;

        /* Calculate acceleration */
        motion_acceleration = g_settings_get_double (settings, KEY_MOTION_ACCELERATION);

        if (motion_acceleration >= 1.0) {
                /* we want to get the acceleration, with a resolution of 0.5
                 */
                if ((motion_acceleration - floor (motion_acceleration)) < 0.25) {
                        numerator = floor (motion_acceleration);
                        denominator = 1;
                } else if ((motion_acceleration - floor (motion_acceleration)) < 0.5) {
                        numerator = ceil (2.0 * motion_acceleration);
                        denominator = 2;
                } else if ((motion_acceleration - floor (motion_acceleration)) < 0.75) {
                        numerator = floor (2.0 *motion_acceleration);
                        denominator = 2;
                } else {
                        numerator = ceil (motion_acceleration);
                        denominator = 1;
                }
        } else if (motion_acceleration < 1.0 && motion_acceleration > 0) {
                /* This we do to 1/10ths */
                numerator = floor (motion_acceleration * 10) + 1;
                denominator= 10;
        } else {
                numerator = -1;
                denominator = -1;
        }

        /* And threshold */
        motion_threshold = g_settings_get_int (settings, KEY_MOTION_THRESHOLD);

        gdk_error_trap_push ();

        /* Get the list of feedbacks for the device */
        states = XGetFeedbackControl (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice, &num_feedbacks);
        if (states == NULL)
                goto out;
        state = (XFeedbackState *) states;
        for (i = 0; i < num_feedbacks; i++) {
                if (state->class == PtrFeedbackClass) {
                        /* And tell the device */
                        feedback.class      = PtrFeedbackClass;
                        feedback.length     = sizeof (XPtrFeedbackControl);
                        feedback.id         = state->id;
                        feedback.threshold  = motion_threshold;
                        feedback.accelNum   = numerator;
                        feedback.accelDenom = denominator;

                        g_debug ("Setting accel %d/%d, threshold %d for device '%s'",
                                 numerator, denominator, motion_threshold, gdk_device_get_name (device));

                        XChangeFeedbackControl (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()),
                                                xdevice,
                                                DvAccelNum | DvAccelDenom | DvThreshold,
                                                (XFeedbackControl *) &feedback);

                        break;
                }
                state = (XFeedbackState *) ((char *) state + state->length);
        }

        if (gdk_error_trap_pop ())
                g_warning ("Error setting acceleration on \"%s\"", gdk_device_get_name (device));

        XFreeFeedbackList (states);

    out:

        xdevice_close (xdevice);
}