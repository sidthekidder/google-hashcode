# google-hashcode

Solutions to various problems from Google Hashcode - scored using https://github.com/vitowalteranelli/Google-HashCode-Playground

## [2017 Qualification Round](https://storage.googleapis.com/coding-competitions.appspot.com/HC/2017/hashcode2017_qualification_task.pdf)

Score: 562500

Naive simple greedy technique - assign priority to each request ( = number of requests * latency without caching). Place the max priority request into the closest cache server if enough space. Repeat until priority queue empty.
