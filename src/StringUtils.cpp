#include "StringUtils.h"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    std::string Temp = str;
    
    // if end is 0 include until end of string
    if (end == 0){ 
        end = Temp.length();
    }

    // negative start index -> conv to equivalent positive index
    if (start < 0){
        start = start + Temp.length();
        if (start < 0){
            start = 0;
        }
    }

    // negative end index -> conv to equivalent positive index
    if (end < 0){
        end = end + Temp.length();
        if (end < 0){
            end = 0;
        }
    }

    // end index out of bounds to right, set to end of input string
    if (end > Temp.length()){
        end = Temp.length();
    }

    // start index is after end index
    if (start >= end){
        return "";
    }

    return Temp.substr(start, end - start);
}

std::string Capitalize(const std::string &str) noexcept{
    std::string Temp = str;
    
    // capitalize first char in string
    Temp[0] = toupper(Temp[0]);
    return Temp;
}

std::string Upper(const std::string &str) noexcept{
    std::string Temp = str;
    size_t i;
    
    // loop entire string and capitalize each char
    for (i = 0; i < Temp.length(); i++){
        Temp[i] = toupper(Temp[i]);
    }
    return Temp;
}

std::string Lower(const std::string &str) noexcept{
    std::string Temp = str;
    size_t i;

    // loop entire string and lowercase each char
    for (i = 0; i < Temp.length(); i++){
        Temp[i] = tolower(Temp[i]);
    }
    return Temp;
}

std::string LStrip(const std::string &str) noexcept{
    std::string Temp = str;
    size_t len = Temp.length();
    
    // find the first character (first non-whitespace)
    size_t firstChar = Temp.find_first_not_of(" \n\t");
    
    // no characters found in string, return empty string
    if (firstChar == std::string::npos){
        return "";
    }

    // substring only including indices of firstChar and to right
    return Temp.substr(firstChar, len - firstChar);
}

std::string RStrip(const std::string &str) noexcept{
    std::string Temp = str;

    // find the last character (last non-whitespace)
    size_t lastChar = Temp.find_last_not_of(" \n\t");

    // no characters found in string, return empty string
    if (lastChar == std::string::npos){
        return "";
    }

    // substring only from beginning until lastChar (including lastChar)
    return Temp.substr(0,lastChar + 1);
}

