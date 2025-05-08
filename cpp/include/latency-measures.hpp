#pragma once
#include <chrono>

class LatencyMeasure
{
    public:
    virtual ~LatencyMeasure()
    {}

    virtual void begin()
    {
        m_isSet = 1;
    }
    virtual void end()
    {
        if (1 != m_isSet)
            start();
        m_isSet = 2;
    }
    virtual size_t latency_ms() const = 0;

    bool hasLatency() const
    {
        return 2 == m_isSet;
    }
    void reset()
    {
        m_isSet = 0;
    }

    protected:
    LatencyMeasure() : m_isSet(0) {}
    int m_isSet;

};

class RegularLatencyMeasure: LatencyMeasure
{
    public:
    RegularLatencyMeasure() : LatencyMeasure()
    {}
    virtual ~RegularLatencyMeasure() {}

    virtual void begin()
    {
        LatencyMeasure::begin();
        m_begin = std::chrono::system_clock::now();
    }
    virtual void end()
    {
        LatencyMeasure::end();
        m_end = std::chrono::system_clock::now();
    }
    virtual size_t latency_ms() const
    {
        if (hasLatency())
            return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_begin).count();
        return (size_t)-1;            
    }

    protected:
    std::chrono::system_clock::time_point m_begin;
    std::chrono::system_clock::time_point m_end;
};

class HighResolutionLatencyMeasure: LatencyMeasure
{
    public:
    HighResolutionLatencyMeasure() : LatencyMeasure() 
    {}
    virtual ~HighResolutionLatencyMeasure()
    {}

    virtual void begin()
    {
        LatencyMeasure::begin();
        m_begin = std::chrono::high_resolution_clock::now();
    }
    virtual void end()
    {
        LatencyMeasure::end();
        m_end = std::chrono::high_resolution_clock::now();
    }
    virtual size_t latency_ms() const
    {
        if (hasLatency())
            return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_begin).count();
        return (size_t)-1;
    }

    protected:
    std::chrono::high_resolution_clock::time_point m_begin;
    std::chrono::high_resolution_clock::time_point m_end;
};