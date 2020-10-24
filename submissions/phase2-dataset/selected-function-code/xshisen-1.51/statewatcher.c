void
statewatcher(Widget w, caddr_t unused, XEvent *event)
{
    if (event->type == MapNotify) {
        // de-iconified
#if DEBUG
        fprintf(stderr, "Deiconified.\n");
#endif
        workproc_id = XtAppAddWorkProc(app_context, (XtWorkProc)refreshtimer, NULL);
        icon_state = 0;
        if (timeout_id)
            XtRemoveTimeOut(timeout_id);
    }
    else if (event->type == UnmapNotify) {
        // iconified
#if DEBUG
        fprintf(stderr, "Iconified.\n");
#endif
        changeiconwindow();
        icon_state = 1;
    }
}
