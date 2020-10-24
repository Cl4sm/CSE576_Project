  vector min, max;
  object * cur;

  if (*rootlist == NULL)  /* don't bound non-existant objects */
    return;

  gmin->x =  FHUGE;   gmin->y =  FHUGE;   gmin->z =  FHUGE;
  gmax->x = -FHUGE;   gmax->y = -FHUGE;   gmax->z = -FHUGE;

  cur=*rootlist;
  while (cur != NULL)  {  /* Go! */
    min.x = -FHUGE; min.y = -FHUGE; min.z = -FHUGE;
    max.x =  FHUGE; max.y =  FHUGE; max.z =  FHUGE;

    if (cur->methods->bbox((void *) cur, &min, &max)) {
      gmin->x = MYMIN( gmin->x , min.x);
      gmin->y = MYMIN( gmin->y , min.y);
      gmin->z = MYMIN( gmin->z , min.z);

      gmax->x = MYMAX( gmax->x , max.x);
      gmax->y = MYMAX( gmax->y , max.y);
      gmax->z = MYMAX( gmax->z , max.z);
    }

    cur=cur->nextobj;
  }
}
