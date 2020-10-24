int list_delete_entry( List *list, List_Entry *entry )
{
    /* delete possible? */
    if ( entry == 0 ) return 0;
    if ( list->count == 0 ) return 0;
    if ( entry == &list->head || entry == &list->tail ) return 0;
    /* adjust anchor and counter */
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    list->count--;
    /* check current_entry */
    if ( list->cur_entry == entry )
        list->cur_entry = list->cur_entry->prev;
    /* free memory */
    if ( list->auto_delete ) {
        if ( list->callback )
            (list->callback)( entry->item );
        else
            free( entry->item );
    }
    free( entry );
    return 1;
}