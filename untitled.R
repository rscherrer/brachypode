rm(list = ls())

library(tidyverse)

theme_set(theme_classic())

m <- 0.5
rmax <- 4
s <- 0.5
a <- 5
k1 <- 1000
k2 <- 1000
theta1 <- 1
theta2 <- 3

r <- function(x) rmax - s * x
g <- function(x, theta) r(x) / (1 + exp(a * (theta - x)))

tibble(x = seq(0, 5, 0.1), r = r(x)) %>%
  ggplot(aes(x = x, y = r)) +
  geom_line() +
  geom_hline(yintercept = 1, linetype = 3)

tibble(theta = seq(0, 5, 0.1), g = g(2, theta)) %>%
  ggplot(aes(x = theta, y = g)) +
  geom_line() +
  geom_vline(xintercept = 2, linetype = 2) +
  geom_vline(xintercept = theta1) +
  geom_vline(xintercept = theta2) +
  geom_hline(yintercept = 1, linetype = 3)

w <- function(x, theta, n, k) g(x, theta) - n / k

w1 <- function(y, x) w(y, theta1, n1(x), k1)
w2 <- function(y, x) w(y, theta2, n2(x), k2)

lambda <- function(y, x) {

  w1 <- w1(y, x)
  w2 <- w2(y, x)

  0.5 * ((1 - m) * (w1 + w2) + sqrt((m - 1)^2 * (w1 + w2)^2 - 4 * (1 - 2 * m) * w1 * w2))

}
