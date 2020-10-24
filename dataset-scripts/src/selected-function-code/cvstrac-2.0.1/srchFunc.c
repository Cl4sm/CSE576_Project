void srchFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  int i, j, k, score, total;
  char *zPattern;

  if( argc<2 || argv[0]==0 ) return;
  zPattern = (char *)sqlite3_value_text(argv[0]);
  total = 0;
  if( zPattern && zPattern[0] ){
    for(i=0; zPattern[i]; i++){
      if( isspace(zPattern[i]) ) continue;
      for(j=1; zPattern[i+j] && !isspace(zPattern[i+j]); j++){}
      score = 0;
      for(k=1; k<argc; k++){
        int one_score;
        char *zWord = (char *)sqlite3_value_text(argv[k]);
        if( zWord==0 || zWord[0]==0 ) continue;
        one_score = score_word(&zPattern[i], zWord, j, 0);
        if( one_score>score ) score = one_score;
      }
      total += score;
      i += j-1;
    }
  }
  sqlite3_result_int(context, total);
}