#include <iostream>
#include <fstream>
#include <istream>
#include <string>
using namespace std;

const int MAXRESULTS = 20; //Max matches that can be found
const int MAXDICTWORDS = 30000; //Max words that can be read in

int loadDictionary(ifstream &dictfile, string dict[]);
int permuteRecur(string word, const string dict[], int size, string results[]);
void printRecur(const string results[], int size);
bool loop_check(string word, string dict, int size, bool status);

//for  basic test
#include <algorithm>
#include <cassert>

void doBasicTests()
{
    const int TESTINGMAX = 10;
    {
        string dictionary[TESTINGMAX];
        ifstream dictfile("smallwords.txt");
//        ifstream dictfile("/Users/junnymac/Desktop/Fall 2021/CSCI 02 0130/smallwords.txt");
        if (!dictfile) {
            cout << "smallwords.txt file not found!" << endl;
            exit (1);
        }
        int numResults = loadDictionary(dictfile, dictionary);
        std::sort(dictionary, dictionary + numResults);
        assert(numResults == 5 && dictionary[0] == "act" && dictionary[numResults-1] == "tac");
    }
    {
        string results[TESTINGMAX];
        string example[ ] = { "act", "cat", "camino", "rat", "tac" };
        int numResults = permuteRecur("tca", example, 5, results);
        std::sort(results, results + numResults);
        assert(numResults == 3 && results[0] == "act" && results[1] == "cat" && results[2] == "tac");
    }
    {
        string results[TESTINGMAX];
        string example[ ] = { "kool", "moe", "dee" };
        int numResults ;
        numResults = permuteRecur("look", example, 1, results);
        assert(numResults == 1 && results[0] == "kool");
    }
    cout << "Passed all basic tests" << endl;
}
//

int main() {
    string results[MAXRESULTS];
    string dict[MAXDICTWORDS];
    ifstream dictfile;  //file containing the list of words
    int nwords; //number of words read from dictionary
    string word;

//    dictfile.open("/Users/junnymac/Desktop/Fall 2021/CSCI 02 0130/words.txt");
    dictfile.open("words.txt"); //input file open
    if(!dictfile){  //check error
        cout<<"File not found!"<<endl;
        return(1);
    }

    nwords = loadDictionary(dictfile, dict);
    if(nwords > MAXDICTWORDS){
        cout<<"Too large load file"<<endl;
        return(2);
    }

    cout<<"Please enter a string for an anagram: ";
    cin>>word;  //prompt string value to find
    int numMatches = permuteRecur(word, dict, nwords, results);
    if(numMatches > MAXRESULTS){
        cout<<"Too large results"<<endl;
        return(3);
    }
    if (numMatches == 0)
        cout<<"No matches found"<<endl;
    else
        printRecur(results, numMatches);

    //for basic tests
    doBasicTests();

}

//reading value from input file and saving to the dict array.
int loadDictionary(ifstream &dictfile, string dict[]){  //OK
    char line[40];
    if (dictfile.getline(line, 40)){
        *dict = line;
        return loadDictionary(dictfile, dict+1) + 1;
    }
}

//comparing contained characters of the word each step by step
// between the word from input and the word from the dict array
//if all contained words are the same, it will return true.
bool loop_check(string word, string dict, int size, bool status){   //OK
    if (size == 0 && status == true) {
        return true;
    }else if (size == 0 && status == false) {
        return false;
    }
    //check each character
    if (word[0] == dict[size-1]) {
        //reducing size of word after checking a character
        word = word.substr(1);
        dict = dict.substr(0, size - 1)+dict.substr(size);
        return loop_check(word, dict, dict.length(), true);
    }else {
        return loop_check(word,dict,size-1,false);
    }
}

//first, comparing the length of words. if it is the same, call flag_loop function
//to check the same contained character. if it is true, save the value to
// the results array or keep searching.
int permuteRecur(string word, const string dict[], int size, string results[]){ //OK
    if (size == 0){
        return 0;
    }
    //check the same length of words
    if (word.length() == (*dict).length()){
        //check contained the same character or not
        bool flag_loop = loop_check(word,*dict,word.length(),false);
        if (flag_loop){
            //if it is true, save it to the results array
            *results = *dict;
            return permuteRecur(word, dict+1, size-1, results+1)+1;
        }else{
            return permuteRecur(word, dict+1, size-1, results);
        }
    }else{
        return permuteRecur(word, dict+1, size-1, results);
    }
}

//printing searching word in the results array.
void printRecur(const string results[], int size){  //OK
    if(size > 0){
        cout<<"Matching word "<<*(results)<<endl;
        return printRecur(results+1, size-1);
    }
}

