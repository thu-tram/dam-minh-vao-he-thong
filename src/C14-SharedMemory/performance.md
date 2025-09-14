





-   -   [14. Leveraging Shared Memory in the Multicore
        Era]()
        -   [14.1. Programming Multicore
            Systems]()
        -  
        -   [14.3. Synchronizing
            Threads]()
            -  
            -  
            -   [14.3.3. Other Synchronization
                Constructs]()
        -   [14.4. Measuring Parallel
            Performance]()
            -   [14.4.1. Parallel Performance
                Basics]()
            -   [14.4.2. Advanced
                Topics]()
        -  
        -  
        -   [14.7. Implicit Threading with
            OpenMP]()
        -  
        -  











## 14.4. Measuring the Performance of Parallel Programs 

So far, we have used the `gettimeofday` function to measure the amount
of time it takes for programs to execute. In this section, we discuss
how to measure how well a parallel program performs in comparison to a
serial program as well as other topics related to measuring the
performance of parallel programs.


We first cover some basics related to parallel performance:



-   Speedup

-   Efficiency

-   Amdahl's Law


While Amdahl's Law and speedup are two very important concepts related
to performance, having a good understanding of the following topics will
round out a reader's understanding of performance:



-   [Gustafson-Barsis
    Law](performance_advanced.html#_gustafson_barsis_law)

-   Scalability


Specifically, the Gustafson-Barsis Law gives a better understanding of
the limits of Amdahl's Law.





