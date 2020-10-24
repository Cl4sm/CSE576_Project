static int isAllowedAttr(const char *zAttr,int nAttr){
  int i,j;
  int inquote = 0;
  int inbody = 0;

  for(i=0; i<nAttr && zAttr[i]!='>'; i++){
    if( !inbody && !inquote && isalpha(zAttr[i]) ){
      for(j=1; i+j<nAttr && isalnum(zAttr[i+j]); j++){}

      if( !inSortedList(&zAttr[i], j, azAllowedAttr,
                        sizeof(azAllowedAttr)/sizeof(azAllowedAttr[0]))){
        return 0;
      }
      i += j-1;
      inbody = 0;
    }else if( inquote && zAttr[i]=='"' ){
      inquote=0;
    }else if( !inquote && zAttr[i]=='"' ){
      inquote=1;
    }else if( isspace(zAttr[i]) ){
      inbody = 0;
    }
  }
  return 1;
}