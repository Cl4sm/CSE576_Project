int list_delete_pos( List *list, int pos )
{
    int         i;
    List_Entry  *cur = &list->head;

    /* check if deletion possbile */
    if ( list_empty( list ) ) return 0;
    if ( pos < 0 || pos >= list->count ) return 0;
    /* get to correct entry */
    for ( i = 0; i <= pos; i++ ) cur = cur->next;
    /* modify anchors */
    cur->next->prev = cur->prev;
    cur->prev->next = cur->next;
    /* decrease counter */
    list->count--;
    /* check current_entry */
    if ( list->cur_entry == cur )
        list->cur_entry = list->cur_entry->prev;
    /* free memory */
    if ( list->auto_delete ) {
        if ( list->callback )
            (list->callback)( cur->item );
        else
            free( cur->item );
    }
    free( cur );
    return 1;
}