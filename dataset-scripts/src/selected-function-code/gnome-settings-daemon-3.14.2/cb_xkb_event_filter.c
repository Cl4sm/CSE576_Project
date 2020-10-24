static GdkFilterReturn
cb_xkb_event_filter (GdkXEvent              *xevent,
                     GdkEvent               *ignored1,
                     GsdA11yKeyboardManager *manager)
{
        XEvent   *xev   = (XEvent *) xevent;
        XkbEvent *xkbEv = (XkbEvent *) xevent;

        /* 'event_type' is set to zero on notifying us of updates in
         * response to client requests (including our own) and non-zero
         * to notify us of key/mouse events causing changes (like
         * pressing shift 5 times to enable sticky keys).
         *
         * We only want to update GSettings when it's in response to an
         * explicit user input event, so require a non-zero event_type.
         */
        if (xev->xany.type == (manager->priv->xkbEventBase + XkbEventCode) &&
            xkbEv->any.xkb_type == XkbControlsNotify &&
            xkbEv->ctrls.event_type != 0) {
                g_debug ("XKB state changed");
                set_gsettings_from_server (manager);
        } else if (xev->xany.type == (manager->priv->xkbEventBase + XkbEventCode) &&
                   xkbEv->any.xkb_type == XkbAccessXNotify) {
                if (xkbEv->accessx.detail == XkbAXN_AXKWarning) {
                        g_debug ("About to turn on an AccessX feature from the keyboard!");
                        /*
                         * TODO: when XkbAXN_AXKWarnings start working, we need to
                         * invoke ax_keys_warning_dialog_run here instead of in
                         * set_gsettings_from_server().
                         */
                }
        }

        return GDK_FILTER_CONTINUE;
}