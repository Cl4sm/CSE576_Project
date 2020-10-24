MenuAC(Widget w, XEvent *event, String *params, Cardinal *num)
{
#if DEBUG
    fprintf(stderr, "MenuAC: (%d,%d)\n",atoi(params[0]),atoi(params[1]));
#endif
    if (*num < 2)
        GameCB(w, 4);
    switch(params[0][0]) {
    case '0':
        GameCB(w, atoi(params[1]));
        break;
    case '1':
        SuppCB(w, atoi(params[1]));
        break;
    case '2':
        ChangeGameCB(w, atoi(params[1]), NULL);
        break;
    }
}
