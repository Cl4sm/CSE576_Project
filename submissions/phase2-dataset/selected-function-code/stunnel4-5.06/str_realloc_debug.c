    ALLOC_LIST *prev_alloc_list, *alloc_list;

    if(!ptr)
        return str_alloc_debug(size, file, line);
    prev_alloc_list=get_alloc_list_ptr(ptr, file, line);
    if(prev_alloc_list->size>size) /* shrinking the allocation */
        memset((u8 *)ptr+size, 0, prev_alloc_list->size-size); /* paranoia */
    alloc_list=realloc(prev_alloc_list,
        sizeof(ALLOC_LIST)+size+sizeof canary);
    if(!alloc_list)
        fatal_debug("Out of memory", file, line);
    ptr=alloc_list+1;
    if(size>alloc_list->size) /* growing the allocation */
        memset((u8 *)ptr+alloc_list->size, 0, size-alloc_list->size);
    if(alloc_list->tls) { /* not detached */
        /* refresh possibly invalidated linked list pointers */
        if(alloc_list->tls->head==prev_alloc_list)
            alloc_list->tls->head=alloc_list;
        if(alloc_list->next)
            alloc_list->next->prev=alloc_list;
        if(alloc_list->prev)
            alloc_list->prev->next=alloc_list;
        /* update statistics while the old size is still available */
        alloc_list->tls->bytes+=size-alloc_list->size;
    }
    alloc_list->size=size;
    alloc_list->file=file;
    alloc_list->line=line;
    alloc_list->valid_canary=canary_initialized; /* before memcpy */
    memcpy((u8 *)ptr+size, canary, sizeof canary);
    return ptr;
}
