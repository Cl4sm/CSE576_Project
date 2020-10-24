void
gsd_a11y_keyboard_manager_stop (GsdA11yKeyboardManager *manager)
{
        GsdA11yKeyboardManagerPrivate *p = manager->priv;

        g_debug ("Stopping a11y_keyboard manager");

        if (p->desc != NULL) {
                XkbDescRec *desc;

                desc = get_xkb_desc_rec (manager);
                if (desc != NULL) {
                        if (p->desc->ctrls->enabled_ctrls != desc->ctrls->enabled_ctrls) {
                                gdk_error_trap_push ();
                                XkbSetControls (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()),
                                                DEFAULT_XKB_SET_CONTROLS_MASK,
                                                p->desc);

                                XSync (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), FALSE);
                                gdk_error_trap_pop_ignored ();
                        }
                        XkbFreeKeyboard (desc, XkbAllComponentsMask, True);
                }
                XkbFreeKeyboard (p->desc, XkbAllComponentsMask, True);
                p->desc = NULL;
        }

        if (p->start_idle_id != 0) {
                g_source_remove (p->start_idle_id);
                p->start_idle_id = 0;
        }

        if (p->device_manager != NULL) {
                g_signal_handler_disconnect (p->device_manager, p->device_added_id);
                p->device_manager = NULL;
        }

        if (p->settings != NULL) {
                g_signal_handlers_disconnect_by_func (p->settings, keyboard_callback, manager);
                g_object_unref (p->settings);
                p->settings = NULL;
        }

        gdk_window_remove_filter (NULL,
                                  (GdkFilterFunc) cb_xkb_event_filter,
                                  manager);

        p->slowkeys_shortcut_val = FALSE;
        p->stickykeys_shortcut_val = FALSE;
}