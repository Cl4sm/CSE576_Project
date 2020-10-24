static void InstallDecl(Decl *pDecl){
  int h;
  Decl *pOther;

  h = Hash(pDecl->zName,0) % DECL_HASH_SIZE;
  pOther = apTable[h];
  while( pOther && strcmp(pDecl->zName,pOther->zName)!=0 ){
    pOther = pOther->pSameHash;
  }
  if( pOther ){
    pDecl->pSameName = pOther->pSameName;
    pOther->pSameName = pDecl;
  }else{
    pDecl->pSameName = 0;
    pDecl->pSameHash = apTable[h];
    apTable[h] = pDecl;
  }
  pDecl->pNext = 0;
  if( pDeclFirst==0 ){
    pDeclFirst = pDeclLast = pDecl;
  }else{
    pDeclLast->pNext = pDecl;
    pDeclLast = pDecl;
  }
}