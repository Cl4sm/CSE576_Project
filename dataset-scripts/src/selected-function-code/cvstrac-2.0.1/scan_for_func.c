static void scan_for_func(char *zLine){
  int i,j,k; 
  if( nUsed<=nFixed ) return;
  for(i=0; isspace(zLine[i]); i++){}
  if( zLine[i]==0 ) return;
  if( strncmp(&zLine[i],"void",4)!=0 ){
    if( zLine[i]!='*' ) goto page_skip;
    return;
  }
  i += 4;
  if( !isspace(zLine[i]) ) goto page_skip;
  while( isspace(zLine[i]) ){ i++; }
  for(j=0; isalnum(zLine[i+j]) || zLine[i+j]=='_'; j++){}
  if( j==0 ) goto page_skip;
  for(k=nFixed; k<nUsed; k++){
    aEntry[k].zFunc = string_dup(&zLine[i], j);
  }
  i+=j;
  while( isspace(zLine[i]) ){ i++; }
  if( zLine[i]!='(' ) goto page_skip;
  nFixed = nUsed;
  return;

page_skip:   
   for(i=nFixed; i<nUsed; i++){
      fprintf(stderr,"%s:%d: skipping page \"%s\"\n",
         zFile, nLine, aEntry[i].zPath);
   }
   nUsed = nFixed;
}