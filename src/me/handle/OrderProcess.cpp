#include <me/handle/OrderProcess.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <iostream>

using namespace me::handle;


void OrderProcess::onMsg(void* msg)
{
    auto order = reinterpret_cast<me::book::Order*>(msg);
    

    if (order->getSequence() == 1)
    {
        m_totalTime.begin();
    }
    else if (order->getID() == "END2")
    {
        m_totalTime.end();
        std::cout << m_totalTime.getStatInMilliSec() << std::endl;
    }

    auto orderBook = m_orderBooks[order->getSymbol()];

    if (orderBook == nullptr)
    {
        orderBook = new me::book::OrderBook(order->getSymbol(), *this);
        m_orderBooks[order->getSymbol()] = orderBook;
    }

    orderBook->submitOrder(std::unique_ptr<me::book::Order>(order));
}

void OrderProcess::onMatch(me::book::OrderMatch *orderMatch)
{   
    sendMsgToOutput(orderMatch);
}