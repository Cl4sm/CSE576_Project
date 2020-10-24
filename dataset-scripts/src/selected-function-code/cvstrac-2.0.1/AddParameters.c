static void AddParameters(int index, int *pArgc, char ***pArgv){
  int argc = *pArgc;      /* The original argc value */
  char **argv = *pArgv;   /* The original argv value */
  int newArgc;            /* Value for argc after inserting new arguments */
  char **zNew = 0;        /* The new argv after this routine is done */
  char *zFile;            /* Name of the input file */
  int nNew = 0;           /* Number of new entries in the argv[] file */
  int nAlloc = 0;         /* Space allocated for zNew[] */
  int i;                  /* Loop counter */
  int n;                  /* Number of characters in a new argument */
  int c;                  /* Next character of input */
  int startOfLine = 1;    /* True if we are where '#' can start a comment */
  FILE *in;               /* The input file */
  char zBuf[1000];        /* A single argument is accumulated here */

  if( index+1==argc ) return;
  zFile = argv[index+1];
  in = fopen(zFile,"r");
  if( in==0 ){
    fprintf(stderr,"Can't open input file \"%s\"\n",zFile);
    exit(1);
  }
  c = ' ';
  while( c!=EOF ){
    while( c!=EOF && isspace(c) ){
      if( c=='\n' ){
        startOfLine = 1;
      }
      c = getc(in); 
      if( startOfLine && c=='#' ){
        while( c!=EOF && c!='\n' ){
          c = getc(in);
        }
      }
    }
    n = 0;
    while( c!=EOF && !isspace(c) ){
      if( n<sizeof(zBuf)-1 ){ zBuf[n++] = c; }
      startOfLine = 0;
      c = getc(in);
    }
    zBuf[n] = 0;
    if( n>0 ){
      nNew++;
      if( nNew + argc > nAlloc ){
        if( nAlloc==0 ){
          nAlloc = 100 + argc;
          zNew = malloc( sizeof(char*) * nAlloc );
        }else{
          nAlloc *= 2;
          zNew = realloc( zNew, sizeof(char*) * nAlloc );  
        }
      }
      if( zNew ){
        int j = nNew + index;
        zNew[j] = malloc( n + 1 );
        if( zNew[j] ){
          strcpy( zNew[j], zBuf );
        }
      }
    }
  }
  newArgc = argc + nNew - 1;
  for(i=0; i<=index; i++){
    zNew[i] = argv[i];
  }
  for(i=nNew + index + 1; i<newArgc; i++){
    zNew[i] = argv[i + 1 - nNew];
  }
  zNew[newArgc] = 0;
  *pArgc = newArgc;
  *pArgv = zNew;
}