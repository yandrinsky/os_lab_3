#include <windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

const long BLOCK_SIZE = 307230;
const long N = 200000000;

HANDLE *threadsArray;
CRITICAL_SECTION criticalSection;

struct ThreadProps {
    double *resultPI;
    DWORD  *offset;
    int startBlock;
    int THREADS_AMOUNT;
};

struct CalculateResult {
    double PI;
    unsigned int workTime;
};

DWORD WINAPI thread(LPVOID param) {
    auto *props = (ThreadProps *) param;
    DWORD startBlock = props->startBlock;
    DWORD *offset = props->offset;
    long THREADS_AMOUNT = props->THREADS_AMOUNT;
    double *resultPI = props->resultPI;

    double x, pi;

    while (startBlock < N) {
        pi = 0;

        for (int i = startBlock; (i < startBlock + BLOCK_SIZE) && (i < N); ++i) {
            x = (i + 0.5) / N;
            pi += (4.0 / (1.0 + x * x)) / N;
        }


        EnterCriticalSection(&criticalSection);

        *offset += BLOCK_SIZE;
        startBlock = *offset;
        *resultPI += pi;

        LeaveCriticalSection(&criticalSection);
    }


    return 0;
}

CalculateResult *calculatePI(int THREADS_AMOUNT) {
    threadsArray = new HANDLE[THREADS_AMOUNT];
    double resultPI = 0;
    DWORD offset = THREADS_AMOUNT * BLOCK_SIZE;

    InitializeCriticalSection(&criticalSection);

    for (int i = 0; i < THREADS_AMOUNT; ++i) {
        threadsArray[i] = CreateThread(
                NULL,
                0,
                thread,
                new ThreadProps{&resultPI, &offset, BLOCK_SIZE * i,  THREADS_AMOUNT},
                CREATE_SUSPENDED,
                NULL
        );
    }


    unsigned int start = GetTickCount();

    for (int i = 0; i < THREADS_AMOUNT; ++i) {
        ResumeThread(threadsArray[i]);
    }

    WaitForMultipleObjects(THREADS_AMOUNT, threadsArray, TRUE, INFINITE);

    unsigned int end = GetTickCount();

    DeleteCriticalSection(&criticalSection);

    for (int i = 0; i < THREADS_AMOUNT; ++i) {
        CloseHandle(threadsArray[i]);
    }

    delete[] threadsArray;
    threadsArray = nullptr;

    return new CalculateResult{resultPI, end - start};
}

int main() {
    int treadsAmount;

    do {
        cout << endl << "Enter number of threads (-1 - exit): ";
        cin >> treadsAmount;

        if(treadsAmount == -1){
            break;
        }

        CalculateResult result = *calculatePI(treadsAmount);

        cout << "Time: " << result.workTime << endl;
        cout << "PI: " << setprecision(15) << result.PI << endl;
    } while (true);

    return 0;
}