static gint button_event(GtkWidget *widget, GdkEventButton *event,
                         gpointer data)
{
    frontend *fe = (frontend *)data;
    int button;

    if (!backing_store_ok(fe))
        return TRUE;

    if (event->type != GDK_BUTTON_PRESS && event->type != GDK_BUTTON_RELEASE)
        return TRUE;

    if (event->button == 2 || (event->state & GDK_SHIFT_MASK))
	button = MIDDLE_BUTTON;
    else if (event->button == 3 || (event->state & GDK_MOD1_MASK))
	button = RIGHT_BUTTON;
    else if (event->button == 1)
	button = LEFT_BUTTON;
    else
	return FALSE;		       /* don't even know what button! */

    if (event->type == GDK_BUTTON_RELEASE)
        button += LEFT_RELEASE - LEFT_BUTTON;

    if (!midend_process_key(fe->me, event->x - fe->ox,
                            event->y - fe->oy, button))
	gtk_widget_destroy(fe->window);

    return TRUE;
}