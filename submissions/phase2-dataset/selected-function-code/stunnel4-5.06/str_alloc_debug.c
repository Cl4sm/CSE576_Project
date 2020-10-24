    ALLOC_TLS *alloc_tls;
    ALLOC_LIST *alloc_list;

    if(!str_initialized)
        fatal_debug("str not initialized", file, line);
    alloc_tls=get_alloc_tls();
    if(!alloc_tls) { /* first allocation in this thread */
        alloc_tls=calloc(1, sizeof(ALLOC_TLS));
        if(!alloc_tls)
            fatal_debug("Out of memory", file, line);
        alloc_tls->head=NULL;
        alloc_tls->bytes=alloc_tls->blocks=0;
        set_alloc_tls(alloc_tls);
    }
    alloc_list=calloc(1, sizeof(ALLOC_LIST)+size+sizeof canary);
    if(!alloc_list)
        fatal_debug("Out of memory", file, line);

    alloc_list->prev=NULL;
    alloc_list->next=alloc_tls->head;
    alloc_list->tls=alloc_tls;
    alloc_list->size=size;
    alloc_list->file=file;
    alloc_list->line=line;
    alloc_list->valid_canary=canary_initialized; /* before memcpy */
    memcpy((u8 *)(alloc_list+1)+size, canary, sizeof canary);
    alloc_list->magic=0xdeadbeef;

    if(alloc_tls->head)
        alloc_tls->head->prev=alloc_list;
    alloc_tls->head=alloc_list;
    alloc_tls->bytes+=size;
    alloc_tls->blocks++;

    return alloc_list+1;
}
