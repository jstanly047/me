#include <me/book/OrderBook.h>
#include <me/book/Order.h>
#include <me/book/OrderMatch.h>
#include <mock/me/book/MockOrderBookCallback.h>
#include <vector>
#include <gtest/gtest.h>

using::testing::_;

class TestOrderBook : public testing::Test
{
protected:
    void SetUp() override final
    {
        m_orderBook = new me::book::OrderBook<::me::book::Order*>("IBM", m_mockCallback);
        m_sequence = 0;
    }

    me::book::Order* addOrder(bool isBuy, double price, unsigned long long qty)
    {
        m_sequence++;
        std::string id = "id_" + std::to_string(m_sequence);
        auto order = new me::book::Order(m_sequence, isBuy, id, price, qty);
        m_orderBook->submitOrder(order);
        return order;
    }

    mock::me::book::MockOrderBookCallback m_mockCallback;
    me::book::OrderBook<::me::book::Order*>* m_orderBook = nullptr;
    long long int m_sequence = 0;
};

TEST_F(TestOrderBook, checkAskOrder)
{
    auto order1 = addOrder(me::book::Order::SELL, 10.0, 15ull);
    auto order2 = addOrder(me::book::Order::SELL, 15.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order1, order2}));
    auto order3 = addOrder(me::book::Order::SELL, 9.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order3, order1, order2}));
    auto order4 = addOrder(me::book::Order::SELL, 10.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order3, order1, order4, order2}));
    auto order5 = addOrder(me::book::Order::SELL, 20.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order3, order1, order4, order2, order5}));
    EXPECT_TRUE(m_orderBook->getBids().empty());
}

TEST_F(TestOrderBook, checkBidOrder)
{
    auto order1 = addOrder(me::book::Order::BUY, 15.0, 15ull);
    auto order2 = addOrder(me::book::Order::BUY, 10.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order1, order2}));
    auto order3 = addOrder(me::book::Order::BUY, 20.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order3, order1, order2}));
    auto order4 = addOrder(me::book::Order::BUY, 15.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order3, order1, order4, order2}));
    auto order5 = addOrder(me::book::Order::BUY, 9.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order3, order1, order4, order2, order5}));
    EXPECT_TRUE(m_orderBook->getAsks().empty());
}

TEST_F(TestOrderBook, noMatchForBuyOrder)
{
    auto order1 = addOrder(me::book::Order::SELL, 15.0, 15ull);
    auto order2 = addOrder(me::book::Order::SELL, 10.0, 15ull);
    auto order3 = addOrder(me::book::Order::SELL, 9.0, 15ull);
    auto order4 = addOrder(me::book::Order::SELL, 10.0, 15ull);
    auto order5 = addOrder(me::book::Order::SELL, 20.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order3, order2, order4, order1, order5}));
    EXPECT_TRUE(m_orderBook->getBids().empty());
    auto order6 = addOrder(me::book::Order::BUY, 8.0, 15ull);
    EXPECT_CALL(m_mockCallback, onMatch(_)).Times(0);
    EXPECT_EQ(m_orderBook->getAsks().size(), 5ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order6}));
}

TEST_F(TestOrderBook, noMatchForSellOrder)
{
    auto order1 = addOrder(me::book::Order::BUY, 15.0, 15ull);
    auto order2 = addOrder(me::book::Order::BUY, 10.0, 15ull);
    auto order3 = addOrder(me::book::Order::BUY, 20.0, 15ull);
    auto order4 = addOrder(me::book::Order::BUY, 15.0, 15ull);
    auto order5 = addOrder(me::book::Order::BUY, 9.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order3, order1, order4, order2, order5}));
    EXPECT_TRUE(m_orderBook->getAsks().empty());
    auto order6 = addOrder(me::book::Order::SELL, 21.0, 15ull);
    EXPECT_CALL(m_mockCallback, onMatch(_)).Times(0);
    EXPECT_EQ(m_orderBook->getBids().size(), 5ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order6}));
}

