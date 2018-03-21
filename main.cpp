#include <iomanip>
#include <iostream>
#include <random>
#include <boost/timer/timer.hpp>
#include "Matrix.h"


/**
 * a function that takes 2 types and 2 lambdas and an optional number of times to run,
 * and returns the average amount of time taken by the tF when tF is passed m1 and m2
 * @tparam T any type
 * @tparam testF a lambda that takes two T& and performs an operation on them
 * @tparam randF a lambda that takes a T& and randomizes it
 * @param m1 the first param to be run in the test
 * @param m2 the second param to be run in the test
 * @param tF a lambda to test the given params
 * @param rF a lambda to randomize one of the given params
 * @param numTests the number of times tF is to be run, default=100
 * @return the total CPU time taken by tF in seconds
 */
template <typename T, typename testF, typename randF>
double avgTime(T x, T y, const testF& tF, const randF& rF, const int numTests=100) {
    boost::timer::cpu_timer timer;
    timer.stop();
    for (int i = 0; i < numTests; i++) {
        rF(x);
        rF(y);
        timer.resume();
        tF(x, y);
        timer.stop();
    }
    return (timer.elapsed().system + timer.elapsed().user) / numTests / pow(10, 9);
}

int main() {
    const int mSize = 100;
    Matrix<int> m1 (mSize);
    Matrix<int> m2 (mSize);
    const int numTests = 50;
    srand((unsigned int)time(nullptr));
    auto randF = [] (Matrix<int>& m) -> void { m.map([] (int i ) { return rand(); }); }; //not truly random, doesn't matter in this context
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "addition: " << avgTime(m1, m2, [] (Matrix<int>& m1, Matrix<int>& m2) { m1 + m2; }, randF, numTests) << "s" << std::endl;
    std::cout << "compound addition: " << avgTime(m1, m2, [] (Matrix<int>& m1, Matrix<int>& m2) { m1 += m2; }, randF, numTests) << "s" << std::endl;
    std::cout << "multiplication: " << avgTime(m1, m2, [] (Matrix<int>& m1, Matrix<int>& m2) { m1 * m2; }, randF, numTests) << "s" << std::endl;
    std::cout << "compound multiplication: " << avgTime(m1, m2, [] (Matrix<int>& m1, Matrix<int>& m2) { m1 *= m2; }, randF, numTests) << "s" << std::endl;
    return 0;
}
