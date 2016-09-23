////////////////////////////////////
//                                //
//    LinkedList Storage Header   //
//                                //
////////////////////////////////////

struct matrixNode{
    int i = 0, j = 0;
    double value = 0;
    matrixNode *next = nullptr, *previous = nullptr;
};

matrixNode listFileReader(string fileName, matrixNode* current){
    ifstream reader;
    reader.open(fileName);
    string tempLine, tempElement;
    int tempI, tempJ;
    double tempValue;
    matrixNode largest;
    largest.i = 0; largest.j = 0;

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
        current->i = tempI;
        current->j = tempJ;
        if(largest.i < current->i || largest.j < current->j) largest = *current;    //We keep track of the largest i and j node.
        current->value = tempValue;
        current->next = new matrixNode;     //Point to the next node, and make that node point to this one.
        current->next->previous = current;
        current = current->next;            //Change our current node for the next iteration to work on.
    }

    reader.close();
    return largest;
}

void innerListRecAdder(bool& isMatching, matrixNode* currentA, matrixNode*& currentB, matrixNode*& previousB){
    if(!currentB) return;
    if(currentA->i == currentB->i && currentA->j == currentB->j){
        currentB->value += currentA->value;
        isMatching = true;
        return;
    }
    previousB = currentB;       //Both previousB and currentB were passed by reference so that the outer function can use them.
    currentB = currentB->next;
    innerListRecAdder(isMatching, currentA, currentB, previousB);
}

void listRecAdder(matrixNode* currentA, matrixNode* currentB){
    if(!currentA) return;
    bool isMatching = false;
    matrixNode *rootB = currentB, *previousB;
    innerListRecAdder(isMatching, currentA, currentB, previousB);
    if(!isMatching){
        currentB = new matrixNode;
        currentB->previous = previousB; //previousB is here to connect currentB with the node before it.
        previousB->next = currentB;     //We have to also connect that node with this new node.
        currentB->i = currentA->i;
        currentB->j = currentA->j;
        currentB->value = currentA->value;
    }
    listRecAdder(currentA->next, rootB);    //Call the same function with a new A node but the original B node.
}

void listIterAdder(matrixNode* currentA, matrixNode* currentB){
    bool isMatching = false;
    matrixNode *rootB = currentB, *previousB;
    while(currentA){
        while(currentB){
            if(currentA->i == currentB->i && currentA->j == currentB->j){
                currentB->value += currentA->value;
                isMatching = true;
                break;
            }
            previousB = currentB;
            currentB = currentB->next;
        }
        if(!isMatching){                //If no match, replace currentA with currentB, which is currently past the list.
            currentB = new matrixNode;
            currentB->previous = previousB; //previousB is here to connect currentB with the node before it.
            previousB->next = currentB;     //We have to also connect that node with this new node.
            currentB->i = currentA->i;
            currentB->j = currentA->j;
            currentB->value = currentA->value;
        }
        else isMatching = false;
        currentB = rootB;               //We reset currentB to the starting position.
        currentA = currentA->next;
    }
}

void swap(matrixNode* a, matrixNode* b){
    int tempI = a->i, tempJ = a->j;
    double tempValue = a->value;
    a->i = b->i; a->j = b->j; a->value = b->value;
    b->i = tempI; b->j = tempJ; b->value = tempValue;
}

void listSort(matrixNode* current){
    bool swapped;
    matrixNode *root = current;
    do{
        swapped = false;
        while(current->next){
            if(current->i > current->next->i || (current->i == current->next->i && current->j > current->next->j)){ //If i is bigger, swap. OR if i is equal and j is bigger.
                swap(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
        current = root;
    } while(swapped); //When there are no more swaps to be done on the list, exit.
}

void listFileWriter(string fileName, matrixNode* current){
    ofstream writer;
    writer.open(fileName);
    while(current->next){
        if(current->value != 0)    //Only print a node if it isn't zero.
            writer << fixed << current->i << " " << current->j << " " << setprecision(2) << current->value << endl;
        current = current->next;
    }
    writer << fixed << current->i << " " << current->j << " " << setprecision(2) << current->value << endl;
    writer.close();
}

void deleteList(matrixNode* current){   //Loops through the linked list while deleting the previous as it goes.
    while(current->next){
        current = current->next;
        delete current->previous;
    }
    delete current;     //We are at the end of the list. Delete the current node.
}