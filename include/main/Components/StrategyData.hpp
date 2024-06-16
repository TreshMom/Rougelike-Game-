#pragma once
#include <memory>

class StrategyContext;

// Данные о стратегии моба
struct StrategyData {
    std::unique_ptr<StrategyContext> strategy_context;
};
