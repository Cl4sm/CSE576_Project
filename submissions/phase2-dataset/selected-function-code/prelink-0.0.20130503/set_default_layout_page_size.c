static void
set_default_layout_page_size (void)
{
  /* From gcc.dg/20020523-1.c test in gcc 3.2 testsuite.  */
  int fl1, fl2;

#ifndef __x86_64__
  /* See if we can use cpuid.  */
  __asm__ ("pushfl; pushfl; popl %0; movl %0,%1; xorl %2,%0;"
	   "pushl %0; popfl; pushfl; popl %0; popfl"
	   : "=&r" (fl1), "=&r" (fl2)
	   : "i" (0x00200000));
  if (((fl1 ^ fl2) & 0x00200000) == 0)
    return;
#define cpuid(fl1, fl2, fn) \
  __asm__ ("movl %%ebx, %1; cpuid; xchgl %%ebx, %1" \
	   : "=a" (fl1), "=r" (fl2) : "0" (fn) : "ecx", "edx")
#else
#define cpuid(fl1, fl2, fn) \
  __asm__ ("cpuid" : "=a" (fl1), "=b" (fl2) : "0" (fn) : "ecx", "edx")
#endif

  /* See if CPUID gives capabilities.  */
  cpuid (fl1, fl2, 0);
  if (fl1 < 1 || fl2 != 0x68747541 /* Auth - AMD */)
    return;

  /* CPUID 1.  */
  cpuid (fl1, fl2, 1);
  if (((fl1 >> 8) & 0x0f) + ((fl1 >> 20) & 0xff) == 0x15 /* Family */)
    /* On AMD Bulldozer CPUs default to --layout-page-size=0x8000.  */
    layout_page_size = 0x8000;
}
