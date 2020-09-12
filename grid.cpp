#include "grid.h"

#include <cassert>

//#include "PerlinNoise/PerlinNoise.hpp"

grid::grid(
  const int width,
  const int height
) : m_grid(static_cast<size_t>(height), std::vector<double>(static_cast<size_t>(width), 0.0))
{

}

void test_grid()
{
  // Default construct
  {
    const grid g;
    assert(g.get_height() == 0);
    // assert(g.get_width() == 0); Cannot get width, if there is no row
  }
  // Default construct
  {
    const grid g(2, 3);
    assert(g.get_width() == 2);
    assert(g.get_height() == 3);
  }
  // Set and get are symmetric
  {
    grid g(4, 5);
    g.set(2, 3, 3.14);
    assert(std::abs(g.get(2, 3) - 3.14) < 0.000001);
  }
}

double grid::get(const int x, const int y) const
{
  assert(y >= 0);
  assert(y < get_height());
  assert(x >= 0);
  assert(x < get_width());
  return m_grid[static_cast<size_t>(y)][static_cast<size_t>(x)];
}

int grid::get_height() const noexcept
{
  return static_cast<int>(m_grid.size());
}

int grid::get_width() const
{
  assert(!m_grid.empty());
  return static_cast<int>(m_grid[0].size());
}

void grid::set(const int x, const int y, const double value)
{
  assert(y >= 0);
  assert(y < get_height());
  assert(x >= 0);
  assert(x < get_width());
  m_grid[static_cast<size_t>(y)][static_cast<size_t>(x)] = value;
}
