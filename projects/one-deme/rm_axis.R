# Function to remove an axis from a plot
rm_axis <- function(axis = "x") {

  # Remove the x-axis...
  if (axis == "x") {

    theme(
      axis.line.x = element_blank(),
      axis.ticks.x = element_blank(),
      axis.text.x = element_blank(),
      axis.title.x = element_blank()
    )

  } else if (axis == "y") {

    # Or the y-axis
    theme(
      axis.line.y = element_blank(),
      axis.ticks.y = element_blank(),
      axis.text.y = element_blank(),
      axis.title.y = element_blank()
    )

  }
}
