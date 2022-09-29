rm(list = ls())

library(tidyverse)
library(patchwork)

theme_set(theme_classic())

rmax <- 4

svalues <- c(0.1, 0.3, 0.5, 0.7)

data <- map_dfr(svalues, function(s) {

  tibble(x = seq(0, 6, 0.01), s = s, r = rmax - s * x)

})

p1 <- data %>%
  ggplot(aes(x = x, y = r, group = s, color = s)) +
  geom_line() +
  scale_color_gradient(low = "black", high = "purple", guide = "legend", breaks = svalues) +
  xlab("Stress tolerance (x)") +
  ylab("Intrinsic growth (r(x))") +
  geom_hline(yintercept = 1, linetype = 3)

s <- 0.5
a <- 5

xvalues <- c(1, 2, 3, 4, 5)

data2 <- map_dfr(xvalues, function(x) {

  tibble(
    theta = seq(0, 6, 0.01),
    x = x,
    g = (rmax - s * x) / (1 + exp(a * (theta - x)))
  )

})

p2 <- data2 %>%
  ggplot(aes(x = theta, y = g, color = x, group = x)) +
  geom_line(alpha = 0.8) +
  scale_color_gradient(low = "gray80", high = "gray20", guide = "legend", breaks = xvalues) +
  xlab(parse(text = "'Environmental stress ('*theta*')'")) +
  ylab("Growth rate (g(x))") +
  geom_vline(xintercept = 0, linetype = 1, color = "forestgreen") +
  geom_vline(xintercept = 4, linetype = 1, color = "lightgoldenrod") +
  geom_hline(yintercept = 1, linetype = 3)

plot <- wrap_plots(p2, p1, nrow = 1, widths = c(4, 3))

ggsave("zz.png", plot, width = 8, height = 3, dpi = 300)
ggsave("zz_labelled.png", plot + plot_annotation(tag_level = "A"), width = 8, height = 3, dpi = 300)