TEST_F(TestOrderBook, onFullFillMatchForBuyOrderWithSamePrice)
{
    auto order1 = addOrder(me::book::Order::SELL, 15.0, 15ull);
    auto order2 = addOrder(me::book::Order::SELL, 10.0, 15ull);
    auto order3 = addOrder(me::book::Order::SELL, 9.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order3, order2, order1}));
    EXPECT_TRUE(m_orderBook->getBids().empty());
    me::book::OrderMatch* orderMatch = nullptr;
    EXPECT_CALL(m_mockCallback, onMatch(_)).WillOnce(testing::SaveArg<0>(&orderMatch));
    auto order4 = addOrder(me::book::Order::BUY, 8.0, 15ull);
    addOrder(me::book::Order::BUY, 9.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order2, order1}));
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order4}));
    EXPECT_EQ(orderMatch->getBuyOrderID(), "id_5");
    EXPECT_EQ(orderMatch->getSellOrderID(), "id_3");
    EXPECT_EQ(orderMatch->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatch->getAskPrice(), 9.0);
}

TEST_F(TestOrderBook, onFullFillMatchForSellOrderWithSamePrice)
{
    auto order1 = addOrder(me::book::Order::BUY, 10.0, 15ull);
    auto order2 = addOrder(me::book::Order::BUY, 15.0, 15ull);
    auto order3 = addOrder(me::book::Order::BUY, 20.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order3, order2, order1}));
    EXPECT_TRUE(m_orderBook->getAsks().empty());
    me::book::OrderMatch* orderMatch = nullptr;
    EXPECT_CALL(m_mockCallback, onMatch(_)).WillOnce(testing::SaveArg<0>(&orderMatch));
    auto order4 = addOrder(me::book::Order::SELL, 21.0, 15ull);
    addOrder(me::book::Order::SELL, 20.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order2, order1}));
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order4}));
    EXPECT_EQ(orderMatch->getBuyOrderID(), "id_3");
    EXPECT_EQ(orderMatch->getSellOrderID(), "id_5");
    EXPECT_EQ(orderMatch->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatch->getAskPrice(), 20.0);
}


TEST_F(TestOrderBook, onFullFillMatchForBuyOrderHighPrice)
{
    auto order1 = addOrder(me::book::Order::SELL, 15.0, 15ull);
    auto order2 = addOrder(me::book::Order::SELL, 10.0, 15ull);
    auto order3 = addOrder(me::book::Order::SELL, 9.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order3, order2, order1}));
    EXPECT_TRUE(m_orderBook->getBids().empty());
    me::book::OrderMatch* orderMatch = nullptr;
    EXPECT_CALL(m_mockCallback, onMatch(_)).WillOnce(testing::SaveArg<0>(&orderMatch));
    auto order4 = addOrder(me::book::Order::BUY, 8.0, 15ull);
    addOrder(me::book::Order::BUY, 15.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order2, order1}));
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order4}));
    EXPECT_EQ(orderMatch->getBuyOrderID(), "id_5");
    EXPECT_EQ(orderMatch->getSellOrderID(), "id_3");
    EXPECT_EQ(orderMatch->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatch->getAskPrice(), 9.0);
}

TEST_F(TestOrderBook, onFullFillMatchForSellOrderLowPrice)
{
    auto order1 = addOrder(me::book::Order::BUY, 10.0, 15ull);
    auto order2 = addOrder(me::book::Order::BUY, 15.0, 15ull);
    auto order3 = addOrder(me::book::Order::BUY, 20.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order3, order2, order1}));
    EXPECT_TRUE(m_orderBook->getAsks().empty());
    me::book::OrderMatch* orderMatch = nullptr;
    EXPECT_CALL(m_mockCallback, onMatch(_)).WillOnce(testing::SaveArg<0>(&orderMatch));
    auto order4 = addOrder(me::book::Order::SELL, 21.0, 15ull);
    addOrder(me::book::Order::SELL, 9.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order2, order1}));
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order4}));
    EXPECT_EQ(orderMatch->getBuyOrderID(), "id_3");
    EXPECT_EQ(orderMatch->getSellOrderID(), "id_5");
    EXPECT_EQ(orderMatch->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatch->getAskPrice(), 9.0);
}

