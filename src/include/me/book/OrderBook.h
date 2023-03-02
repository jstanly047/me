#pragma once

#include <set>
#include <memory>
#include "Order.h"

namespace me { namespace book {

    class OrderMatch;

    class OrderBookCallback
    {
    public:
        virtual void onMatch(OrderMatch* ) = 0;
    };

    class OrderBook
    {
        struct AskComparator
        {
            bool operator()(const std::unique_ptr<Order>& p1, const std::unique_ptr<Order>& p2) const;
        };

        struct BidComparator
        {
            bool operator()(const std::unique_ptr<Order>& p1, const std::unique_ptr<Order>& p2) const;
        };

    public:
        OrderBook(const OrderBook&) = delete;
        OrderBook operator=(const OrderBook&) = delete;

        OrderBook(const std::string& symbol, OrderBookCallback& callback);
        void submitOrder(std::unique_ptr<Order> order);

        const auto& getBids() const { return m_bids; }
        const auto& getAsks() const { return m_asks; }

    private:
        std::string m_symbol;
        std::set<std::unique_ptr<Order>, BidComparator> m_bids;
        std::set<std::unique_ptr<Order>, AskComparator> m_asks;
        OrderBookCallback& m_callback;
    };
}}