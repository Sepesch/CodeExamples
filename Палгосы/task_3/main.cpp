#include <iostream>
#include "Strassen.cpp"
#include "Sorter.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    string choice;
    if(argc>1){
        choice = argv[1];
    }
    else{
        choice = "1";
    }
    if(choice == "1"){
        testCorrectness(128, 20);
        
        performanceComparison(64, 2048, 2, 3);
        
    }
    if(choice == "2"){
        testSortingCorrectness(100000, 20);
    
        performanceComparisonSorting(10000, 1000000, 5, 5);
        
    }

    
    return 0;
}