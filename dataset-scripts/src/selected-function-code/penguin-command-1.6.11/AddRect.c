void AddRect(int x1, int y1, int x2, int y2)
{
  int temp;
   /* Make sure X1 is before X2 */
   if (x2 < x1){
      temp = x2;
      x2 = x1;
      x1 = temp;
   }
   /* Make sure Y1 is before Y2 */
   if (y2 < y1){
      temp = y2;
      y2 = y1;
      y1 = temp;
   }
  blitrect.x = x1;
  blitrect.y = y1;
  blitrect.w = x2-x1+1;
  blitrect.h = y2-y1+1;
  if (x1<0) printf("x is too small in function AddRect!\n");else
  if (y1<0) printf("y is too small in function AddRect!\n");else
  if (x2>=800) printf("x is too big in function AddRect!\n");else
  if (y2>=600) printf("y is too big in function AddRect!\n");else {
      blitrects[blitrects_num]=blitrect;
      if (++blitrects_num>=RECTS_NUM-2)
      {printf("Too many blits!\n");blitrects_num--;Update();}
  }
}