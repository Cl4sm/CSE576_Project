static void adjust_list_nesting(int *aList, int iTarget){
  int iDepth = iTarget;
  if( iDepth<0 ) iDepth = 0x7fffffff & -iDepth;
  if( aList[0]==iDepth && iDepth>0 && aList[iDepth]*iTarget<0 ){
    iDepth--;
  }
  while( aList[0]>iDepth ){
    if( aList[aList[0]--]>0 ){
      cgi_printf("</ul>\n");
    }else{
      cgi_printf("</ol>\n");
    }
  }
  while( aList[0]<iDepth-1 ){
    cgi_printf("<ul>\n");
    aList[0]++;
    aList[aList[0]] = +1;
  }
  iDepth = iTarget;
  if( iDepth<0 ) iDepth = 0x7fffffff & -iDepth;  
  if( aList[0]==iDepth-1 ){
    if( iTarget<0 ){
      cgi_printf("<ol>\n");
      aList[iDepth] = -1;
    }else{
      cgi_printf("<ul>\n");
      aList[iDepth] = +1;
    }
    aList[0]++;
  }
}