std::string Strip(const std::string &str) noexcept{
    std::string Temp = str;

    // find the first character (first non-whitespace)
    size_t firstChar = Temp.find_first_not_of(" \n\t");
    
    // no characters found in string, return empty string
    if (firstChar == std::string::npos){
        return "";
    }

    // find the last character (last non-whitespace)
    size_t lastChar = Temp.find_last_not_of(" \n\t");

    // substring from first character to the last character (inclusive)
    return Temp.substr(firstChar, lastChar - firstChar + 1);
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    std::string Temp = str;
    size_t len = Temp.length();
    size_t fillRight, fillLeft;
    
    // width less than string length --> return entire string
    if (width < len){
        return Temp;
    }

    // total number of items that need to be added = toFill
    size_t toFill = width - len;
    
    // need to add even number of items: split half and half right and left
    if (toFill % 2 == 0){
        fillRight = toFill/2;
        fillLeft = toFill/2;
    }

    // toFill is odd: remaining extra 1 item goes to right
    else {
        fillLeft = toFill/2;
        fillRight = toFill - fillLeft;
    }

    // insert fillRight # of fill chars to end, insert fillLeft # of fill chars to beginning
    Temp.insert(len, fillRight, fill);
    Temp.insert(0, fillLeft, fill);
    return Temp;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    std::string Temp = str;
    size_t len = Temp.length();
    
    // total number of items that need to be added = toFill
    size_t toFill = width - len;

    // width less than string length --> return entire string
    if (width < len){
        return Temp;
    }

    // insert all of toFill to the end
    Temp.insert(len, toFill, fill);
    return Temp;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    std::string Temp = str;
    size_t len = Temp.length();
    
    // total number of items that need to be added = toFill
    size_t toFill = width - len;
    
    // width less than string length --> return entire string
    if (width < len){
        return Temp;
    }

    // insert all of toFill to the beginning
    Temp.insert(0, toFill, fill);
    return Temp;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{ 
    std::string Temp = str;
    std::string Old = old;
    std::string New = rep;
    size_t pos = 0;

    // replace empty string --> nowhere to replace
    if (Old == ""){
        return Temp;
    }

    // loop continuously looks for the next char to be replaced until there are none left
    while ((pos = Temp.find(Old, pos)) != std::string::npos){
        // at current position, remove the Old string and replace with New string
        Temp.replace(pos, Old.length(), New);
        // move position past the New string that was just added
        pos += New.length();
    }
    return Temp;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    std::vector<std::string> Result;
    std::string Temp = str;
    std::string Splt = splt;
    
    // if empty string, return empty vector
    if (Temp == "") return Result;

    // case where the delimiter is an empty string, only add continuous strings of non-whitespace
    if (Splt == "") {
        size_t strt = 0;
        
        // looping through temp
        while (strt < Temp.length()){
            
            // s is the first non-whitespace char, loop breaks if none left
            size_t s = Temp.find_first_not_of(" \n\t", strt);
            if (s == std::string::npos) break;
            
            // e is the first space found, if none left e is the index of Temp
            size_t e = Temp.find(' ', s);
            if (e == std::string::npos) e = Temp.length();
            
            // add to vector: substring from s to e
            Result.push_back(Temp.substr(s, e-s));
            
            // new starting position is 1 index after e
            strt = e + 1;
        }
        return Result;
    }

    // main case 
    size_t strt = 0;

    // looping through Temp
    while (strt <= Temp.length()) {
        
        // set pos to position of first delimiter after strt
        size_t pos = Temp.find(Splt, strt);

        // no more delimeters --> add rest of Temp to the vector and break loop
        if (pos == std::string::npos) {
            Result.push_back(Temp.substr(strt));
            break;
        }

        // delimiter found, add to vector: substring from strt to pos (index of delimiter)
        Result.push_back(Temp.substr(strt, pos-strt));

        // starting position move past the delimiter 
        strt = pos + Splt.length();
        
    }

    return Result;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    std::vector<std::string> Vect = vect;
    std::string Delim = str;
    std::string Temp;

    // loop through vector elements
    for (size_t i = 0; i < Vect.size(); i++){

        // for all elements after the first, add the delimiter to end of the result string Temp
        if (i > 0) Temp += Delim;

        // add the string from position i of the vector to the end of Temp
        Temp += Vect[i];
    }

    return Temp;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    std::string Temp = str;
    size_t pos = 0;
    
    // loop while there is still a tab remaining after current position
    while ((pos = Temp.find('\t', pos)) != std::string::npos){

        // at the location of the tab replace it with 'tabsize' number of spaces
        Temp.replace(pos, 1, std::string(tabsize, ' '));

        // increment position by tabsize
        pos += tabsize;
    } 
    return Temp;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    int count = 0;
    std::string Left = left;
    std::string Right = right;
    int l = Left.length();
    int r = Right.length();

    // if we want to ignore case, convert all to lowercase
    if (ignorecase == true) {
        Left = Lower(Left);
        Right = Lower(Right);
    }

    // if either string is empty edit dist is the length of the other string
    if (l == 0) return r;
    if (r == 0) return l;

    // create dynamic progamming table dp where dp[i][j] represents edit distance between first i chars of Left and first j chars of Right
    std::vector<std::vector<int>> dp(l+1, std::vector<int>(r+1));

    // initialize first column and row of dp
    for (int i = 0; i <= l; i++) dp[i][0] = i;
    for (int j = 0; j <= r; j++) dp[0][j] = j;

    // nested loop through every combination of i and j
    for (int i = 1; i <= l; i++){
        for (int j = 1; j <= r; j++){

            // if characters are equal, no edit --> move to next char in both strings
            if (Left[i-1] == Right[j-1]) dp[i][j] = dp[i-1][j-1];
            
            // otherwise fill dp[i][j] by adding 1 for the edit and the minimum edit distance of the 3 operations
            else dp[i][j] = 1 + std::min({
                dp[i-1][j], // deletion
                dp[i][j-1], // insertion
                dp[i-1][j-1]}); // substitution
        }
    }
    return dp[l][r];
}

};