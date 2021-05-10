#pragma once
#include "Types.h"

constexpr uint8 decimal(float dec)
{
  return round(dec * 255);
}

constexpr uint8 percent(float percent)
{
  return decimal(percent / 100);
}