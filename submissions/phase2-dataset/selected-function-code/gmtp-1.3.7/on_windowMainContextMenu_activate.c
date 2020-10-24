    GtkMenu *menu;
    GdkEventButton *event_button;
    g_return_val_if_fail(widget != NULL, FALSE);
    g_return_val_if_fail(GTK_IS_MENU(widget), FALSE);
    g_return_val_if_fail(event != NULL, FALSE);

    /* The "widget" is the menu that was supplied when
     * g_signal_connect_swapped() was called.
     */
    menu = GTK_MENU(widget);
    if (event->type == GDK_BUTTON_PRESS) {
        event_button = (GdkEventButton *) event;
        if (event_button->button == 3) {
            gtk_menu_popup(menu, NULL, NULL, NULL, NULL,
                    event_button->button, event_button->time);
            return TRUE;
        }
    }
    return FALSE;
} // end on_windowMainContextMenu_activate()