TEST_F(TestOrderBook, sellPartialMatchWithBuy)
{
    auto order1 = addOrder(me::book::Order::SELL, 15.0, 15ull);
    auto order2 = addOrder(me::book::Order::SELL, 9.0, 15ull);
    auto order3 = addOrder(me::book::Order::SELL, 10.0, 15ull);
    auto order4 = addOrder(me::book::Order::SELL, 9.0, 15ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order2, order4, order3, order1}));
    EXPECT_TRUE(m_orderBook->getBids().empty());
    me::book::OrderMatch* orderMatch = nullptr;
    EXPECT_CALL(m_mockCallback, onMatch(_)).WillOnce(testing::SaveArg<0>(&orderMatch));
    auto order5 = addOrder(me::book::Order::BUY, 8.0, 15ull);
    addOrder(me::book::Order::BUY, 9.0, 10ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order2, order4, order3, order1}));
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order5}));
    EXPECT_EQ(orderMatch->getBuyOrderID(), "id_6");
    EXPECT_EQ(orderMatch->getSellOrderID(), "id_2");
    EXPECT_EQ(orderMatch->getMatchedQuantity(), 10ull);
    EXPECT_EQ(orderMatch->getAskPrice(), 9.0);
    EXPECT_EQ(order2->getQty(), 5ull);
}

TEST_F(TestOrderBook, buyPartialMatchWithSell)
{
    auto order1 = addOrder(me::book::Order::BUY, 10.0, 15ull);
    auto order2 = addOrder(me::book::Order::BUY, 20.0, 15ull);
    auto order3 = addOrder(me::book::Order::BUY, 15.0, 15ull);
    auto order4 = addOrder(me::book::Order::BUY, 20.0, 15ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order2, order4, order3, order1}));
    EXPECT_TRUE(m_orderBook->getAsks().empty());
    me::book::OrderMatch* orderMatch = nullptr;
    EXPECT_CALL(m_mockCallback, onMatch(_)).WillOnce(testing::SaveArg<0>(&orderMatch));
    auto order5 = addOrder(me::book::Order::SELL, 21.0, 15ull);
    addOrder(me::book::Order::SELL, 20.0, 10ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order2, order4, order3, order1}));
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order5}));
    EXPECT_EQ(orderMatch->getBuyOrderID(), "id_2");
    EXPECT_EQ(orderMatch->getSellOrderID(), "id_6");
    EXPECT_EQ(orderMatch->getMatchedQuantity(), 10ull);
    EXPECT_EQ(orderMatch->getAskPrice(), 20.0);
    EXPECT_EQ(order2->getQty(), 5ull);
}

