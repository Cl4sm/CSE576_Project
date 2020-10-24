const char *cgi_parameter(const char *zName, const char *zDefault){
  int lo, hi, mid, c;
  if( nUsedQP<=0 ) return zDefault;
  if( sortQP ){
    qsort(aParamQP, nUsedQP, sizeof(aParamQP[0]), qparam_compare);
    sortQP = 0;
  }
  lo = 0;
  hi = nUsedQP-1;
  while( lo<=hi ){
    mid = (lo+hi)/2;
    c = strcmp(aParamQP[mid].zName, zName);
    if( c==0 ){
      return aParamQP[mid].zValue;
    }else if( c>0 ){
      hi = mid-1;
    }else{
      lo = mid+1;
    }
  }
  return zDefault;
}