    ALLOC_TLS *alloc_tls;
    ALLOC_LIST *alloc_list;
    int i=0;

    if(!str_initialized)
        fatal_debug("str not initialized", __FILE__, __LINE__);
    alloc_tls=get_alloc_tls();
    if(!alloc_tls) {
        s_log(LOG_DEBUG, "str_stats: alloc_tls not initialized");
        return;
    }
    if(!alloc_tls->blocks && !alloc_tls->bytes)
        return; /* skip if no data is allocated */
    s_log(LOG_DEBUG, "str_stats: %lu block(s), "
            "%lu data byte(s), %lu control byte(s)",
        (unsigned long int)alloc_tls->blocks,
        (unsigned long int)alloc_tls->bytes,
        (unsigned long int)(alloc_tls->blocks*
            (sizeof(ALLOC_LIST)+sizeof canary)));
    for(alloc_list=alloc_tls->head; alloc_list; alloc_list=alloc_list->next) {
        if(++i>10) /* limit the number of results */
            break;
        s_log(LOG_DEBUG, "str_stats: %lu byte(s) at %s:%d",
            (unsigned long int)alloc_list->size,
            alloc_list->file, alloc_list->line);
    }
}
