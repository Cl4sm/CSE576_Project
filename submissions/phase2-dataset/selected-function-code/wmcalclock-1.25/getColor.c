unsigned long getColor(char *ColorName, float fac, int *red, int *grn, int *blu) {

     XColor 		Color;
     XWindowAttributes 	Attributes;

     XGetWindowAttributes(display, Root, &Attributes);
     Color.pixel = 0;

     XParseColor(display, Attributes.colormap, ColorName, &Color);
     Color.red 	 = (unsigned short)(fac*(Color.red-24) + 24);
     Color.blue  = (unsigned short)(fac*(Color.blue-24) + 24);
     Color.green = (unsigned short)(fac*(Color.green-24) + 24);
     Color.flags = DoRed | DoGreen | DoBlue;
     XAllocColor(display, Attributes.colormap, &Color);

     
     *red = Color.red;
     *grn = Color.green;
     *blu = Color.blue;
     return Color.pixel;

}
