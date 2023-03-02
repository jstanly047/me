#include <me/book/OrderMatch.h>
#include <me/book/Order.h>
#include <OrderMatch.pb.h>
#include <assert.h>

using namespace me::book;

OrderMatch::OrderMatch(const std::string& symbol, const std::string& sellOrderId, 
                     double askPrice, unsigned long long int matchedQty,
                     const std::string& buyOrderId, double bidPrice):
                     m_symbol(symbol),
                     m_sellOrderId(sellOrderId),
                     m_askPrice(askPrice),
                     m_matchedQuantity(matchedQty),
                     m_buyOrderId(buyOrderId),
                     m_bidPrice(bidPrice)
{

}

OrderMatch::OrderMatch(const Order& buyOrder, const Order& sellOrder)
{
    assert(buyOrder.isBuy() && sellOrder.isSell());
    m_symbol = sellOrder.getSymbol();
    m_sellOrderId = sellOrder.getID();
    m_askPrice = sellOrder.getPrice();
    m_matchedQuantity = std::min(buyOrder.getQty(), sellOrder.getQty());
    m_buyOrderId = buyOrder.getID();
    m_bidPrice = buyOrder.getPrice();
}

OrderMatch* OrderMatch::decode(uint8_t* megBuffer, uint32_t size)
{
    message::OrderMatch orderMatchMsg;
    orderMatchMsg.ParseFromArray(megBuffer, size);
    return new OrderMatch(orderMatchMsg.symbol(), orderMatchMsg.sellorderid(), orderMatchMsg.askprice(),
                orderMatchMsg.matchedquantity(), orderMatchMsg.buyorderid(), orderMatchMsg.bidprice());
}

std::pair<uint8_t*, uint32_t> OrderMatch::encode() const
{
    message::OrderMatch orderMatch;
    orderMatch.set_symbol(m_symbol);
    orderMatch.set_sellorderid(m_sellOrderId);
    orderMatch.set_askprice(m_askPrice);
    orderMatch.set_matchedquantity(m_matchedQuantity);
    orderMatch.set_buyorderid(m_buyOrderId);
    orderMatch.set_bidprice(m_bidPrice);
    
    uint32_t msgBytesSize =  (uint32_t) orderMatch.ByteSizeLong();
    uint8_t* buffer = new uint8_t[msgBytesSize];
    orderMatch.SerializeToArray(buffer, msgBytesSize);
    return {buffer, msgBytesSize};
}

std::string OrderMatch::toString() const
{
    std::string orderStr = "MATCH(";
    orderStr.append(m_symbol);
    orderStr.append(", ");
    orderStr.append(m_sellOrderId);
    orderStr.append(", ");
    orderStr.append(std::to_string(m_askPrice));
    orderStr.append(", ");
    orderStr.append(std::to_string(m_matchedQuantity));
    orderStr.append(", ");
    orderStr.append(m_buyOrderId);
    orderStr.append(", ");
    orderStr.append(std::to_string(m_bidPrice));
    orderStr.append(")");
    return orderStr;
}
