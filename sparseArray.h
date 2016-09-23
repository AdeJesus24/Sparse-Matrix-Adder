///////////////////////////////
//                           //
//   Array Storage Header    //
//                           //
///////////////////////////////

struct matrixEntry{     //An entry in the array has a coordinate made from i and j, and a value.
    int i = 0, j = 0;
    double value = 0;
};

short arrayFileReader(string fileName, matrixEntry* array){
    ifstream reader;
    reader.open(fileName);
    string tempLine, tempElement;
    int tempI, tempJ;
    double tempValue;

    short n = 0;
    while(std::getline(reader, tempLine)){
        if(*(tempLine.begin()) == '#' || tempLine.length() == 0) continue;
        std::stringstream tempStream;
        tempStream.str(tempLine);

        try{
            getline(tempStream, tempElement, ' ');  //All three values from the file line are read.
            tempI = stoi(tempElement);
            getline(tempStream, tempElement, ' ');
            tempJ = stoi(tempElement);
            getline(tempStream, tempElement, ' ');
            tempValue = stod(tempElement);
        }
        catch(...) {continue;}  //catches stoi and stod errors.
        if(tempI < 1 || tempJ < 1) continue;  //We don't want i's and j's that are zero or negative.
        array[n].i = tempI;
        array[n].j = tempJ;
        array[n++].value = tempValue;
    }

    reader.close();
    return n;
}

matrixEntry largestArrayEntry(matrixEntry* array, short size){  //Finding the largest entry is useful for two reasons:
    matrixEntry largest;                                        //It lets us compare the two matrices to check that they are equal sizes,
    largest.i = 0; largest.j = 0;                               //and it also helps us in purgeArrayZeroValues.
    short n = 0;
    while(n < size){
        if(largest.i < array[n].i || largest.j < array[n].j) largest = array[n];    //Look for the largest matrix entry.
        n++;
    }
    return largest;
}

short purgeArrayZeroValues(matrixEntry* array, matrixEntry largest, short size){    //Whenever a zero value is found that is NOT the largest i-j, replace it with the
    short n = 0;                                                                    //last entry in the array and remove that entry from the end. We're going to sort it later anyways.
    while(n < size){
        if(array[n].value == 0 && !(array[n].i == largest.i && array[n].j == largest.j)){
            array[n] = array[--size];
            array[size].i = 0; array[size].j = 0; array[size].value = 0;
        }
        n++;
    }
    return size;    //We are updating the size by doing this, so we return the new decremented size.
}

void innerArrayRecAdder(int iterA, int iterB, bool& isMatching, matrixEntry* arrayA, matrixEntry* arrayB, short Asize, short Bsize){
    if(iterB >= Bsize) return;
    if(arrayA[iterA].i == arrayB[iterB].i && arrayA[iterA].j == arrayB[iterB].j){       //This loop looks through arrayB for a match with the arrayA from the outer loop. It sets a flag which was passed by reference,
        arrayB[iterB].value += arrayA[iterA].value;                                     //so the outer loop will be able to append the arrayA element to the end of arrayB if there was no match.
        isMatching = true;                                                              //If there is a match, we add arrayA to arrayB here.
    }
    else innerArrayRecAdder(iterA, iterB+1, isMatching, arrayA, arrayB, Asize, Bsize);
}

void arrayRecAdder(int iterA, matrixEntry* arrayA, matrixEntry* arrayB, short Asize, short Bsize){
    if(iterA >= Asize) return;
    bool isMatching = false;
    innerArrayRecAdder(iterA, 0, isMatching, arrayA, arrayB, Asize, Bsize); //We call the inner loop.
    if(!isMatching) arrayB[Bsize++] = arrayA[iterA];
    arrayRecAdder(iterA+1, arrayA, arrayB, Asize, Bsize);   //We call the outer loop again.
}

void arrayIterAdder(matrixEntry* arrayA, matrixEntry* arrayB, short Asize, short Bsize){
    bool isMatching = false;
    short iterA = 0, iterB = 0;
    while(iterA < Asize){
        while(iterB < Bsize){
            if(arrayA[iterA].i == arrayB[iterB].i && arrayA[iterA].j == arrayB[iterB].j){
                arrayB[iterB].value += arrayA[iterA].value;
                isMatching = true;
                break;
            }
            iterB++;
        }
        iterB = 0;                                          //If there was no match, add the arrayA element to the end of arrayB.
        if(!isMatching) arrayB[Bsize++] = arrayA[iterA];
        else isMatching = false;
        iterA++;
    }
}

void swap(matrixEntry* a, matrixEntry* b){
    matrixEntry temp = *a;
    *a = *b;
    *b = temp;
}

void arraySort(matrixEntry* array, short size){
    short outer = 0, inner;
    while(outer < size - 1){    //Bubble sorting starts at the end and goes backwards. For each
        inner = size - 1;       //iteration, the inner starts at the outer position and goes forwards to the end.
        while(outer < inner){
            if(array[inner-1].i > array[inner].i || (array[inner-1].i == array[inner].i && array[inner-1].j > array[inner].j)) {
                swap(array[inner-1], array[inner]);
            }
            inner--;
        }
        outer++;
    }
}

void arrayFileWriter(string fileName, matrixEntry* array, short size){
    ofstream writer;
    writer.open(fileName);
    short n = 0;
    while(n < size) {
        if(array[n].value != 0 || n == size - 1)    //Only print an entry if it isn't zero, UNLESS it's the last entry in the line. Since we sorted the array, the last entry in the 
            writer << fixed << array[n].i << " " << array[n].j << " " << setprecision(2) << array[n].value << endl;                 //line is going to be the largest one.
        n++;
    }
    writer.close();
}