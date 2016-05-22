в папке `include` находятся реализации политик LRU, LFU, FIFO 

в папке `test` написаны unit тесты с использованием gtest

каждый алгоритм тестировался набором тестов:
max_size,
SimpleTest,
BasicFunctionality,
RandomTests,
ThreadSafety,

собирается все `cmake`

для запуска тестов `./run.sh`
пример использования `main/main.cpp`
