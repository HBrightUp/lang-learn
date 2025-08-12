#include <iostream>
#include <string>
#include <memory>

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual std::string doAlgorithm(const std::string& data) = 0;
};


class ConcreteStrategyA : public Strategy {
public:
    std::string doAlgorithm(const std::string& data) override {
        return "Strategy A: " + data;
    }
};

class ConcreteStrategyB : public Strategy {
public:
    std::string doAlgorithm(const std::string& data) override {
        return "Strategy B: " + data;
    }
};

class Context {
private:
    std::unique_ptr<Strategy> m_strategy;
public:
    Context(std::unique_ptr<Strategy> strategy) : m_strategy(std::move(strategy)) {}

    void setStrategy(std::unique_ptr<Strategy> strategy) {
        m_strategy = std::move(strategy);
    }

    std::string executeStrategy(const std::string& data) {
        return m_strategy->doAlgorithm(data);
    }
};