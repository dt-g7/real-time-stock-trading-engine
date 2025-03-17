#include <iostream>
#include <thread>
#include <vector>
#include "OrderBook.hpp"
#include "Simulation.hpp"

int main() {
    int ordersPerThread = 50;
    int numThreads = 6;
    std::vector<std::thread> threads;

    // Launch simulation threads to add orders concurrently.
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(simulateOrders, ordersPerThread, i));
    }
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Total orders: " << getOrderCount() << "\n";

    std::vector<Match> matches = matchOrder();
    std::cout << "Matches: " << matches.size() << "\n";
    for (std::size_t i = 0; i < matches.size(); ++i) {
        std::cout << "Match: " << i
                  << " | Buy Order ID: " << matches[i].buy.orderId
                  << ", Ticker: " << matches[i].buy.ticker
                  << ", Price: " << matches[i].buy.price
                  << " matched with Sell Order ID: " << matches[i].sell.orderId
                  << ", Ticker: " << matches[i].sell.ticker
                  << ", Price: " << matches[i].sell.price << "\n";
    }
    return 0;
}
