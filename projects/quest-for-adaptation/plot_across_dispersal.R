rm(list = ls())

library(tidyverse)
library(brachypoder)

theme_set(theme_classic())

paths <- file.path("data", c(
  "sim-quest-default",
  "sim-quest-dispersal-0.3",
  "sim-quest-dispersal-0.5"
))

data <- map_dfr(paths, function(path) {

  data <- read_trait_mean_data(path)
  pars <- read_parameters(path)
  data$dispersal <- pars$dispersal
  return(data)

}, .id = "sim")

pars <- read_parameters(paths[1])

data <- data %>% mutate(dispersal_lab = paste("m =", dispersal))

plot <- data %>%
  ggplot(aes(x = time, y = z)) +
  geom_hline(yintercept = pars$stress[1], color = "#efe58b") +
  geom_hline(yintercept = pars$stress[2], color = "#006400") +
  geom_point() +
  xlab("Time (generations)") +
  ylab("Trait value") +
  facet_wrap(. ~ dispersal_lab)

ggsave("dispersal.png", plot, width = 6, height = 3, dpi = 400)
