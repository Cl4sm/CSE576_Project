static gboolean not_size_allocated_yet(GtkWidget *w)
{
    /*
     * This function tests whether a widget has not yet taken up space
     * on the screen which it will occupy in future. (Therefore, it
     * returns true only if the widget does exist but does not have a
     * size allocation. A null widget is already taking up all the
     * space it ever will.)
     */
    if (!w)
        return FALSE;        /* nonexistent widgets aren't a problem */

#if GTK_CHECK_VERSION(2,18,0)  /* skip if no gtk_widget_get_allocation */
    {
        GtkAllocation a;
        gtk_widget_get_allocation(w, &a);
        if (a.height == 0 || a.width == 0)
            return TRUE;       /* widget exists but has no size yet */
    }
#endif

    return FALSE;
}