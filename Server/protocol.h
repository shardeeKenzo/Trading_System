#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>

#pragma pack(push, 1)
struct Header
{
    uint8_t command; // 1 - place, 2 - modify, 3- delete
};

struct OrderAdd : public Header
{
    double price;
    uint32_t quantity;
    uint8_t side; // 0 - buy, 1 - sell
    uint8_t orderType; // 0 - GoodTillCancel, 2 - FOK, 3 - IOC
};

struct OrderCancel : public Header
{
    uint32_t order_id; // for modify/delete
};

struct OrderModify : public Header
{
    uint32_t order_id; // for modify/delete
    double price;
    uint32_t quantity;
    uint8_t side; // 0 - buy, 1 - sell
    uint8_t orderType; // 0 - GoodTillCancel, 2 - FOK, 3 - IOC
};

struct OrderResponse
{
    uint8_t status; // 0 - ok, 1 - error
    uint32_t order_id;
};
#pragma pack(pop)


#endif //PROTOCOL_H
