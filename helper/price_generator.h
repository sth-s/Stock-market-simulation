#ifndef PRICE_GENERATOR
#define PRICE_GENERATOR
#include "input_parser.h"

/**
 * Generates the next value of price based on share info
 *
 * @param share share for price generation
 *
 * @return randomly generated share price on the next day
 *
 **/
double price_generator(const Share &);

#endif