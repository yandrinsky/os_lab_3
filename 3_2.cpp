#include <iostream>
#include<windows.h>
#include <iomanip>


using namespace std;

const long BLOCK_SIZE = 307230;
const long N = 200000000;
int treadsAmount = 0;

struct CalculateResult {
    double PI;
    unsigned int workTime;
};

CalculateResult *calculatePI() {
    double resultPI = 0;
    double x;

    unsigned int start = GetTickCount();

#pragma omp parallel for schedule(dynamic, BLOCK_SIZE) num_threads(treadsAmount) reduction(+: resultPI)

    for (int i = 0; i < N; ++i) {
        x = (i + 0.5) / N;
        resultPI += (4.0 / (1.0 + x * x)) / N;
    }

    unsigned int end = GetTickCount();

    return new CalculateResult{resultPI, end - start};
}

int main() {
    do {
        cout << endl << "Enter number of threads (-1 - exit): ";
        cin >> treadsAmount;

        if(treadsAmount == -1){
            break;
        }

        CalculateResult result = *calculatePI();

        cout << "Time: " << result.workTime << endl;
        cout << "PI: " << setprecision(15) << result.PI << endl;
    } while (true);

    return 0;
}