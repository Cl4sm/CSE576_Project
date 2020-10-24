static gint motion_event(GtkWidget *widget, GdkEventMotion *event,
                         gpointer data)
{
    frontend *fe = (frontend *)data;
    int button;

    if (!backing_store_ok(fe))
        return TRUE;

    if (event->state & (GDK_BUTTON2_MASK | GDK_SHIFT_MASK))
	button = MIDDLE_DRAG;
    else if (event->state & GDK_BUTTON1_MASK)
	button = LEFT_DRAG;
    else if (event->state & GDK_BUTTON3_MASK)
	button = RIGHT_DRAG;
    else
	return FALSE;		       /* don't even know what button! */

    if (!midend_process_key(fe->me, event->x - fe->ox,
                            event->y - fe->oy, button))
	gtk_widget_destroy(fe->window);
#if GTK_CHECK_VERSION(2,12,0)
    gdk_event_request_motions(event);
#else
    gdk_window_get_pointer(widget->window, NULL, NULL, NULL);
#endif

    return TRUE;
}