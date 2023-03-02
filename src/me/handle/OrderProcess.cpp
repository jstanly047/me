#include <me/handle/OrderProcess.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <iostream>

using namespace me::handle;


void OrderProcess::onMsg(void* msg)
{
    auto order = reinterpret_cast<me::book::Order*>(msg);

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