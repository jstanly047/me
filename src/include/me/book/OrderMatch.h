#pragma once

#include <string>

namespace me { namespace book {
    class Order;

    class OrderMatch{
    public:
        OrderMatch(const OrderMatch& ) = delete;
        OrderMatch& operator==(const OrderMatch& ) = delete;
        OrderMatch(const Order& buyOrder, const Order& sellOrder);

        const std::string& getSellOrderID() const { return m_sellOrderId; }
        double getAskPrice() const { return m_askPrice; }
        unsigned long long getMatchedQuantity() const { return m_matchedQuantity; }
        const std::string& getBuyOrderID() const { return m_buyOrderId; }
        double getBidPrice() const { return m_bidPrice; }

    private:
        std::string m_sellOrderId;
        double m_askPrice;
        unsigned long long int m_matchedQuantity;
        std::string m_buyOrderId;
        double m_bidPrice;
    };

}}