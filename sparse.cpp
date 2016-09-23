#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

#include "sparseArray.h"
#include "ArgumentManager.h"
#include "sparseLinkedList.h"

short lineCounter(string);  //Return the number of lines in a file, even if they are invalid.

int main(int argc, char *argv[]){
    ArgumentManager am(argc, argv);             //Parse the input parameters.
    string storage = am.get("storage"), recursive = am.get("recursive"), 
    A = am.get("A"), B = am.get("B"), result = am.get("result");
    if(am.get("operation") == "multiply") {
        cout << "This functionality has not yet been implemented.\n";   //For a future assignment.
        return 0;
    }
    if(A == "" || B == "" || result == ""){
        cout << "You must provide two input filenames and an output filename.\n"<<  //If there are no given filenames, return an error.
        "Use this format:\n"<<
        "sparse \"operation=<add|multiply>;storage=<array|linkedlist>;recursive=<Y/N>;A=<file>;B=<file>;result=<file>\"\n";
        return 0;
    }
    short Asize = lineCounter(A), Bsize = lineCounter(B), ABsize = Asize + Bsize;   //We are adding two matrices together, so the output needs to have enough space for both.
    if(Asize == -1 || Bsize == -1){             //Linecounter returns -1 if the file couldn't be opened.
        cout << "Error: No file found.\n";
        return 0;
    }
    if(Asize == 0 || Bsize == 0){
        cout << "Error: An input file is empty.\n";
        return 0;
    }
    if(storage == "linkedlist"){
        matrixNode *rootA = new matrixNode, *rootB = new matrixNode;
        matrixNode largestA = listFileReader(A, rootA), largestB = listFileReader(B, rootB);
        if(largestA.i != largestB.i || largestA.j != largestB.j){               //Check bottom right corner of matrices. They should be equivalent sizes.
            cout << "Error: The input matrices must be of the same size.\n";
            return 0;
        }
        if(recursive == "Y") listRecAdder(rootA, rootB);
        else listIterAdder(rootA, rootB);
        listSort(rootB);
        listFileWriter(result, rootB);
        deleteList(rootA); deleteList(rootB);
    }
    else{
        matrixEntry *arrayA = new matrixEntry[Asize], *arrayB = new matrixEntry[ABsize]; //arrayB will be where we add arrayA to, so we use ABsize to define it.
        Asize = arrayFileReader(A, arrayA); Bsize = arrayFileReader(B, arrayB); //we redefine asize and bsize because the file reader will omit invalid lines.
        matrixEntry largestA = largestArrayEntry(arrayA, Asize), largestB = largestArrayEntry(arrayB, Bsize);
        if(largestA.i != largestB.i || largestA.j != largestB.j){               //Check bottom right corner of matrices. They should be equivalent sizes.
            cout << "Error: The input matrices must be of the same size.\n";
            return 0;
        }
        Asize = purgeArrayZeroValues(arrayA, largestA, Asize); Bsize = purgeArrayZeroValues(arrayB, largestB, Bsize); //We must get rid of zero values in a separate function because we don't want to remove the largest value.
        if(recursive == "Y") arrayRecAdder(0, arrayA, arrayB, Asize, Bsize);                                //Thus, we first calculate the largest value and then pass it so that it isn't deleted from the arrays. This will change the size, so we update the sizes.
        else arrayIterAdder(arrayA, arrayB, Asize, Bsize);  //Both versions of the addition operation.
        arraySort(arrayB, ABsize);              //Bubble sort on arrayB, our output
        arrayFileWriter(result, arrayB, ABsize);    //write the output to the given result file
        delete[] arrayA; delete[] arrayB;
    }

    cout << "Operation complete. Check \"" + result + "\" for the output matrix.\n";
    return 0;
}

short lineCounter(string fileName){
    ifstream counter;
    counter.open(fileName);
    if(!counter.good()){
        counter.close();
        return -1;              //Return -1 if there is a bad file.
    }

    short count = 0;
    string temp;
    while(getline(counter, temp)){
        if(*(temp.begin()) == '#' || temp.length() == 0) continue;  //Commented lines have a # to signify them
        count++;
    }

    counter.close();
    return count;
}