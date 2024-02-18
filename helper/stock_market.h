#ifndef STOCK_MARKET
#define STOCK_MARKET
#include "input_parser.h"
#include <unordered_map>
#include <string>
#include <vector>

/**
 * Class responsible for all stock market actions
 * and all needed info storing.
 **/
class Stock_Market
{
private:
    // dictionary of (sharename, share) pairs
    std::unordered_map<std::string, Share> _shares;
    // list of wallets
    std::vector<Wallet> _wallets;
    // dictionary of (strategyId, strategy) pairs
    std::unordered_map<int, Strategy> _strategies;
    // dictionary of (sharename, list of share price history) pairs
    std::unordered_map<std::string, std::vector<double>> _prices_history;

public:
    Stock_Market(std::unordered_map<std::string, Share>, std::vector<Wallet>, std::unordered_map<int, Strategy>);

    // initial day, when all wallets buy their first shares corresponding to their strategies
    void buy_day();

    /**
     * Simulation of trade day with 2 phases:
     * 1. All wallets buy/sell shares corresponding to their strategies
     * 2. New share price calculates firstly considering changes of demand and
     * secondly affected by new day
     **/
    void trade_day(int);

    // return _prices_history for printing goals
    std::unordered_map<std::string, std::vector<double>> getPricesHistory() const;

    // return _wallets for printing goals
    std::vector<Wallet> getWallets() const;

    // return _shares for printing goals
    std::unordered_map<std::string, Share> getShares() const;
};

#endif