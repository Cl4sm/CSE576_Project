num2str(value, string, dms)
double value;
char *string;
short dms;
{
        int eps, d, m, s;

        if (dms) {
          if (value<0) {
            value = -value; 
            eps = -1;
          } else
            eps = 1;
	  value = value+1/7200.0;
          d = (int) value;
          value = 60 * (value - d);
          m = (int) value;
          value = 60 * (value - m);       
          s = (int) value;
          sprintf(string, "%s%d°%02d'%02d\"", (eps==1)?"":"-", d, m, s);
        } else
          sprintf(string, "%.3f", value); 
        return string;
}
