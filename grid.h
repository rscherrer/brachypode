#ifndef GRID_H
#define GRID_H

#include <iosfwd>
#include <vector>

/// A 2D grid
class grid
{
public:
  ///Construct a grid
  /// @param width the number of columns
  /// @param height the number of rows
  grid(
    const int width = 0,
    const int height = 0
  );

  /// Get a value.
  /// Throws an exception if the grid location does not exist.
  /// @param x the column, starts at index 0
  /// @param y the row, starts at index 0
  double get(const int x, const int y) const;

  /// Get the number of rows
  int getHeight() const noexcept;

  /// Get the number of columns
  /// Throws an exception if there are no rows
  int getWidth() const;

  /// Set a value.
  /// Throws an exception if the grid location does not exist.
  /// @param x the column, starts at index 0
  /// @param y the row, starts at index 0
  /// @param value the value to store in the grid
  void set(const int x, const int y, const double value);

private:
  /// Stored row-wise, i.e. [y][x]
  ///
  /// I will use a double as a data type for now,
  /// this one day will become 'Individual'
  std::vector<std::vector<double>> mGrid;
};

/// Add a landscape to the grid: resulting values go from zero to one
/// @param g a grid
/// @param horizontalPeriod the horizontal period (in number
///   of grid cells) after which the pattern repeats
/// @param verticalPeriod the vertical period (in number
///   of grid cells) after which the pattern repeats
grid addLandscape(
  grid g,
  const double horizontalPeriod,
  const double verticalPeriod
);

/// Test the grid, to be sure it works correctly
void testGrid();

/// Allow std::cout to stream a grid to screen
/// Uses ASCII art :-)
std::ostream& operator<<(std::ostream&, const grid&);


#endif // GRID_H
