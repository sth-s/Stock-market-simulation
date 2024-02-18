#include "stock_market.h"
#include "price_generator.h"

// basic initialisation
Stock_Market::Stock_Market(std::unordered_map<std::string, Share> shares_dict, std::vector<Wallet> wallets_list, std::unordered_map<int, Strategy> strategies_dict)
    : _shares(shares_dict), _wallets(wallets_list), _strategies(strategies_dict)
{
    for (const auto &x : _shares)
    {
        _prices_history[x.first] = {};
    }
}

// helper function which calculates new share price affected by demand changes
// P.S. currently doesn't have considerable effects
double calculate_demand(const Share &share, const int demand_diff)
{
    double prevDemandCf = share.reservedNumber / 250000 + 1;
    double newDemandCf = (share.reservedNumber + demand_diff) / 250000 + 1;
    double newPrice = share.price / prevDemandCf * newDemandCf;
    return newPrice;
}

//initialization day, when every strategy buy first shares
void Stock_Market::buy_day()
{
    std::unordered_map<std::string, int> demand_dict = {};
    for (auto &wallet : _wallets)
    {
        Strategy strategy = _strategies[wallet.strategyId];
        double low_risk = strategy.riskBoundaries[0];
        double middle_risk = strategy.riskBoundaries[1];
        unsigned low_risk_num = 0, mid_risk_num = 0, high_risk_num = 0;
        for (const auto &sharename : wallet.shares)
        {
            Share share = _shares[sharename];
            if (share.variance < low_risk)
            {
                low_risk_num++;
            }
            else if (share.variance < middle_risk)
            {
                mid_risk_num++;
            }
            else
            {
                high_risk_num++;
            }
        }
        for (const auto &sharename : wallet.shares)
        {
            Share share = _shares[sharename];
            Strategy strategy = _strategies[wallet.strategyId];
            std::array<double, 3> shares_fractions = strategy.sharesFractions;
            int shares_num;
            if (share.variance < low_risk)
            {
                shares_num = wallet.initialFund * shares_fractions[0] / low_risk_num / share.price;
            }
            else if (share.variance < middle_risk)
            {
                shares_num = wallet.initialFund * shares_fractions[1] / mid_risk_num / share.price;
            }
            else
            {
                shares_num = wallet.initialFund * shares_fractions[2] / high_risk_num / share.price;
            }
            wallet.sharesDistribution[sharename] = shares_num;
            wallet.freeMoney -= shares_num * share.price;
            demand_dict[sharename] += shares_num;
        }
    }
    for (auto &share : _shares)
    {
        _prices_history[share.first].push_back(share.second.price);
        share.second.price = calculate_demand(share.second, demand_dict[share.first]);
        share.second.reservedNumber += demand_dict[share.first];
    }
}

void Stock_Market::trade_day(int daysNum)
{
    if (daysNum != 0)
    {
        std::unordered_map<std::string, int> demand_dict = {};
        for (auto &wallet : _wallets)
        {
            Strategy strategy = _strategies[wallet.strategyId];
            for (const auto &sharename : wallet.shares)
            {
                Share share = _shares[sharename];
                double lastPrice = *_prices_history[sharename].end();
                double expectedPrice = lastPrice * (1 + share.expectedReturn);
                Strategy strategy = _strategies[wallet.strategyId];
                // if price goes too high, we sell it (probably it's overvalued)
                if ((1 + strategy.deviationCf) * expectedPrice > share.price && wallet.sharesDistribution[sharename] > 0)
                {
                    wallet.sharesDistribution[sharename] -= 1;
                    wallet.freeMoney += share.price;
                    demand_dict[sharename] -= 1;
                }
                // if price goes too low, we buy it (probably, it's undervalued)
                else if ((1 - strategy.deviationCf) * expectedPrice < share.price && wallet.freeMoney > share.price)
                {
                    wallet.sharesDistribution[sharename] += 1;
                    wallet.freeMoney -= share.price;
                    demand_dict[sharename] += 1;
                }
            }
        }
        for (auto &share : _shares)
        {
            _prices_history[share.first].push_back(share.second.price);
            share.second.price = calculate_demand(share.second, demand_dict[share.first]);
            share.second.reservedNumber += demand_dict[share.second.name];
            share.second.price = price_generator(share.second);
        }
        trade_day(daysNum - 1);
    }
}

std::unordered_map<std::string, std::vector<double>> Stock_Market::getPricesHistory() const
{
    return _prices_history;
}

std::vector<Wallet> Stock_Market::getWallets() const
{
    return _wallets;
}

std::unordered_map<std::string, Share> Stock_Market::getShares() const
{
    return _shares;
}
