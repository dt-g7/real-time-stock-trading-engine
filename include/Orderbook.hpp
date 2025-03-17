#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <vector>

const int MAX_ORDERS = 10000;
const int NUM_TICKERS = 1024;

struct Order {
    double price;
    int orderId;
    int quantity;
    bool buy_or_sell;
    int ticker;

    Order();
    Order(int orderId, bool isBuy, int ticker, int quantity, double price);
};

struct Match {
    Order buy;
    Order sell;
};

void addOrder(bool isBuy, int ticker, int quantity, double price);
int getOrderCount();
std::vector<Match> matchOrder();

#endif // ORDERBOOK_HPP
