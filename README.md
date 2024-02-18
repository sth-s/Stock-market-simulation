## A few words about the project objectives

The goal of this project is to simulate the performance of certain portfolios in a simulated stock market. The whole project is divided into 2 simulations: the market simulation and the investor simulation. Later, it will be possible to create interactions between the simulations of the investors and the simulations of the market, therefore creating cause and effect relationships.


The market simulation will involve the creation of a stochastic process through a mathematical model, in which the performance of each share is represented by a pseudo-random function that evolves over time. The trend of the shares will be characterized by statistical parameters that will determine their volatility over time.

The investor simulation will involve the management of many stock wallets, with each wallet being able to follow investment 'strategies'. In the first phase, the market shares will be assigned at the beginning of the simulation and will stay in the assigned wallets until the end of the simulation. Later, the wallets will be able to buy and sell market shares during the simulation by following the selected 'strategies'.

## Input structure

**Share_Info**
A list of information about shares.
- **name**: The name of the company.
- **variance**: Variance (a measure of risk) of the share price.
- **price_change**: Change in the share price (daily).
- **expected_return**: Expected return of the share.
- **price**: Current price of the share.

**Wallets_Info**
Information about portfolios.
- **name**: Name of the portfolio.
- **fund**: Amount of funds in the portfolio.
- **shares**: List of shares included in the portfolio.
- **strategy_id**: Identifier for the portfolio management strategy.

**Strategies_Info**
Information about portfolio management strategies.
- **id**: Identifier of the strategy.
- **name**: Name of the strategy.
- **shares_fractions**: Proportions of shares in the portfolio.
- **risk_boundaries**: Risk boundaries for the strategy (low/mid and mid/high).
- **deviation_cf**: Deviation coefficient for the strategy.

## Project run

1. Firsly, some setup:

```
sudo apt-get install nlohmann-json3-dev
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make
./simulation
```

2. Then you will be asked to define the number of days in simulation:
you can choose whatever you want, our suggestions to try `100` and `1000`


3. You can also find the result in the output.json file and change some values in input.json if you want, such as adding your strategy to the Strategies section and adding new wallets with the choice of indicating the strategy number (the executable input is in the project root).


4. In the "example_cases" directory you can find an example of the expected input and an example 10 day simulation result on input.

