#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "helper/price_generator.h"
#include "helper/input_parser.h"
#include "helper/stock_market.h"

double custom_round(double x, int n) {
    return round(x * pow(10, n)) / pow(10, n);
}

int main() {
    nlohmann::json jsonResult;
    std::ofstream outputFile("../output.json");

    //initialization of stock_market;
    Data data = readDataFromJson("input.json");
    Stock_Market market(data.shares, data.wallets, data.strategies);

    market.buy_day();

    std::cout << "Number of simulation days" << std::endl;
    int n;
    std::cin >> n;

    market.trade_day(n);

    //output.json generation
    //Price change
    std::unordered_map<std::string, std::vector<double>> pricesHistory = market.getPricesHistory();
    for (auto &entry: pricesHistory) {
        for (auto &x: entry.second) {
            x = round(x * 100) / 100;
        }
        jsonResult["Price change"][entry.first] = entry.second;
    }

    //Wallets Summary
    std::unordered_map<std::string, Share> shares = market.getShares();
    for (const auto &wallet: market.getWallets()) {

        //Wallet
        const double currentFund = wallet.freeMoney
                                   + std::accumulate(wallet.sharesDistribution.begin(), wallet.sharesDistribution.end(),
                                                     0.0,
                                                     [&shares](double total, const std::pair<std::string, double> &pair) {
                                                         return total + pair.second * shares[pair.first].price;
                                                     });
        jsonResult["Wallets Summary"][wallet.name]["Income"]["Current fund"] = custom_round(currentFund, 2);
        jsonResult["Wallets Summary"][wallet.name]["Income"]["Income"] = custom_round(currentFund - wallet.initialFund,
                                                                                      2);
        jsonResult["Wallets Summary"][wallet.name]["Income"]["Current free money"] = custom_round(wallet.freeMoney, 2);

        //Shares
        for (const auto &share: wallet.sharesDistribution) {
            jsonResult["Wallets Summary"][wallet.name]["Shares"][share.first]["Price"] = custom_round(
                    shares[share.first].price, 2);
            jsonResult["Wallets Summary"][wallet.name]["Shares"][share.first]["Invested in share"] = custom_round(
                    share.second * shares[share.first].price, 2);
            jsonResult["Wallets Summary"][wallet.name]["Shares"][share.first]["Price difference"] = custom_round(
                    shares[share.first].price - shares[share.first].initialPrice, 2);
        }
        jsonResult["Wallets Summary"][wallet.name]["Shares"]["Non invested"]["Price"] = 0;
        jsonResult["Wallets Summary"][wallet.name]["Shares"]["Non invested"]["Invested in share"] = custom_round(
                wallet.freeMoney, 2);
        jsonResult["Wallets Summary"][wallet.name]["Shares"]["Non invested"]["Price difference"] = 0;
    }

    //Output
    outputFile << std::setw(4) << jsonResult << std::endl;
    outputFile.close();

    //output.json path search
    std::filesystem::path outputPath = std::filesystem::current_path().parent_path() / "output.json";

    std::cout << "simulation completed\n" <<
              "Complete information on: " << outputPath << std::endl;
    return 0;
}