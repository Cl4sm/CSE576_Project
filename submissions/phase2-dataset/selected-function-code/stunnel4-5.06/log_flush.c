void log_flush(LOG_MODE new_mode) {
    struct LIST *tmp;

    /* prevent changing LOG_MODE_CONFIGURED to LOG_MODE_ERROR
     * once stderr file descriptor is closed */
    if(mode!=LOG_MODE_CONFIGURED)
        mode=new_mode;

    enter_critical_section(CRIT_LOG);
    while(head) {
        log_raw(head->level, head->stamp, head->id, head->text);
        str_free(head->stamp);
        str_free(head->id);
        str_free(head->text);
        tmp=head;
        head=head->next;
        str_free(tmp);
    }
    leave_critical_section(CRIT_LOG);
    head=tail=NULL;
}