TEST_F(TestOrderBook, multipleSellMatchForBuyOrder_FullFill)
{
    std::vector<me::book::OrderMatch*> orderMatches;

    auto saveOrderMatch = [&](me::book::OrderMatch* orderMatch)
    {
        orderMatches.push_back(orderMatch);
    };

    auto order1 = addOrder(me::book::Order::SELL, 15.0, 15ull);
    auto order2 = addOrder(me::book::Order::SELL, 9.0, 15ull);
    auto order3 = addOrder(me::book::Order::SELL, 10.0, 20ull);
    auto order4 = addOrder(me::book::Order::SELL, 9.0, 10ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order2, order4, order3, order1}));
    EXPECT_TRUE(m_orderBook->getBids().empty());
    EXPECT_CALL(m_mockCallback, onMatch).Times(3).WillRepeatedly(saveOrderMatch);
    auto order5 = addOrder(me::book::Order::BUY, 8.0, 15ull);
    addOrder(me::book::Order::BUY, 10.0, 40ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order3, order1}));
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order5}));
    EXPECT_EQ(orderMatches[0]->getBuyOrderID(), "id_6");
    EXPECT_EQ(orderMatches[0]->getSellOrderID(), "id_2");
    EXPECT_EQ(orderMatches[0]->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatches[0]->getAskPrice(), 9.0);

    EXPECT_EQ(orderMatches[1]->getBuyOrderID(), "id_6");
    EXPECT_EQ(orderMatches[1]->getSellOrderID(), "id_4");
    EXPECT_EQ(orderMatches[1]->getMatchedQuantity(), 10ull);
    EXPECT_EQ(orderMatches[1]->getAskPrice(), 9.0);

    EXPECT_EQ(orderMatches[2]->getBuyOrderID(), "id_6");
    EXPECT_EQ(orderMatches[2]->getSellOrderID(), "id_3");
    EXPECT_EQ(orderMatches[2]->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatches[2]->getAskPrice(), 10.0);
    EXPECT_EQ(order3->getQty(), 5ull);
}

TEST_F(TestOrderBook, multipleBuyMatchForSellOrder_FullFill)
{
    std::vector<me::book::OrderMatch*> orderMatches;

    auto saveOrderMatch = [&](me::book::OrderMatch* orderMatch)
    {
        orderMatches.push_back(orderMatch);
    };

    auto order1 = addOrder(me::book::Order::BUY, 10.0, 15ull);
    auto order2 = addOrder(me::book::Order::BUY, 20.0, 15ull);
    auto order3 = addOrder(me::book::Order::BUY, 15.0, 20ull);
    auto order4 = addOrder(me::book::Order::BUY, 20.0, 10ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order2, order4, order3, order1}));
    EXPECT_TRUE(m_orderBook->getAsks().empty());
    EXPECT_CALL(m_mockCallback, onMatch(_)).Times(3).WillRepeatedly(saveOrderMatch);
    auto order5 = addOrder(me::book::Order::SELL, 21.0, 15ull);
    addOrder(me::book::Order::SELL, 15.0, 40ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order3, order1}));
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order5}));
    EXPECT_EQ(orderMatches[0]->getBuyOrderID(), "id_2");
    EXPECT_EQ(orderMatches[0]->getSellOrderID(), "id_6");
    EXPECT_EQ(orderMatches[0]->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatches[0]->getAskPrice(), 15.0);

    EXPECT_EQ(orderMatches[1]->getBuyOrderID(), "id_4");
    EXPECT_EQ(orderMatches[1]->getSellOrderID(), "id_6");
    EXPECT_EQ(orderMatches[1]->getMatchedQuantity(), 10ull);
    EXPECT_EQ(orderMatches[1]->getAskPrice(), 15.0);

    EXPECT_EQ(orderMatches[2]->getBuyOrderID(), "id_3");
    EXPECT_EQ(orderMatches[2]->getSellOrderID(), "id_6");
    EXPECT_EQ(orderMatches[2]->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatches[2]->getAskPrice(), 15.0);
    EXPECT_EQ(order3->getQty(), 5ull);
}

