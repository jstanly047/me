#pragma once

#include <me/book/OrderBook.h>
#include <me/book/OrderMatch.h>
#include <gmock/gmock.h>

namespace mock { namespace me { namespace book 
{
    class MockOrderBookCallback : public ::me::book::OrderBookCallback
    {
    public:
        MOCK_METHOD(void, onMatch, (::me::book::OrderMatch*));
    };
}}}