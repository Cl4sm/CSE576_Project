static int score_word(const char *zWord, const char *zText, int n, int *pIdx){
  int c1, c2, i, best;
  int idx = -1;

  best = 0;
  c1 = zWord[0];
  if( isupper(c1) ){
    c2 = tolower(c1);
  }else{
    c2 = toupper(c1);
  }
  if( n<=0 ) n = strlen(zWord);
  for(i=0; zText[i]; i++){
    if( (zText[i]==c1 || zText[i]==c2) && strncasecmp(zWord,&zText[i],n)==0){
      int score = 6;
      if( (i==0 || !isalnum(zText[i-1]))
           && (zText[i+n]==0 || !isalnum(zText[i+n])) ){
        score = 8;
        if( strncmp(zWord,&zText[i],n)==0 ){
          idx = i;
          best = score = 10;
          break;
        }
      }
      if( score>best ){
        best = score;
        idx = i;
      }
    }
  }
  if( pIdx ) *pIdx = idx;
  return best;
}