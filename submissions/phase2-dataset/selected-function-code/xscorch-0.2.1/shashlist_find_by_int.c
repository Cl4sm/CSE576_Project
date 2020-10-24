/* shashlist_find_by_int
   Find a shashlist item by integer key. */

   int hashindex;
   shashlist_item *item;

   if(list == NULL || key < 0)
      return(NULL);

   /* Locate the item using hash lookup. */
   hashindex = shash(list->hashbits, key);
   item = list->hash[hashindex];

   /* Scan the hash chain, as needed. */
   while(item != NULL && item->key != key)
      item = item->chain;

   /* item will be either the right item or NULL, at this point. */
   return(item);

}
