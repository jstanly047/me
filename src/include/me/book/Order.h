#pragma once

#include <string>

namespace me { namespace book {
    class OrderMatch;

    class Order{
    public:
        Order(const Order& ) = delete;
        Order& operator==(const Order& ) = delete;

        Order();

        unsigned long long getSequence() const { return m_sequence; }
        double getPrice() const { return m_price; }
        unsigned long long getQty() const { return m_qty; }
        bool isBuy() const { return m_isBuy; }
        bool isSell() const { return !m_isBuy; }
        const std::string& getID() const { return m_id; }
        
        // assume this is Sell order
        bool isBuyOrderMatch(const Order& buyOrder) const;
        // assume this is Buy order
        bool isSellOrderMatch(const Order& sellOrder) const;
        void updateQty(const OrderMatch& );
        bool isFullFill() const { return m_qty == 0ull; }

    private:
        unsigned long long m_sequence = 0ull;
        double m_price = 0.0;
        unsigned long long m_qty = 0ull;
        bool m_isBuy = false;
        std::string m_id;
    };

}}