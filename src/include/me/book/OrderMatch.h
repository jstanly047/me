#pragma once

#include <string>

namespace me { namespace book {
    class Order;

    class OrderMatch{
    public:
        OrderMatch(const OrderMatch& ) = delete;
        OrderMatch& operator==(const OrderMatch& ) = delete;
        static OrderMatch* decode(uint8_t* megBuffer, uint32_t size);

        OrderMatch(const std::string& symbol, const std::string& sellOrderId, double askPrice, 
                        unsigned long long int matchedQty, const std::string& buyOrderId, 
                        double bidPrice);
        OrderMatch(const Order& buyOrder, const Order& sellOrder);
        std::pair<uint8_t*, uint32_t> encode() const;

        const std::string& getSymbol() const { return m_symbol; }
        const std::string& getSellOrderID() const { return m_sellOrderId; }
        double getAskPrice() const { return m_askPrice; }
        unsigned long long getMatchedQuantity() const { return m_matchedQuantity; }
        const std::string& getBuyOrderID() const { return m_buyOrderId; }
        double getBidPrice() const { return m_bidPrice; }
        std::string toString() const;

    private:
        std::string m_symbol;
        std::string m_sellOrderId;
        double m_askPrice;
        unsigned long long int m_matchedQuantity;
        std::string m_buyOrderId;
        double m_bidPrice;
    };

}}