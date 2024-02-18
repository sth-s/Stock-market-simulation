#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "input_parser.h"

struct Share;

struct Data;

Data readDataFromJson(const std::string &filename) {
    Data data;

    std::ifstream file("../" + filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    nlohmann::json j_data;
    file >> j_data;

    try {
        //Init shares
        const nlohmann::json &shareInfoArray = j_data["Share_Info"];
        for (const auto &share: shareInfoArray) {
            Share shareInfo;
            shareInfo.name = share["name"].get<std::string>();
            shareInfo.variance = share["variance"].get<double>();
            shareInfo.priceChange = share["price_change"].get<double>();
            shareInfo.expectedReturn = share["expected_return"].get<double>();
            shareInfo.initialPrice = share["price"].get<double>();
            shareInfo.price = shareInfo.initialPrice;

            data.shares[shareInfo.name] = shareInfo;
        }

        //Init wallets
        const nlohmann::json &walletsInfoArray = j_data["Wallets_Info"];
        for (const auto &wallet: walletsInfoArray) {
            Wallet walletInfo;
            walletInfo.name = wallet["name"].get<std::string>();
            walletInfo.initialFund = wallet["fund"].get<double>();
            walletInfo.freeMoney = walletInfo.initialFund;
            walletInfo.shares = wallet["shares"].get<std::vector<std::string>>();
            walletInfo.strategyId = wallet["strategy_id"].get<int>();

            data.wallets.push_back(walletInfo);
        }

        //Init strategies
        const nlohmann::json &strategiesInfoArray = j_data["Strategies_Info"];
        for (const auto &strategy: strategiesInfoArray) {
            Strategy strategyInfo;

            strategyInfo.id = strategy["id"].get<int>();

            //Proof of valid shares_fractions
            auto sharesFractions = strategy["shares_fractions"];
            if (sharesFractions.size() != 3)
                throw std::runtime_error("Wrong strategy length");
            if (sharesFractions[0].get<double>() + sharesFractions[1].get<double>() + sharesFractions[2].get<double>() >
                1)
                throw std::runtime_error("Wrong strategy value");
            for (int i = 0; i < 3; ++i) {
                strategyInfo.sharesFractions[i] = sharesFractions[i].get<double>();
            }

            //Proof of valid risk_boundaries
            auto riskBoundaries = strategy["risk_boundaries"];
            if (riskBoundaries.size() != 2)
                throw std::runtime_error("Wrong strategy length");
            if (riskBoundaries[0].get<double>() > riskBoundaries[1].get<double>())
                throw std::runtime_error("Wrong strategy value");
            for (int i = 0; i < 2; ++i) {
                strategyInfo.riskBoundaries[i] = riskBoundaries[i].get<double>();
            }

            strategyInfo.deviationCf = strategy["deviation_cf"].get<double>();

            data.strategies[strategyInfo.id] = strategyInfo;
        }
    }
    catch (const nlohmann::json::exception &e) {
        throw std::runtime_error("Error reading JSON: " + std::string(e.what()));
    }

    return data;
}