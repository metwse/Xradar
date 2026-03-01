# Xradar
a *runtime* for signal processing and radar control.

## Architecture
Xradar employs a custom event loop, optimized for parallel computing. The
event loop synchronizes processing pipeline, providing parallel computing for
processing branches.

The system is component based, and it can be extended using dynamic libraries
*at runtime*. The core event loop handles 3 kinds of components:

1. Producer: The root signal source. Initiates a processing pipeline. This
   pipeline may branch out depending on configuration.
2. Middleware: Signal processor middleware. One or more processor can be
   chained.
3. Consumer: End of the signal pipeline. Used for visualization/result
   recording.

A middleware component can apply backpressure to a producer by sending
delay-hint messages back to the root producer. Xradar provides a generic
interface for this backward communication; however, the specific handling of
these messages is entirely implementation-defined

### Pipeline
The processing pipeline is initiated by the `data_callback` of a producer
(input source). Signals are propagated and processed through the pipeline,
where the event loop automatically parallelizes operations whenever possible

```txt
source ----> middleware1 ----> middleware2 ----> visualizer
                           |                 |
                           --> middleware3 ---
```

Here, the source triggers signal propagation. First, `middleware1` runs; then,
`middleware2` and `middleware3` run *in parallel*. Finally, the `visualizer` is
fed the results from `middleware2` and `middleware3` in the order of their
construction.
