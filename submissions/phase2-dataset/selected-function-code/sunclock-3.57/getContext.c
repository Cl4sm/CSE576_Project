getContext(win)
Window win;
{
   struct Sundata * Context;
   if (win==Menu) 
     return MenuCaller;
   if (win==Filesel)
     return FileselCaller;
   if (win==Zoom)
     return ZoomCaller;
   if (win==Option)
     return OptionCaller;
   if (win==Urban)
     return UrbanCaller;

   Context = Seed;
   while (Context && Context->win != win) Context = Context->next;
   return Context;
}
