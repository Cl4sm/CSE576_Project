static int has_binary_keyword(const char* filename){
  FILE* in;
  char line[80];
  int has_binary=0;

  in = fopen(filename, "r");
  if( in==0 ) return 2;

  while( fgets(line, sizeof(line), in) ){
    /* End of header? */
    if( line[0]=='\n' || line[0]=='\r' ){
      break;
    }

    /* Is this the "expand" field? */
#define EXPAND "expand"
    if( strncmp(line, EXPAND, strlen(EXPAND))==0 ){
      /* Does its value contain 'b'? */
      if( strchr(line+strlen(EXPAND), 'b') ){
        has_binary=1;
      }
      break;
    }
  }

  fclose(in);
  return has_binary;
}