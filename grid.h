#ifndef GRID_H
#define GRID_H

#include <vector>

#include <random>
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
  int get_height() const noexcept;

  /// Get the number of columns
  /// Throws an exception if there are no rows
  int get_width() const;

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
  std::vector<std::vector<double>> m_grid;
};

/// Add Perlin noise to the grid
/// @param g a grid
/// @param frequency the Perlin noise frequence
/// @param n_octaves the Perlin noise number of octaves
/// @param rng_seed the random number generator seed.
///   TODO: replace by using a RNG engine by reference,
///   (for example, 'std::mt19937&'), so a simulation has
///   exactly one RNG engine
grid add_perlin_noise(
  const grid g,
  const double frequency = 8.0,
  const int n_octaves = 8,
  const int rng_seed = 12345
);

/// Create a grid with Perlin noise
/// @param width the number of columns
/// @param height the number of rows
/// @param frequency the Perlin noise frequence
/// @param n_octaves the Perlin noise number of octaves
/// @param rng_seed the random number generator seed.
///   TODO: replace by using a RNG engine by reference,
///   (for example, 'std::mt19937&'), so a simulation has
///   exactly one RNG engine
grid create_grid_with_perlin_noise(
  const int width = 0,
  const int height = 0,
  const double frequency = 8.0,
  const int n_octaves = 8,
  const int rng_seed = 12345
);

/// Test the grid, to be sure it works correctly
void test_grid();

#endif // GRID_H
