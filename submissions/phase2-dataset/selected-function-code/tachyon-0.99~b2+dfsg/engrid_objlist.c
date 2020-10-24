  object * cur, * next, **prev;
  int numsucceeded = 0;

  if (*list == NULL) 
    return 0;
  
  prev = list; 
  cur = *list;

  while (cur != NULL) {
    next = cur->nextobj;

    if (engrid_object(g, cur, 1)) {
      *prev = next;
      numsucceeded++;
    } else {
      prev = (object **) &cur->nextobj;
    }

    cur = next;
  } 

  return numsucceeded;
}
