static Boolean
refreshtimer(XtPointer p)
{
    if (bd->Active()) {
        bd->HourlyPatrol();
        tm->DisplayTimer(bd->GetRest());
    } else {
        tm->DisplayCurrentTime();
    }
    // This is a way not to consume too much CPU time
    // just for such a silly game...
#if HAVE_USLEEP
    usleep(idletime.tv_usec);
#elif HAVE_SELECT
    select(0, NULL, NULL, NULL, &idletime);
#endif

    if (icon_state)
        return True;
    else
        return False;
}
