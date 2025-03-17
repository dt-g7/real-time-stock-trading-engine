#include <iostream>
#include <atomic>
#include <mutex>
#include <vector>
#include "OrderBook.hpp"

// Global variables for order management
std::atomic<int> orderCount(0);
Order orderBook[MAX_ORDERS];
std::mutex coutMutex;

// Order default constructor
Order::Order() : price(0.0), orderId(0), quantity(0), buy_or_sell(false), ticker(0) {}

// Order parameterized constructor
Order::Order(int orderId, bool isBuy, int ticker, int quantity, double price)
    : price(price), orderId(orderId), quantity(quantity), buy_or_sell(isBuy), ticker(ticker) {}

void addOrder(bool isBuy, int ticker, int quantity, double price) {
    int index = orderCount.fetch_add(1, std::memory_order_relaxed);
    if (index >= MAX_ORDERS) { return; }
    orderBook[index] = Order(index, isBuy, ticker, quantity, price);

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Add Order ID: " << orderBook[index].orderId
                  << " | Buy or Sell: " << (orderBook[index].buy_or_sell ? "Buy" : "Sell")
                  << " | Ticker: " << orderBook[index].ticker
                  << " | Quantity: " << orderBook[index].quantity
                  << " | Price: " << orderBook[index].price << "\n";
    }
}

int getOrderCount() {
    return orderCount.load(std::memory_order_relaxed);
}

std::vector<Match> matchOrder() {
    double lowestSellPrice[NUM_TICKERS];
    int lowestSellIndex[NUM_TICKERS];
    for (int c = 0; c < NUM_TICKERS; ++c) {
        lowestSellPrice[c] = 1e9;
        lowestSellIndex[c] = -1;
    }

    int totalOrders = getOrderCount();

    // First pass: find the lowest sell price per ticker
    for (int i = 0; i < totalOrders; ++i) {
        Order order = orderBook[i];
        if (!order.buy_or_sell) { // Sell order
            int t = order.ticker;
            if (order.price < lowestSellPrice[t]) {
                lowestSellPrice[t] = order.price;
                lowestSellIndex[t] = i;
            }
        }
    }

    std::vector<Match> matches;
    // Second pass: match buy orders with the corresponding sell order
    for (int i = 0; i < totalOrders; ++i) {
        Order buyOrder = orderBook[i];
        if (buyOrder.buy_or_sell) { // Buy order
            int t = buyOrder.ticker;
            if (lowestSellIndex[t] != -1 && buyOrder.price >= lowestSellPrice[t]) {
                Match m = { buyOrder, orderBook[lowestSellIndex[t]] };
                matches.push_back(m);
            }
        }
    }
    return matches;
}
