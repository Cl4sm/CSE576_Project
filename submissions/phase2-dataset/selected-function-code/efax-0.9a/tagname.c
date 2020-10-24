{
  static struct tagnamestruct {  int code ;  char *name ; } 
  tagnames [] = {
  { 256, "width" },
  { 257, "length" },
  { 258, "bits/sample" },
  { 259, "compresssion(g3=3)" },
  { 262, "photometric(0-min=white)" },
  { 266, "fill order(msb2lsb=1)" },
  { 273, "strip offsets" },
  { 274, "orientation(1=normal)" },
  { 277, "samples/pixel" },
  { 278, "rows/strip" },
  { 279, "strip byte counts" },
  { 282, "xresolution" },
  { 283, "yresolution" },
  { 284, "storage(1=single plane)" },
  { 292, "g3options" },
  { 296, "resolution units(2=in,3=cm)" },
  { 297, "page number" },
  { 327, "clean fax(0=clean/1=regen/2=errors)" },
  {0,0} },
  *p ;
  
  for ( p=tagnames ; p->code ; p++ )
    if ( tag == p->code ) break ;
  return p->code ? p->name :  "unknown tag" ;
}
