#include <me/book/OrderBook.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <memory>

using namespace me::book;

template<typename OrderPtrType>
bool OrderBook<OrderPtrType>::AskComparator::operator()(const OrderPtrType& p1, const OrderPtrType& p2) const
{
    if (p1->getPrice() == p2->getPrice())
    {
        return p1->getSequence() < p2->getSequence();
    }

    return p1->getPrice() < p2->getPrice();
}

template<typename OrderPtrType>
bool OrderBook<OrderPtrType>::BidComparator::operator()(const OrderPtrType& p1, const OrderPtrType& p2) const
{
    if (p1->getPrice() == p2->getPrice())
    {
        return p1->getSequence() < p2->getSequence();
    }

    return p1->getPrice() > p2->getPrice();
}

template<typename OrderPtrType>
OrderBook<OrderPtrType>::OrderBook(const std::string& symbol, OrderBookCallback& callback):m_symbol(symbol), m_callback(callback)
{
    
}

template<typename OrderPtrType>
void OrderBook<OrderPtrType>::submitOrder(OrderPtrType order)
{
    if (order->isBuy())
    {
        for (auto askItr = m_asks.begin(); askItr != m_asks.end(); )
        {
            if ((*askItr)->isBuyOrderMatch(*order))
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
            if ((*bidItr)->isSellOrderMatch(*order))
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

template class me::book::OrderBook<me::book::Order*>;
template class me::book::OrderBook<std::unique_ptr<Order>>;