#include "Base/Hash.h"

template<>
int Base::hash<uint32_t>(uint32_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::hash<int32_t>(int32_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::hash<uint16_t>(uint16_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::hash<int16_t>(int16_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::hash<uint8_t>(uint8_t const& value)
{
  return static_cast<int>(value);
}

template<>
int Base::hash<int8_t>(int8_t const& value)
{
  return static_cast<int>(value);
}