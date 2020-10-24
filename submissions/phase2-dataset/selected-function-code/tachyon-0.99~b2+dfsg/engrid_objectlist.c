static int engrid_objectlist(grid * g, objectlist ** list) {
  objectlist * cur, * next, **prev;
  int numsucceeded = 0; 

  if (*list == NULL) 
    return 0;
  
  prev = list; 
  cur = *list;

  while (cur != NULL) {
    next = cur->next;

    if (engrid_object(g, cur->obj, 0)) {
      *prev = next;
      free(cur);
      numsucceeded++;
    } else {
      prev = &cur->next;
    }

    cur = next;
  } 

  return numsucceeded;
}
