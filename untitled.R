rm(list = ls())

library(tidyverse)
library(brachypoder)

theme_set(theme_classic())

data <- read_trait_mean_data("test")

data %>%
  ggplot(aes(x = time, y = x, group = interaction(deme, patch))) +
  geom_point() +
  facet_grid(deme ~ patch)

