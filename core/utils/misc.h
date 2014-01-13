#ifndef MISC_H
#define MISC_H

namespace bits {
inline long countBits(long n) {  
  unsigned int c = 0;
  for (; n; ++c) n &= n - 1;
  return c;
}
} // namespace bits
#endif // MISC_H
