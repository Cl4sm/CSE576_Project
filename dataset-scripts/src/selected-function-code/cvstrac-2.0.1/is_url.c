int is_url(const char *z){
  int i;
  int minlen = 6;
  switch( z[0] ){
    case 'h':
     if( strncmp(z,"http:",5)==0 ) minlen = 7;
     else if( strncmp(z,"https:",6)==0 ) minlen = 8;
     else return 0;
     break;
    case 'f':
     if( strncmp(z,"ftp://",6)==0 ) minlen = 7;
     else return 0;
     break;
    case 'm':
     if( strncmp(z,"mailto:",7)==0 ) minlen = 10;
     else return 0;
     break;
    default:
     return 0;
  }
  for(i=0; z[i] && !isspace(z[i]); i++){}
  while( i>0 ){
    switch( z[i-1] ){
      case '.':
      case ')':
      case ']':
      case '}':
      case '?':
      case '!':
      case '"':
      case '\'':
      case ':':
      case ';':
      case ',':
        i--;
        break;
      default:
        return i>=minlen ? i : 0;
    }
  }
  return 0;
}