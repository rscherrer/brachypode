rm(list = ls())

library(tidyverse)
library(brachypoder)

theme_set(theme_classic())

sim_name <- "sim-quest-default"
path <- sprintf("data/%s/", sim_name)

data <- read_trait_mean_data(path)
pars <- read_parameters(path)

plot <- data %>%
  ggplot(aes(x = time, y = z)) +
  geom_hline(yintercept = pars$stress[1], color = "#efe58b") +
  geom_hline(yintercept = pars$stress[2], color = "#006400") +
  geom_point() +
  xlab("Time (generations)") +
  ylab("Trait value")

ggsave(sprintf("%s.png", sim_name), plot, width = 3, height = 2, dpi = 400)
