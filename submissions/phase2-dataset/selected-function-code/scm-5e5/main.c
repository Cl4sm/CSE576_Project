int c;char **v;{
  int n=200,j=0,m,b=2,k=0,t,r=1,d=5;
  long q;
  short *a;
  if(c>1)n=atoi(v[1]);
  if(c>2)d=atoi(v[2]);
  while(k++<d)r=r*10;
  n=n/d+1;
  k=m=3.322*n*d;
  a=calloc(1+m,2);
  while(k)a[--k]=2;
  for(a[m]=4;j<n;b=q%r){
    q=0;
    for(k=m;k;){
      q+=a[k]*r;
      t=(2*k+1);
      a[k]=q%t;
      q=q/t;
      q*=k--;}
    printf("%0*d%s",d,b+q/r,++j%10?"  ":"\n");}
  puts("");}
