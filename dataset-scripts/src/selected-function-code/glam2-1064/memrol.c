void memrol(void *s, size_t n, size_t k) {
  size_t cycles;
  char *start;
  if (n == 0)  /* prevent % 0 */
    return;
  k %= n;  /* allow rotations of size > n */
  if (k == 0)
    return;
  assert(s != NULL);
  cycles = gcd(n, k);

  for (start = s; cycles != 0; ++start) {
    char *x = start + k;
    char tmp = *x;
    while (x != start) {
      char *next = x < (char *)s+n-k ? x+k : x-(n-k);
      *x = *next;
      x = next;
    }
    *start = tmp;
    --cycles;
  }
}