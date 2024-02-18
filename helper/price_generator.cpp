#include <cmath>
#include <random>
#include <chrono>
#include "price_generator.h"

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);
std::normal_distribution<double> norm_dist(0.0, 1.0);

double price_generator(const Share &share)
{
    const double std = sqrt(share.variance / 100);
    double rand_var = norm_dist(generator);
    double new_price = share.price * (1 + (share.expectedReturn / 100) + std * rand_var);
    return new_price;
}