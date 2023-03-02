#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <Order.pb.h>

using namespace me::book;

Order::Order(){}

Order::Order(unsigned long long sequence, bool isBuy, const std::string& id, const std::string& symbol, double price, unsigned long long qty):
            m_sequence(sequence),
            m_price(price),
            m_qty(qty),
            m_isBuy(isBuy),
            m_id(id),
            m_symbol(symbol)
{
    
}


Order* Order::decode(uint8_t* megBuffer, uint32_t size)
{
    message::Order orderMsg;
    orderMsg.ParseFromArray(megBuffer, size);
    return new Order(orderMsg.sequence(), orderMsg.isbuy(), orderMsg.id(), orderMsg.symbol(), orderMsg.price(), orderMsg.qty());
}

std::pair<uint8_t*, uint32_t> Order::encode() const
{
    message::Order order;
    order.set_sequence(m_sequence);
    order.set_price(m_price);
    order.set_qty(m_qty);
    order.set_isbuy(m_isBuy);
    order.set_id(m_id);
    order.set_symbol(m_symbol);
    
    uint32_t msgBytesSize =  (uint32_t) order.ByteSizeLong();
    uint8_t* buffer = new uint8_t[msgBytesSize];
    order.SerializeToArray(buffer, msgBytesSize);
    return {buffer, msgBytesSize};
}

void Order::updateQty(const OrderMatch& orderMatch)
{ 
    m_qty -= orderMatch.getMatchedQuantity();
}

std::string Order::toString() const
{
    std::string orderStr = "ORDER(";
    orderStr.append(std::to_string(m_sequence));
    orderStr.append(", ");
    orderStr.append(m_id);
    orderStr.append(", ");
    orderStr.append(m_symbol);
    orderStr.append(", ");
    orderStr.append(std::to_string(m_isBuy));
    orderStr.append(", ");
    orderStr.append(std::to_string(m_qty));
    orderStr.append(", ");
    orderStr.append(std::to_string(m_price));
    orderStr.append(")");
    return orderStr;
}
