parseFormats(char * format)
{
        int i, j, l;
        
        l = strlen(format);

        j = 1;
        while (j>0) {
           j = 0;
           if (format[0]=='|') { ++format; --l; j = 1;}
           if (l>=2 && format[l-1]=='|' && format[l-2]!= '%') 
                               { format[l-1]='\0'; --l; j = 1; }
        }

        num_formats = 1;
        for (i=1; i<l-1; i++) 
            if (format[i]=='|' && format[i-1]!='%') ++num_formats;

        DateFormat = (char **) salloc(num_formats*sizeof(char *));

        DateFormat[0] = format;
        j = 0;

        for (i=1; i<l-1; i++) 
          if (format[i]=='|' && format[i-1]!='%') {
            ++j;
            format[i] = '\0';
            DateFormat[j] = format+i+1;
          }
}
