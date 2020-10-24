static void put_alrm(Alarm **list, const char *entry,
        char *time, char *date, char *ison, char *mesg, Bool cal) {
    Alarm *lst = *list;
    Bool   ok  = True;

    if (! lst) {
        lst   = xmalloc(sizeof(Alarm));
        *list = lst;
    } else {
        if (strcmp(entry, lst->entry) == 0) ok = False;
        while ( (lst->next) && ok) {
            lst = lst->next;
            if (strcmp(entry, lst->entry) == 0) ok = False;
        }
        if (! ok) return;
        lst->next = xmalloc(sizeof(Alarm));
        lst = lst->next;
    }
    lst->entry      = xstrdup(entry);
    lst->alarm_time = time ? xstrdup(time) : NULL;
    lst->alarm_date = date ? xstrdup(date) : NULL;
    lst->on         = ison ? getbool(ison) : True;
    lst->message    = mesg ? xstrdup(mesg) : NULL;
    lst->cal        = cal;
    lst->next       = NULL;
}