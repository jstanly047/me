#include <me/book/Order.h>
#include <me/book/OrderMatch.h>

using namespace me::book;

Order::Order(){}

Order::Order(unsigned long long sequence, bool isBuy, const std::string& id, double price, unsigned long long qty):
            m_sequence(sequence),
            m_price(price),
            m_qty(qty),
            m_isBuy(isBuy),
            m_id(id)
{
    
}

// assume this is Sell order
bool Order::isBuyOrderMatch(const Order& buyOrder) const
{
    if (buyOrder.getQty() != 0ull && buyOrder.getPrice() >= m_price)
    {
        return true;
    }

    return false;
}

// assume this is Buy order
bool Order::isSellOrderMatch(const Order& sellOrder) const
{
    if (sellOrder.getQty() != 0ull && sellOrder.getPrice() <= m_price)
    {
        return true;
    }

    return false;
}

void Order::updateQty(const OrderMatch& orderMatch)
{ 
    m_qty -= orderMatch.getMatchedQuantity();
}