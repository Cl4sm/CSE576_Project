
    char Command[512];


    if( HasExecute == 0) return; /* no command specified.  Ignore clicks. */
    DblClkDelay = 0;
    if ((xev->button == Button1) && (xev->type == ButtonPress)){
        if (GotFirstClick1) GotDoubleClick1 = 1;
        else GotFirstClick1 = 1;
    } else if ((xev->button == Button2) && (xev->type == ButtonPress)){
        if (GotFirstClick2) GotDoubleClick2 = 1;
        else GotFirstClick2 = 1;
    } else if ((xev->button == Button3) && (xev->type == ButtonPress)){
        if (GotFirstClick3) GotDoubleClick3 = 1;
        else GotFirstClick3 = 1;
    }


    /*
     *  We got a double click on Mouse Button1 (i.e. the left one)
     */
    if (GotDoubleClick1) {
        GotFirstClick1 = 0;
        GotDoubleClick1 = 0;
        sprintf(Command, "%s &", ExecuteCommand);
        system(Command);
    }


    /*
     *  We got a double click on Mouse Button2 (i.e. the left one)
     */
    if (GotDoubleClick2) {
        GotFirstClick2 = 0;
        GotDoubleClick2 = 0;
    }


    /*
     *  We got a double click on Mouse Button3 (i.e. the left one)
     */
    if (GotDoubleClick3) {
        GotFirstClick3 = 0;
        GotDoubleClick3 = 0;
    }



   return;

}
