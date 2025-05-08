# statsd-clients

## C++ single-threaded client

### setup initialize function and macros to easily use in code
```
#include "statsd-client.hpp"

#define SEND_COUNT_METRIC(METRIC_NAME,METRIC_VALUE) \
    StatsDClient::getInstance().count(METRIC_NAME, METRIC_VALUE)

#define SEND_HISTOGRAM_METRIC(METRIC_NAME,METRIC_VALUE) \
    StatsDClient::getInstance().histogram(METRIC_NAME, METRIC_VALUE)

#define SEND_TIMING_METRIC(METRIC_NAME,METRIC_VALUE) \
    StatsDClient::getInstance().timing(METRIC_NAME, METRIC_VALUE)

void initializeStatsD()
{
    if(!StatsDClient::getInstance().open(
        "127.0.0.1",
        3030,
        "MyMetrics",
        { {"TagName1", "TagValue1"}, {"TagName2", "TagValue2"} }
    ))
    {
        throw StatsDClient.getInstance().getLastError();
    }
}
```

### Measure some feature usage or event occurrence
```
    // ..... doing some activity ....
    SEND_COUNT_METRIC("<Category>.<SubCategory>.<Activity>", 1);
```

### Measure some longer lived feature latency or event latency
```
    #include "latency-measures.hpp"

    LatencyMeasure timer = RegularLatencyMeasure();

    timer.begin();
    // ..... doing some activity ....
    timer.end();
    SEND_TIMING_METRIC("<Category>.<SubCategory>.<Activity>", timer.latency_ms());
```

### Measure performance feature latency or event latency
```
    #include "latency-measures.hpp"

    LatencyMeasure timer = HighResolutionLatencyMeasure();

    timer.begin();
    // ..... doing some activity ....
    timer.end();
    SEND_TIMING_METRIC("<Category>.<SubCategory>.<Activity>", timer.latency_ms());
```
## ... more to follow ...
