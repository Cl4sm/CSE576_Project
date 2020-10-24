unsigned char *encode64(const unsigned char *zData, int nData){
  unsigned char *z64;
  int i, n;

  if( nData<=0 ){
    nData = strlen((const char *)zData);
  }
  z64 = malloc( (nData*4)/3 + 6 );
  if(z64==0) return 0;
  for(i=n=0; i+2<nData; i+=3){
    z64[n++] = zBase[ (zData[i]>>2) & 0x3f ];
    z64[n++] = zBase[ ((zData[i]<<4) & 0x30) | ((zData[i+1]>>4) & 0x0f) ];
    z64[n++] = zBase[ ((zData[i+1]<<2) & 0x3c) | ((zData[i+2]>>6) & 0x03) ];
    z64[n++] = zBase[ zData[i+2] & 0x3f ];
  }
  if( i+1<nData ){
    z64[n++] = zBase[ (zData[i]>>2) & 0x3f ];
    z64[n++] = zBase[ ((zData[i]<<4) & 0x30) | ((zData[i+1]>>4) & 0x0f) ];
    z64[n++] = zBase[ ((zData[i+1]<<2) & 0x3c) ];
  }else if( i<nData ){
    z64[n++] = zBase[ (zData[i]>>2) & 0x3f ];
    z64[n++] = zBase[ ((zData[i]<<4) & 0x30) ];
  }
  z64[n] = 0;
  return z64;
}