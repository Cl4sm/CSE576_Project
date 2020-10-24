     PixmapWidget PW;
     Position     x, y;
     Pixel          value, toflood;
{
  Position save_x, save_y, x_left, x_right;

  if (value == toflood) return; /* nothing to flood in this case */
  
  if (QueryFlood(PW, x, y, toflood)) 
    Flood(PW, x, y, value);
      
  save_x = x;
  save_y = y;
  
  x++;
  while (QueryFlood(PW, x, y, toflood)) {
    Flood(PW, x, y, value);
    x++;
  }
  x_right = --x;
  
  x = save_x;
  x--;
  while (QueryFlood(PW, x, y, toflood)) {
    Flood(PW, x, y, value);
    x--;
  }
  x_left = ++x;
  
  
  x = x_left;
  y = save_y;
  y++;
  
  while (x <= x_right) {
    Boolean flag = False;
    Position x_enter;
    
    while (QueryFlood(PW, x, y, toflood) && (x <= x_right)) {
      flag = True;
      x++;
    }
    
    if (flag) {
      if ((x == x_right) && QueryFlood(PW, x, y, toflood))
	FloodLoop(PW, x, y, value, toflood);
      else
	FloodLoop(PW, x - 1, y, value, toflood);
    }
    
    x_enter = x;
    
    while (!QueryFlood(PW, x, y, toflood) && (x < x_right))
      x++;
    
    if (x == x_enter) x++;
  }
  
  x = x_left;
  y = save_y;
  y--;
  
  while (x <= x_right) {
    Boolean flag = False;
    Position x_enter;
    
    while (QueryFlood(PW, x, y, toflood) && (x <= x_right)) {
      flag = True;
      x++;
    }
    
    if (flag) {
      if ((x == x_right) && QueryFlood(PW, x, y, toflood))
	FloodLoop(PW, x, y, value, toflood);
      else
	FloodLoop(PW, x - 1, y, value, toflood);
    }
    
    x_enter = x;
    
    while (!QueryFlood(PW, x, y, toflood) && (x < x_right))
      x++;
    
    if (x == x_enter) x++;
  }
}
