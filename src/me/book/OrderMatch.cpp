#include <me/book/OrderMatch.h>
#include <me/book/Order.h>
#include <assert.h>

using namespace me::book;

OrderMatch::OrderMatch(const Order& buyOrder, const Order& sellOrder)
{
    assert(buyOrder.isBuy() && sellOrder.isSell());

    m_sellOrderId = sellOrder.getID();
    m_askPrice = sellOrder.getPrice();
    m_matchedQuantity = std::min(buyOrder.getQty(), sellOrder.getQty());
    m_buyOrderId = buyOrder.getID();
    m_bidPrice = buyOrder.getPrice();
}
