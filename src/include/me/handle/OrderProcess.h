#pragma once

#include <me/thread/WorkerThread.h>
#include <me/book/OrderBook.h>
#include <unordered_map>
#include <string>
#include <me/utility/TimerClock.h>

namespace me { namespace book {
    class Order;
}}

namespace me { namespace handle {
    class OrderProcess : public me::thread::WorkerThreadCallback, public me::book::OrderBookCallback
    {
    private:
        void onMsg(void* msg) final override;
        void onMatch(me::book::OrderMatch* orderMatch) final override;
        std::unordered_map<std::string, me::book::OrderBook*> m_orderBooks;

        me::utility::TimerClock m_totalTime = me::utility::TimerClock("Worker Received Orders");
        unsigned long long int m_count = 0;
    };
}}