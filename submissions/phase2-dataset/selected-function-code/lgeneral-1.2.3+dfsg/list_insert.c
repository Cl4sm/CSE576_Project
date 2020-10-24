int list_insert( List *list, void *item, int pos )
{
    int         i;
    List_Entry    *cur = &list->head;
    List_Entry    *new_entry = 0;

    /* check if insertion possible */
    if ( pos < 0 || pos > list->count ) return 0;
    if ( item == 0 ) return 0;
    /* get to previous entry */
    for (i = 0; i < pos; i++) cur = cur->next;
    /* create and anchor new entry */
    new_entry = calloc( 1, sizeof( List_Entry ) );
    new_entry->item = item;
    new_entry->next = cur->next;
    new_entry->prev = cur;
    cur->next->prev = new_entry;
    cur->next = new_entry;
    list->count++;
    return 1;
}