TEST_F(TestOrderBook, multipleSellMatchForBuyOrder_Partial)
{
    std::vector<me::book::OrderMatch*> orderMatches;

    auto saveOrderMatch = [&](me::book::OrderMatch* orderMatch)
    {
        orderMatches.push_back(orderMatch);
    };

    auto order1 = addOrder(me::book::Order::SELL, 15.0, 15ull);
    auto order2 = addOrder(me::book::Order::SELL, 9.0, 15ull);
    auto order3 = addOrder(me::book::Order::SELL, 10.0, 20ull);
    auto order4 = addOrder(me::book::Order::SELL, 9.0, 10ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order2, order4, order3, order1}));
    EXPECT_TRUE(m_orderBook->getBids().empty());
    EXPECT_CALL(m_mockCallback, onMatch).Times(3).WillRepeatedly(saveOrderMatch);
    auto order5 = addOrder(me::book::Order::BUY, 8.0, 15ull);
    auto order6 = addOrder(me::book::Order::BUY, 10.0, 55ull);
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order1}));
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order6, order5}));
    EXPECT_EQ(orderMatches[0]->getBuyOrderID(), "id_6");
    EXPECT_EQ(orderMatches[0]->getSellOrderID(), "id_2");
    EXPECT_EQ(orderMatches[0]->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatches[0]->getAskPrice(), 9.0);

    EXPECT_EQ(orderMatches[1]->getBuyOrderID(), "id_6");
    EXPECT_EQ(orderMatches[1]->getSellOrderID(), "id_4");
    EXPECT_EQ(orderMatches[1]->getMatchedQuantity(), 10ull);
    EXPECT_EQ(orderMatches[1]->getAskPrice(), 9.0);

    EXPECT_EQ(orderMatches[2]->getBuyOrderID(), "id_6");
    EXPECT_EQ(orderMatches[2]->getSellOrderID(), "id_3");
    EXPECT_EQ(orderMatches[2]->getMatchedQuantity(), 20ull);
    EXPECT_EQ(orderMatches[2]->getAskPrice(), 10.0);
    EXPECT_EQ(order6->getQty(), 10ull);
}

TEST_F(TestOrderBook, multipleBuyMatchForSellOrder_Partial)
{
    std::vector<me::book::OrderMatch*> orderMatches;

    auto saveOrderMatch = [&](me::book::OrderMatch* orderMatch)
    {
        orderMatches.push_back(orderMatch);
    };

    auto order1 = addOrder(me::book::Order::BUY, 10.0, 15ull);
    auto order2 = addOrder(me::book::Order::BUY, 20.0, 15ull);
    auto order3 = addOrder(me::book::Order::BUY, 15.0, 20ull);
    auto order4 = addOrder(me::book::Order::BUY, 20.0, 10ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order2, order4, order3, order1}));
    EXPECT_TRUE(m_orderBook->getAsks().empty());
    EXPECT_CALL(m_mockCallback, onMatch(_)).Times(3).WillRepeatedly(saveOrderMatch);
    auto order5 = addOrder(me::book::Order::SELL, 21.0, 15ull);
    auto order6 = addOrder(me::book::Order::SELL, 15.0, 55ull);
    EXPECT_THAT(m_orderBook->getBids(), testing::ElementsAreArray({order1}));
    EXPECT_THAT(m_orderBook->getAsks(), testing::ElementsAreArray({order6, order5}));
    EXPECT_EQ(orderMatches[0]->getBuyOrderID(), "id_2");
    EXPECT_EQ(orderMatches[0]->getSellOrderID(), "id_6");
    EXPECT_EQ(orderMatches[0]->getMatchedQuantity(), 15ull);
    EXPECT_EQ(orderMatches[0]->getAskPrice(), 15.0);

    EXPECT_EQ(orderMatches[1]->getBuyOrderID(), "id_4");
    EXPECT_EQ(orderMatches[1]->getSellOrderID(), "id_6");
    EXPECT_EQ(orderMatches[1]->getMatchedQuantity(), 10ull);
    EXPECT_EQ(orderMatches[1]->getAskPrice(), 15.0);

    EXPECT_EQ(orderMatches[2]->getBuyOrderID(), "id_3");
    EXPECT_EQ(orderMatches[2]->getSellOrderID(), "id_6");
    EXPECT_EQ(orderMatches[2]->getMatchedQuantity(), 20ull);
    EXPECT_EQ(orderMatches[2]->getAskPrice(), 15.0);
    EXPECT_EQ(order6->getQty(), 10ull);
}