void _getifdata()
{
    int buffer, fd, val, ret = -1;
    unsigned int len, i;
    char tmpinterfacestring[sizeof(ifdata.if_name)+1],*strstrmatch;
    struct nmparms params;
    mib_ifEntry * if_buf;

    
    /*
     * The interface description is more then the pure devicename.
     * Let's do some formating to allow a propper pattern matching
     */
    strcpy(tmpinterfacestring,ifdata.if_name);
    strcat(tmpinterfacestring," ");

    for (i=0; i <= ifdata.if_amount; i++)
    {
        if ((fd = open_mib("/dev/lan", O_RDWR, i, 0)) >= 0)
        {
            if ((if_buf = (mib_ifEntry *) malloc (sizeof(mib_ifEntry))) != 0) {
                params.objid  = ID_ifEntry;
                params.buffer = if_buf;
                len = sizeof(mib_ifEntry);
                params.len    = &len;
                if_buf->ifIndex = i+1;
                if ((ret = get_mib_info(fd, &params)) == 0) {
                    /*
                     * The interface given by the user must start at the
                     * beginning of if_buf->ifDescr. If that's the case,
                     * strstrmatch is equal to if_buf->ifDescr. If not,
                     * strstrmatch might be a subset of if_buf->ifDescr,
                     * or NULL
                     */
                    strstrmatch = strstr(if_buf->ifDescr, (char *)tmpinterfacestring);
                    if ( strstrmatch && (strcmp(strstrmatch,if_buf->ifDescr)== 0))
                    {
                        ifdata.if_valid = 1;
                        ifdata.if_id = i+1;
                        break;
                    }
                }
            }
        }
        free(if_buf);
        close_mib(fd);
    }
    return;
}
