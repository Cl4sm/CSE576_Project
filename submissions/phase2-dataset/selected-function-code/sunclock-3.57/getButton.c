int 
getButton(win, x, y)
Window win;
int x, y;
{
struct Sundata * Context=NULL;
char *keys=NULL;
int nkeys=0;
int y1, w0, n, i, j=0, b, d;
    getWinParams(win, &Context, &keys, &nkeys, &y1, &w0);
    n = -1;
    if (y<=y1 || y>=y1+Context->gdata->menustrip) return n;
    b = 0;
    d = Context->gdata->charspace;
    if (win==Filesel) d = 2*d;
    for (i=0; i<nkeys; i++) {
	j = i*d + b;
	b += 5*(keys[2*i+1]==';');
	if (x>j && x<j+d) {
	    n = i;
	    break;
	}
    }
    if (x>j+d+5) n = nkeys;
    return n;
}
