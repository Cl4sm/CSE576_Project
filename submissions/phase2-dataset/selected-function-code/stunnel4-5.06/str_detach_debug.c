    ALLOC_LIST *alloc_list;

    if(!ptr) /* do not attempt to free null pointers */
        return;
    alloc_list=get_alloc_list_ptr(ptr, file, line);
    if(alloc_list->tls) { /* not detached */
        /* remove from linked list */
        if(alloc_list->tls->head==alloc_list)
            alloc_list->tls->head=alloc_list->next;
        if(alloc_list->next)
            alloc_list->next->prev=alloc_list->prev;
        if(alloc_list->prev)
            alloc_list->prev->next=alloc_list->next;
        /* update statistics */
        alloc_list->tls->bytes-=alloc_list->size;
        alloc_list->tls->blocks--;
        /* clear pointers */
        alloc_list->next=NULL;
        alloc_list->prev=NULL;
        alloc_list->tls=NULL;
    }
}
