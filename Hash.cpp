#include "Base/Hash.h"

template<>
int Base::getHash<uint32_t>(uint32_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::getHash<int32_t>(int32_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::getHash<uint16_t>(uint16_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::getHash<int16_t>(int16_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::getHash<uint8_t>(uint8_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::getHash<int8_t>(int8_t const& value)
{
  return static_cast<int>(value);
}