#include "grid.h"

#include <cassert>
#include <cmath>
#include <iostream>

grid::grid(
  const int width,
  const int height
) : mGrid(static_cast<size_t>(height), std::vector<double>(static_cast<size_t>(width), 0.0))
{

}

grid addLandscape(
  grid g,
  const double horizontalPeriod,
  const double verticalPeriod
)
{
    const int height{g.getHeight()};
    const int width{g.getWidth()};
    for (int y = 0; y != height; ++y)
    {
        for (int x = 0; x != width; ++x)
        {
          const double x_d{static_cast<double>(x)};
          const double y_d{static_cast<double>(y)};
          // z_unscaled in range [-2, 2]
          const double z_unscaled{
            std::sin((x_d * horizontalPeriod / (2.0 * M_PI))) +
            std::cos((y_d * verticalPeriod   / (2.0 * M_PI)))
          };
          const double z{0.5 + (z_unscaled / 4.0)};
          g.set(x, y, z);
        }
    }
    return g;
}

double grid::get(const int x, const int y) const
{
  assert(y >= 0);
  assert(y < getHeight());
  assert(x >= 0);
  assert(x < getWidth());
  return mGrid[static_cast<size_t>(y)][static_cast<size_t>(x)];
}

int grid::getHeight() const noexcept
{
  return static_cast<int>(mGrid.size());
}

int grid::getWidth() const
{
  assert(!mGrid.empty());
  return static_cast<int>(mGrid[0].size());
}

void grid::set(const int x, const int y, const double value)
{
  assert(y >= 0);
  assert(y < getHeight());
  assert(x >= 0);
  assert(x < getWidth());
  mGrid[static_cast<size_t>(y)][static_cast<size_t>(x)] = value;
}

void testGrid()
{
  // Default construct
  {
    const grid g;
    assert(g.getHeight() == 0);
    // assert(g.get_width() == 0); Cannot get width, if there is no row
  }
  // Default construct
  {
    const grid g(2, 3);
    assert(g.getWidth() == 2);
    assert(g.getHeight() == 3);
  }
  // Set and get are symmetric
  {
    grid g(4, 5);
    g.set(2, 3, 3.14);
    assert(std::abs(g.get(2, 3) - 3.14) < 0.000001);
  }
}

std::ostream& operator<<(std::ostream& os, const grid& g)
{
    const int height{g.getHeight()};
    assert(g.getHeight() > 0);
    const int width{g.getWidth()};
    for (int y = 0; y != height; ++y)
    {
        for (int x = 0; x != width; ++x)
        {
          const double z{g.get(x, y)};
          os << '0' + static_cast<int>(z * 10.0);
        }
        os << '\n';
    }
    return os;
}
