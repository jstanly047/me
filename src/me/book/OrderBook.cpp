#include <me/book/OrderBook.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <memory>

using namespace me::book;

namespace
{
    inline bool isBuyOrderMatch(const Order& sellOrder, const Order& buyOrder)
    {
        if (buyOrder.getQty() != 0ull && buyOrder.getPrice() >= sellOrder.getPrice())
        {
            return true;
        }

        return false;
    }

    inline bool isSellOrderMatch(const Order& buyOrder, const Order& sellOrder)
    {
        if (sellOrder.getQty() != 0ull && sellOrder.getPrice() <= buyOrder.getPrice())
        {
            return true;
        }

        return false;
    }
}

bool OrderBook::AskComparator::operator()(const std::unique_ptr<Order>& p1, const std::unique_ptr<Order>& p2) const
{
    if (p1->getPrice() == p2->getPrice())
    {
        return p1->getSequence() < p2->getSequence();
    }

    return p1->getPrice() < p2->getPrice();
}

bool OrderBook::BidComparator::operator()(const std::unique_ptr<Order>& p1, const std::unique_ptr<Order>& p2) const
{
    if (p1->getPrice() == p2->getPrice())
    {
        return p1->getSequence() < p2->getSequence();
    }

    return p1->getPrice() > p2->getPrice();
}

OrderBook::OrderBook(const std::string& symbol, OrderBookCallback& callback):m_symbol(symbol), m_callback(callback)
{
    
}

void OrderBook::submitOrder(std::unique_ptr<Order> order)
{
    if (order->isBuy())
    {
        for (auto askItr = m_asks.begin(); askItr != m_asks.end(); )
        {
            if (isBuyOrderMatch(**askItr, *order))
            {
                auto orderMatch = new OrderMatch(*order, **askItr);
                order->updateQty(*orderMatch);
                (*askItr)->updateQty(*orderMatch);
                m_callback.onMatch(orderMatch);

                if ((*askItr)->isFullFill())
                {
                    askItr = m_asks.erase(askItr);
                    continue;
                }
            }
            else
            {
                break;
            }

            askItr++;
        }

        if (order->isFullFill() == false)
        {
            m_bids.emplace(std::move(order));
        }
    }
    else
    {
        for (auto bidItr = m_bids.begin(); bidItr != m_bids.end(); )
        {
            if (isSellOrderMatch(**bidItr, *order))
            {
                auto orderMatch = new OrderMatch(**bidItr, *order);
                order->updateQty(*orderMatch);
                (*bidItr)->updateQty(*orderMatch);
                m_callback.onMatch(orderMatch);

                if ((*bidItr)->isFullFill())
                {
                    bidItr = m_bids.erase(bidItr);
                    continue;
                }
            }
            else
            {
                break;
            }

            bidItr++;
        }

        if (order->isFullFill() == false)
        {
            m_asks.emplace(std::move(order));
        }
    }
}

//template class me::book::OrderBook<std::unique_ptr<Order>;