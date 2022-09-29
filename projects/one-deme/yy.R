rm(list = ls())

library(tidyverse)
library(brachypoder)
library(brachypodx)
library(rlang)
library(patchwork)
source("rm_axis.R")

theme_set(theme_classic())

pars <- alist(

  rmax <- 4, # maximum growth
  s <- 0.5, # trade-off
  a <- 5, # steepness
  k1 <- 1000, # capacity 1
  k2 <- 1000, # capacity 2
  m <- 0.5, # dispersal
  theta1 <- 0, # stress 1
  theta2 <- 4 # stress 2

)

data <- map_dfr(c(0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7), function(s) {

  pars[[2]] <- parse_expr(paste("s <-", s))

  plot_pip(seq(0, 6, 0.1), pars, init = c(10000, 10000), plotit = FALSE) %>%
    mutate(s = s)

}, .id = "id")

p1 <- data %>%
  mutate(s = paste("s = ", s)) %>%
  ggplot(aes(x = xres, y = x, fill = invades)) +
  geom_tile() +
  xlab(parse(text = "'Resident trait value ('*hat(x)*')'")) +
  ylab("Mutant trait value (x)") +
  labs(fill = parse(text = "lambda(x, hat(x)) > 1")) +
  facet_wrap(. ~ s) +
  scale_fill_manual(values = c("gray20", "gray80")) +
  theme(legend.position = "none")

####################

data2 <- map_dfr(c(0, 1, 2, 3), function(theta1) {

  pars[[7]] <- parse_expr(paste("theta1 <-", theta1))

  plot_pip(seq(0, 6, 0.1), pars, init = c(10000, 10000), plotit = FALSE) %>%
    mutate(theta1 = theta1)

}, .id = "id")

p2 <- data2 %>%
  mutate(theta1 = paste0("theta[F]~'= ", theta1, "'")) %>%
  ggplot(aes(x = xres, y = x, fill = invades)) +
  geom_tile() +
  xlab(parse(text = "'Resident trait value ('*hat(x)*')'")) +
  ylab("Mutant trait value (x)") +
  labs(fill = parse(text = "lambda(x, hat(x)) > 1")) +
  facet_wrap(. ~ theta1, labeller = label_parsed) +
  scale_fill_manual(values = c("gray20", "gray80")) +
  theme(legend.position = "none")

####################

data3 <- map2_dfr(c(1000, 1200, 1400, 1600), c(1000, 800, 600, 400), function(k1, k2) {

  pars[[4]] <- parse_expr(paste("k1 <-", k1))
  pars[[5]] <- parse_expr(paste("k2 <-", k2))

  plot_pip(seq(0, 6, 0.1), pars, init = c(10000, 10000), plotit = FALSE) %>%
    mutate(k1 = k1, k2 = k2)

}, .id = "id")

p3 <- data3 %>%
  mutate(klab = paste0("K[F]~'= ", k1, ",'~K[UF]~'= ", k2, "'")) %>%
  ggplot(aes(x = xres, y = x, fill = invades)) +
  geom_tile() +
  xlab(parse(text = "'Resident trait value ('*hat(x)*')'")) +
  ylab("Mutant trait value (x)") +
  labs(fill = parse(text = "lambda(x, hat(x)) > 1")) +
  facet_wrap(. ~ klab, labeller = label_parsed) +
  scale_fill_manual(values = c("gray20", "gray80")) +
  theme(legend.position = "none")

####################

plot <- wrap_plots(
  p1 + theme(legend.position = "right"),
  wrap_plots(p2 + rm_axis("x"), p3, ncol = 1),
  nrow = 1, widths = c(2, 1), guides = "collect"
) +
  plot_annotation(tag_level = "A")

ggsave("yy.png", plot, width = 10, height = 7, dpi = 300)
