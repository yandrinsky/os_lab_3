//#include <windows.h>
//#include <iostream>
//#include <iomanip>
//
//using namespace std;
//
//const long BLOCK_SIZE = 307230;
//const long N = 200000000;
//
//HANDLE *threadsArray;
//CRITICAL_SECTION criticalSection;
//
//struct ThreadProps {
//    double *resultPI;
//    long startBlock;
//    int THREADS_AMOUNT;
//};
//
//struct CalculateResult {
//    double PI;
//    unsigned int workTime;
//};
//
//DWORD WINAPI thread(LPVOID param) {
//    auto *props = (ThreadProps *) param;
//    long startBlock = props->startBlock;
//    long THREADS_AMOUNT = props->THREADS_AMOUNT;
//    double *resultPI = props->resultPI;
//    long curBlock = startBlock;
//
//    double x, pi;
//
//    while (curBlock < N) {
//        pi = 0;
//
//        for (int i = curBlock; (i < curBlock + BLOCK_SIZE) && (i < N); ++i) {
//            x = (i + 0.5) / N;
//            pi += (4.0 / (1.0 + x * x)) / N;
//        }
//
//        curBlock += THREADS_AMOUNT * BLOCK_SIZE;
//
//        EnterCriticalSection(&criticalSection);
//
//        *resultPI += pi;
//
//        LeaveCriticalSection(&criticalSection);
//    }
//
//    return 0;
//}
//
//CalculateResult *calculatePI(int THREADS_AMOUNT) {
//    threadsArray = new HANDLE[THREADS_AMOUNT];
//    double resultPI = 0;
//
//    InitializeCriticalSection(&criticalSection);
//
//    for (int i = 0; i < THREADS_AMOUNT; ++i) {
//        threadsArray[i] = CreateThread(
//                NULL,
//                0,
//                thread,
//                new ThreadProps{&resultPI, i * BLOCK_SIZE, THREADS_AMOUNT},
//                CREATE_SUSPENDED,
//                NULL
//        );
//    }
//
//
//    unsigned int start = GetTickCount();
//
//    for (int i = 0; i < THREADS_AMOUNT; ++i) {
//        ResumeThread(threadsArray[i]);
//    }
//
//    WaitForMultipleObjects(THREADS_AMOUNT, threadsArray, TRUE, INFINITE);
//
//    unsigned int end = GetTickCount();
//
//    DeleteCriticalSection(&criticalSection);
//
//    for (int i = 0; i < THREADS_AMOUNT; ++i) {
//        CloseHandle(threadsArray[i]);
//    }
//
//    delete[] threadsArray;
//    threadsArray = nullptr;
//
//    return new CalculateResult{resultPI, end - start};
//}
//
//int main() {
//    int treadsAmount;
//
//    do {
//        cout << endl << "Enter number of threads (-1 - exit): ";
//        cin >> treadsAmount;
//
//        if(treadsAmount == -1){
//            break;
//        }
//
//        CalculateResult result = *calculatePI(treadsAmount);
//
//        cout << "Time: " << result.workTime << endl;
//        cout << "PI: " << setprecision(15) << result.PI << endl;
//    } while (true);
//
//    return 0;
//}
