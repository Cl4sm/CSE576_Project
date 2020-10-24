Window win;
struct Sundata **Context;
char **keys;
int *nkeys;
int *y;
int *width;
{
    *Context = NULL;
    if (win==Menu) {
       *Context = MenuCaller;
       *keys = MenuKey;
       *nkeys = N_MENU;
       *y = 0;
       *width = MenuGeom.width;
    }
    if (win==Option) {
       *Context = OptionCaller;
       *keys = OptionKey;
       *nkeys = N_OPTION;
       *y = OptionGeom.height-2*OptionCaller->gdata->menustrip - 1;
       *width = OptionGeom.width;
    }
    if (win==Zoom) {
       *Context = ZoomCaller;
       *keys = ZoomKey;
       *nkeys = N_ZOOM;
       *y = areah+64;
       *width = ZoomGeom.width;
    }
    if (win==Urban) {
       *Context = UrbanCaller;
       *keys = UrbanKey;
       *nkeys = N_URBAN;
       *y = urbanh;
       *width = UrbanGeom.width;
    }
    if (win==Filesel) {
       *Context = FileselCaller;
       *keys = WeakChars;
       *nkeys = 6;
       *y = 0;
       *width = FileselGeom.width;
    }
}
