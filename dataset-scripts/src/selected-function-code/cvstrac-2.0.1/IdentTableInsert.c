static int IdentTableInsert(
  IdentTable *pTable,       /* The table into which we will insert */
  const char *zId,          /* Name of the identifiers */
  int nId                   /* Length of the identifier name */
){
  int h;
  Ident *pId;

  if( nId<=0 ){
    nId = strlen(zId);
  }
  h = Hash(zId,nId) % IDENT_HASH_SIZE;
  for(pId = pTable->apTable[h]; pId; pId=pId->pCollide){
    if( strncmp(zId,pId->zName,nId)==0 && pId->zName[nId]==0 ){
      /* printf("Already in table: %.*s\n",nId,zId); */
      return 0;
    }
  }
  pId = SafeMalloc( sizeof(Ident) + nId + 1 );
  pId->zName = (char*)&pId[1];
  sprintf(pId->zName,"%.*s",nId,zId);
  pId->pNext = pTable->pList;
  pTable->pList = pId;
  pId->pCollide = pTable->apTable[h];
  pTable->apTable[h] = pId;
  /* printf("Add to table: %.*s\n",nId,zId); */
  return 1;
}