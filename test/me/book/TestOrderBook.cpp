#include <me/book/OrderBook.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <gtest/gtest.h>

class TestOrderBook : public testing::Test
{
protected:
  TestOrderBook() { }

  ~TestOrderBook() override
  {
  }
};