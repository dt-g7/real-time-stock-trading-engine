#include <random>
#include <chrono>
#include <thread>
#include "Simulation.hpp"
#include "OrderBook.hpp"

void simulateOrders(int numOrders, int threadId) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> buy_or_sell_distribution(0, 1);
    std::uniform_int_distribution<> ticker_distribution(0, NUM_TICKERS - 1);
    std::uniform_int_distribution<> quantity_distribution(1, 1000);
    std::uniform_real_distribution<> price_distribution(1.0, 1000.0);
    std::uniform_int_distribution<> latency_distribution(5, 50); // Random latency delay

    for (int i = 0; i < numOrders; ++i) {
        bool isBuy = (buy_or_sell_distribution(gen) == 1);
        int ticker = ticker_distribution(gen);
        int quantity = quantity_distribution(gen);
        double price = price_distribution(gen);

        addOrder(isBuy, ticker, quantity, price);

        int sleepTime = latency_distribution(gen);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}
