void get_next_volname(void)             // get file name of next volume
{
   CHAR *cp;
   INT  num;

   if ((cp = (CHAR *) strrchr(aname, '.')) == NULL || !*(cp + 1))
      num = -1;
   else
   {
      cp++;
      num = (*(cp + 1) - '0') * 10 + *(cp + 2) - '0';
      if (!in(num, 0, 99))
         num = -1;
      if (in(*cp, '0', '9'))
         num += (*cp - '0') * 100;
   }
   num++;

   if (num < 100)
      *cp = 'C';
   else
      *cp = num / 100 + '0';
   *(cp + 1) = (num / 10) % 10 + '0';
   *(cp + 2) = num % 10 + '0';
}