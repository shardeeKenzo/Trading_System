#ifndef TRADINGSYSTEM_H
#define TRADINGSYSTEM_H

#include <utility>

#include "OrderBook.h"
#include "MatchingEngine.h"
#include "Trade.h"
#include "../Server/protocol.h"

struct ProcessResult
{
    OrderID order_id;
    bool accepted_;
    Trades trades_;
};

/**
 * Definition for trading system. It connects order book and matching engine so specific matching engine can properly operate on external order book.
 */
class TradingSystem {
public:
    TradingSystem(OrderBook order_book, const MatchingEngine& matching_engine) : order_book_ {std::move(order_book)},
    matching_engine_ {matching_engine}
    {}

    ProcessResult process(const OrderAdd& add);
    ProcessResult process(const OrderCancel& cancel);
    ProcessResult process(const OrderModify& modify);

    /**
     * Call for the trading system to place an order
     */
    ProcessResult systemPlaceOrder(Price price, Quantity quantity, Side side, OrderType type);
    /**
     * Call for the trading system to modify an order
     */
    ProcessResult systemOrderModify(OrderID order_id, Price price, Quantity quantity, OrderType type);
    /**
     * Call for the trading system to delete an order
     */
    ProcessResult systemDeleteOrder(OrderID order_id);

    const OrderBook& getOrderBook() const;
    const MatchingEngine& getMatchingEngine() const;

private:
    OrderBook order_book_;
    MatchingEngine matching_engine_;

    Side checkOrderSide(const uint8_t& side_);
    OrderType checkOrderType(const uint8_t& type_);
};

#endif //TRADINGSYSTEM_H
