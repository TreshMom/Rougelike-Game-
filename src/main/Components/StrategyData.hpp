#pragma once
#include <memory>

class StrategyContext;

struct StrategyData {
    std::unique_ptr<StrategyContext> strategy_context;
};
