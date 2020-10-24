static gint key_event(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    frontend *fe = (frontend *)data;
    int keyval;
    int shift = (event->state & GDK_SHIFT_MASK) ? MOD_SHFT : 0;
    int ctrl = (event->state & GDK_CONTROL_MASK) ? MOD_CTRL : 0;

    if (!backing_store_ok(fe))
        return TRUE;

#if !GTK_CHECK_VERSION(2,0,0)
    /* Gtk 1.2 passes a key event to this function even if it's also
     * defined as an accelerator.
     * Gtk 2 doesn't do this, and this function appears not to exist there. */
    if (fe->accelgroup &&
        gtk_accel_group_get_entry(fe->accelgroup,
        event->keyval, event->state))
        return TRUE;
#endif

    /* Handle mnemonics. */
    if (gtk_window_activate_key(GTK_WINDOW(fe->window), event))
        return TRUE;

    if (event->keyval == GDK_Up)
        keyval = shift | ctrl | CURSOR_UP;
    else if (event->keyval == GDK_KP_Up || event->keyval == GDK_KP_8)
	keyval = MOD_NUM_KEYPAD | '8';
    else if (event->keyval == GDK_Down)
        keyval = shift | ctrl | CURSOR_DOWN;
    else if (event->keyval == GDK_KP_Down || event->keyval == GDK_KP_2)
	keyval = MOD_NUM_KEYPAD | '2';
    else if (event->keyval == GDK_Left)
        keyval = shift | ctrl | CURSOR_LEFT;
    else if (event->keyval == GDK_KP_Left || event->keyval == GDK_KP_4)
	keyval = MOD_NUM_KEYPAD | '4';
    else if (event->keyval == GDK_Right)
        keyval = shift | ctrl | CURSOR_RIGHT;
    else if (event->keyval == GDK_KP_Right || event->keyval == GDK_KP_6)
	keyval = MOD_NUM_KEYPAD | '6';
    else if (event->keyval == GDK_KP_Home || event->keyval == GDK_KP_7)
        keyval = MOD_NUM_KEYPAD | '7';
    else if (event->keyval == GDK_KP_End || event->keyval == GDK_KP_1)
        keyval = MOD_NUM_KEYPAD | '1';
    else if (event->keyval == GDK_KP_Page_Up || event->keyval == GDK_KP_9)
        keyval = MOD_NUM_KEYPAD | '9';
    else if (event->keyval == GDK_KP_Page_Down || event->keyval == GDK_KP_3)
        keyval = MOD_NUM_KEYPAD | '3';
    else if (event->keyval == GDK_KP_Insert || event->keyval == GDK_KP_0)
        keyval = MOD_NUM_KEYPAD | '0';
    else if (event->keyval == GDK_KP_Begin || event->keyval == GDK_KP_5)
        keyval = MOD_NUM_KEYPAD | '5';
    else if (event->keyval == GDK_BackSpace ||
	     event->keyval == GDK_Delete ||
	     event->keyval == GDK_KP_Delete)
        keyval = '\177';
    else if (event->string[0] && !event->string[1])
        keyval = (unsigned char)event->string[0];
    else
        keyval = -1;

    if (keyval >= 0 &&
        !midend_process_key(fe->me, 0, 0, keyval))
	gtk_widget_destroy(fe->window);

    return TRUE;
}