#include "TradingSystem.h"

Side TradingSystem::checkOrderSide(const uint8_t& side_)
{
    switch (side_)
    {
    case 1:
        return Side::Buy;
        break;

    case 2:
        return Side::Sell;
        break;

    default:
        return Side::Undefined;
        break; // TO-DO: define behavior for invalid side
    }

    return Side::Undefined;
}
OrderType TradingSystem::checkOrderType(const uint8_t& type_)
{
    switch (type_)
    {
    case 1:
        return OrderType::GoodTillCancel;
        break;

    case 2:
        return OrderType::FillOrKill;
        break;

    case 3:
        return OrderType::ImmediateOrCancel;
        break;

    default:
        return OrderType::Undefined;
        break; // same TO-DO for invalid OrderType
    }

    return OrderType::Undefined;
}

ProcessResult TradingSystem::process(const OrderAdd& add)
{
    return systemPlaceOrder(add.price, add.quantity, checkOrderSide(add.side), checkOrderType(add.orderType));
}
ProcessResult TradingSystem::process(const OrderCancel& cancel)
{
    return systemDeleteOrder(cancel.order_id);
}
ProcessResult TradingSystem::process(const OrderModify& modify)
{
    return systemOrderModify(modify.order_id, modify.price, modify.quantity, checkOrderType(modify.orderType));
}

ProcessResult TradingSystem::systemPlaceOrder(const Price price, const Quantity quantity, const Side side, const OrderType type)
{
    if (type == OrderType::FillOrKill)
    {
        if (!matching_engine_.canMatch(order_book_, side, price, type, quantity))
        {
            return { .order_id = 0, .accepted_ = false, .trades_ = { }};
        }
    }
    const auto order_id = order_book_.placeOrder(price, quantity, side, type);

    Trades matchTrades = matching_engine_.matchOrders(order_book_);
    return {.order_id = order_id, .accepted_ = true, .trades_ = matchTrades};
}
ProcessResult TradingSystem::systemOrderModify(const OrderID order_id, const Price price, const Quantity quantity, const OrderType type)
{
    auto id = order_book_.modifyOrder(order_id, price, quantity, type);

    Trades matchTrades = matching_engine_.matchOrders(order_book_);
    return {.order_id = id, .accepted_ = true,  .trades_ = matchTrades};
}
ProcessResult TradingSystem::systemDeleteOrder(const OrderID order_id)
{
    order_book_.deleteOrder(order_id);

    Trades matchTrades = matching_engine_.matchOrders(order_book_);
    return {.order_id = order_id, .accepted_ = true, .trades_ = matchTrades };
}

const OrderBook& TradingSystem::getOrderBook() const { return order_book_; }
const MatchingEngine& TradingSystem::getMatchingEngine() const { return matching_engine_; }