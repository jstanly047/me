#pragma once

#include <set>
#include "Order.h"

namespace me { namespace book {

    class OrderMatch;

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

        class OrderBookCallback
        {
        public:
            virtual void onMatch(OrderMatch* ) = 0;
        };

        OrderBook(const OrderBook&) = delete;
        OrderBook operator=(const OrderBook&) = delete;

        OrderBook(const std::string& symbol, OrderBookCallback& callback);
        void submitOrder(OrderPtrType order);

    private:
        std::string m_symbol;
        std::set<OrderPtrType, BidComparator> m_bids;
        std::set<OrderPtrType, AskComparator> m_asks;
        OrderBookCallback& m_callback;
    };
}}