void dehttpize(char *z){
  int i, j;
  i = j = 0;
  while( z[i] ){
    switch( z[i] ){
      case '%':
        if( z[i+1] && z[i+2] ){
          z[j] = AsciiToHex(z[i+1]) << 4;
          z[j] |= AsciiToHex(z[i+2]);
          i += 2;
        }
        break;
      case '+':
        z[j] = ' ';
        break;
      default:
        z[j] = z[i];
        break;
    }
    i++;
    j++;
  }
  z[j] = 0;
}