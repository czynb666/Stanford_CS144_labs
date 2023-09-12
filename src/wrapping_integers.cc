#include "wrapping_integers.hh"
#include <iostream>

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point ) {
  return Wrap32((uint32_t)(zero_point.raw_value_ + n));
}

uint64_t dis(uint64_t x, uint64_t y) {
  return max(x, y) - min(x, y);
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const {
  uint64_t t = (uint32_t)(raw_value_ - zero_point.raw_value_);
  uint64_t x = ((checkpoint >> 32) << 32) + t;
  uint64_t dx = dis(x, checkpoint);

  uint64_t y = x - ((uint64_t)1 << 32);
  uint64_t dy = dis(y, checkpoint);
  
  uint64_t z = x + ((uint64_t)1 << 32);
  uint64_t dz = dis(z, checkpoint);

  return dz <= min(dx, dy) ? z : (dy <= dx ? y : x);
}
