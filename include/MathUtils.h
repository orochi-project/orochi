#pragma once

/**
 * Get the sign of a number.
 *
 * @param x The number to get the sign of.
 */
#define SIGN(x) (((x) > 0) - ((x) < 0))

/**
 * Calculate the value of (a / b) * c, rounded to the nearest integer.
 *
 * @param a The value to divide.
 * @param b The divisor.
 * @param c The multiplier.
 */
#define DIV_MUL_ROUND(a, b, c) (((a) * (c) + (b) / 2) / (b))
