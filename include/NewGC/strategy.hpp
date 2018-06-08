#pragma once



enum class GCEventType
{
    ALLOCATION,
    FREE
};

struct GCEvent
{
    GCEventType type;
    std::size_t size;
};

struct Strategy
{
    Strategy(MemoryManager& memory_manager) : memory_manager(memory_manager) {}
    virtual ~Strategy() = default;
    
    virtual void notify(GCEvent&) = 0;
    
private:
    MemoryManager& memory_manager;
};

struct EveryTenthStrategy : public Strategy
{
    EveryTenthStrategy(MemoryManager& memory_manager) : Strategy(memory_manager) {}
    virtual ~EveryTenthStrategy() = default;
    
    virtual void notify(GCEvent& event) override
    {
        
    }
    
private:
    int num_allocations { 0 };
};
