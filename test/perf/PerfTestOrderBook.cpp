#include <cstdlib>
#include <string>
#include <me/book/Order.h>
#include <me/book/OrderBook.h>
#include <me/book/OrderMatch.h>
#include <memory>
#include <vector>
#include <iostream>
#include <chrono>
#include <deque>

class EmptyOrderBookCallback : public me::book::OrderBookCallback
{
public:
    unsigned int getMatchCount() { return m_matchCount; }
private:
    void onMatch(me::book::OrderMatch* orderMatch) final override
    {
        m_orderMatch.push_back(std::unique_ptr<me::book::OrderMatch>(orderMatch));
        m_matchCount++;
    }

    unsigned int m_matchCount = 0u;
    std::deque<std::unique_ptr<me::book::OrderMatch>> m_orderMatch;
};

std::vector<std::unique_ptr<me::book::Order>> generateOrders(unsigned int size, unsigned int numberOfSymbol)
{
    std::vector<std::unique_ptr<me::book::Order>> orders;
    orders.reserve(size);

    std::vector<std::string> symbols;
    symbols.reserve(numberOfSymbol);

    for (unsigned int i = 0; i < numberOfSymbol ; i++)
    {
        symbols.push_back("SYM" + std::to_string(i));
    }

    for (unsigned int i =0; i < size; i++)
    {
        bool isBuy = (i % 2) == 0;
        unsigned int delta = isBuy ? 1880 : 1884;
        // AsSK 1893
        // ASK 1892
        // ASK 1891
        // ASK 1890
        // ASK 1889 crossable
        // ASK 1888 crossable
        // ASK 1887 crossable
        // ASK 1886 crossable
        // ASK 1885 crossable
        // ASK 1884 crossable

        // BID 1889 crossable
        // BID 1888 crossable
        // BID 1887 crossable
        // BID 1886 crossable
        // BID 1885 crossable
        // BID 1884 crossable
        // BID 1883
        // BID 1882
        // BID 1881
        // BID 1880

        double price = (std::rand() % 10) + delta;
        unsigned long long qty = ((std::rand() % 10) + 1) * 100;
        int symbolIdex = std::rand() % numberOfSymbol;
        //auto order = new me::book::Order(i, isBuy,"OID" + std::to_string(i), symbols[symbolIdex], price, qty);
        orders.push_back(std::make_unique<me::book::Order>(i, isBuy,"OID" + std::to_string(i), symbols[symbolIdex], price, qty));
    } 

    return orders;
}

void checkSingleOrderBook(unsigned int size, int numberOfTimes)
{
    double totalTime = 0.0;
    unsigned int totalMatch = 0u;

    for (int i = 0; i < numberOfTimes; i++)
    {
        EmptyOrderBookCallback callback;
        me::book::OrderBook orderBook("SYM", callback);

        auto orders = generateOrders(size, 1u);

        auto start = std::chrono::high_resolution_clock::now();

        for (auto& order : orders)
        {
            orderBook.submitOrder(std::move(order));
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        totalTime += elapsed_time.count() ;
        totalMatch += callback.getMatchCount();
        std::cout << "ElapsedTime: " << elapsed_time.count() << 
            "(second), OrderCount: "  << size << " MatchCount: " << callback.getMatchCount() << " Order/Second: " << (unsigned int) (size / elapsed_time.count()) <<  std::endl;
    }

    std::cout << "Avg ElapsedTime: " << totalTime / (double) numberOfTimes << "(seconds), MatchCount : " << totalMatch / numberOfTimes 
            << "Order/Second: " << (unsigned long long) ( (unsigned long long)size * (unsigned long long) numberOfTimes / totalTime) << std::endl;
}

int main(int argc, const char* argv[])
{
    unsigned int size = (unsigned int) std::atoi(argv[1]);
    int numberOfTimes = std::atoi(argv[2]);
    checkSingleOrderBook(size, numberOfTimes);
}