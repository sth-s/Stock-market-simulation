#ifndef INPUT_PARSER
#define INPUT_PARSER
#include <string>
#include <vector>
#include <unordered_map>

/*
 * Storage structure for info about share
 */
struct Share
{
    std::string name;
    double variance;
    double priceChange;
    double expectedReturn;
    double initialPrice;
    double price;
    int totalNumber = 1000000;
    int reservedNumber = 0;
};

struct Wallet
{
    std::string name;
    double initialFund;
    double freeMoney;
    std::vector<std::string> shares;
    int strategyId;
    std::unordered_map<std::string, int> sharesDistribution = {}; // deduced from strategy and shares
};

struct Strategy
{
    int id;
    std::array<double, 2> riskBoundaries;
    std::array<double, 3> sharesFractions;
    double deviationCf;
};

/*
 * Storage structure for input info
 */
struct Data
{
    std::unordered_map<std::string, Share> shares;
    std::vector<Wallet> wallets;
    std::unordered_map<int, Strategy> strategies;
};
/*
 * Secondary function for processing input.json into `PortfolioInfo` structure.
 */
Data readDataFromJson(const std::string &filename);

#endif