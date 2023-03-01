#pragma once

#include <set>
#include "Order.h"

namespace me { namespace book {

    class OrderMatch;

    class OrderBookCallback
    {
    public:
        virtual void onMatch(OrderMatch* ) = 0;
    };

    template<typename OrderPtrType>
    class OrderBook
    {
        struct AskComparator
        {
            bool operator()(const OrderPtrType& p1, const OrderPtrType& p2) const;
        };

        struct BidComparator
        {
            bool operator()(const OrderPtrType& p1, const OrderPtrType& p2) const;
        };

    public:
        OrderBook(const OrderBook&) = delete;
        OrderBook operator=(const OrderBook&) = delete;

        OrderBook(const std::string& symbol, OrderBookCallback& callback);
        void submitOrder(OrderPtrType order);

        const auto& getBids() const { return m_bids; }
        const auto& getAsks() const { return m_asks; }

    private:
        std::string m_symbol;
        std::set<OrderPtrType, BidComparator> m_bids;
        std::set<OrderPtrType, AskComparator> m_asks;
        OrderBookCallback& m_callback;
    };
}}