rm(list = ls())

library(tidyverse)
library(brachypoder)
library(brachypodx)
library(patchwork)

theme_set(theme_classic())

pars <- alist(

  m <- 0.5, # dispersal
  d1 <- 0.001, # density-dependence in habitat 1
  d2 <- 0.001, # density-dependence in habitat 2
  bmax <- 10, # maximum number of seeds
  s <- 0.4, # trade-off
  a <- 5, # tolerance decay
  theta1 <- 0, # stress in habitat 1
  theta2 <- 4 # stress in habitat 2

)

pip <- plot_pip(seq(0, 6, 0.01), pars, init = c(1000, 1000))

data <- map_dfr(paste0("data/allfreq/sim", 1:4), function(dir) {

  read_trait_mean_data(dir) %>%
    mutate(allfreq = read_parameters(dir)$allfreq)

}, .id = "id")

from <- first(data$time)
to <- last(data$time)

data2 <- data %>%
  filter(time == from | time == to) %>%
  mutate(time = str_replace(time, as.character(to), "to")) %>%
  mutate(time = str_replace(time, as.character(from), "from")) %>%
  pivot_wider(names_from = "time", values_from = "x")

data2 <- data2 %>%
  mutate(patch_lab = fct_recode(as.character(patch), UF = "0", F = "1"))

a <- 0.04

p1 <- pip +
  scale_fill_manual(
    values = c("gray20", "gray80"),
    guide = guide_legend(override.aes = list(color = NA))
  ) +
  geom_segment(
    data = data2,
    mapping = aes(
      x = from - a * (patch * 2 - 1),
      xend = to - a * (patch * 2 - 1),
      y = from + a * (patch * 2 - 1),
      yend = to + a * (patch * 2 - 1),
      fill = NULL,
      color = patch_lab
    ),
    arrow = arrow(length = unit(0.3, "cm"))
  ) +
  geom_point(
    data = data2,
    mapping = aes(
      x = from - a * (patch * 2 - 1),
      y = from + a * (patch * 2 - 1),
      fill = NULL,
      color = patch_lab
    ),
    size = 2
  ) +
  scale_color_manual(values = c("lightgoldenrod", "forestgreen")) +
  labs(color = "Patch")

p2 <- data %>%
  mutate(patch = fct_recode(as.character(patch), Unfacilitated = "0", Facilitated = "1")) %>%
  ggplot(aes(x = time, y = x, color = patch, group = id)) +
  geom_line() +
  facet_grid(. ~ patch) +
  scale_color_manual(values = c("lightgoldenrod", "forestgreen")) +
  theme(legend.position = "none") +
  xlab("Time (generations)") +
  ylab("Stress tolerance (x)")

plot <- wrap_plots(p1, p2, nrow = 1, guides = "collect", widths = c(4, 3)) +
  plot_annotation(tag_level = "A")

ggsave("test.png", plot, width = 8, height = 4, dpi = 300)
