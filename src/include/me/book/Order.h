#pragma once

#include <string>

namespace me { namespace book {
    class OrderMatch;

    class Order{
    public:
        constexpr static bool BUY = true;
        constexpr static bool SELL = false;
        
        Order(const Order& ) = delete;
        Order& operator==(const Order& ) = delete;

        static Order* decode(uint8_t* megBuffer, uint32_t size);

        Order();
        Order(unsigned long long sequence, bool isBuy, const std::string& id, const std::string& symbol, double price, unsigned long long qty);
        std::pair<uint8_t*, uint32_t> encode() const;

        void setSequence(unsigned long long sequence) { m_sequence = sequence; }
        void setId(const std::string& id) { m_id = id; }
        unsigned long long getSequence() const { return m_sequence; }
        double getPrice() const { return m_price; }
        unsigned long long getQty() const { return m_qty; }
        bool isBuy() const { return m_isBuy; }
        bool isSell() const { return !m_isBuy; }
        const std::string& getID() const { return m_id; }
        const std::string& getSymbol() const { return m_symbol; }
        
        void updateQty(const OrderMatch& );
        bool isFullFill() const { return m_qty == 0ull; }

        std::string toString() const;

    private:
        unsigned long long m_sequence = 0ull;
        double m_price = 0.0;
        unsigned long long m_qty = 0ull;
        bool m_isBuy = BUY;
        std::string m_id;
        std::string m_symbol;
    };

}}