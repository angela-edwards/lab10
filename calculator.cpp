// string-based calculator

#include <iostream> // include input/output stream library
#include <fstream>  // reading from files
#include <string>   // for using string data type
#include <algorithm>  // for reverse() function
using namespace std;


// function declarations
bool isValidDouble(const string &s);  // checks if string represents a valid double
string addDoubles(const string &a, const string &b);  // adds two valid doubles
string removeLeadingZeros(const string &s);  // helper function to remove leading zeroes
void processFile(const string &filename);  // main function to read from file and process lines

// main function
int main() {
    string filename;    // variable holds file name input
    cout << "enter filename: ";  // asks user for input file
    cin >> filename;   // reads file name from user
    processFile(filename);   // process file
    return 0;  // exit program
}

// checks if string follows valid double number format
bool isValidDouble(const string &s) {
    if (s.empty()) return false;  // empty string -> invalid

    int i = 0;
    if (s[i] == '+' || s[i] == '-') i++;  // skip sign if present
    if ((size_t)i >= s.size()) return false;  // if only '+' or '-', invalid

    bool hasDigitsBefore = false;  // track digits before decimal
    bool hasDigitsAfter = false;   // track digits after decimal
    bool hasDot = false;  // track if decimal point is found

    // loop through all characters in string
    for (; (size_t)i < s.size(); i++) {
        if (isdigit(s[i])) {  // if character is a digit
            if (!hasDot) hasDigitsBefore = true;  // before decimal point
            else hasDigitsAfter = true;  // after decimal point
        }
        else if (s[i] == '.') {  // if found a decimal point
            if (hasDot) return false;   // invalid if more than one '.'
            hasDot = true;
        }
        else return false;  //i invalid if non-digit or non-dot character
    }

    // if there is a dot, make sure digits are on both sides
    if (hasDot && (!hasDigitsBefore || !hasDigitsAfter))
        return false;

    // must have at least one digit
    if (!hasDigitsBefore && !hasDigitsAfter)
        return false;

    return true;  // string passed all checks
}

// removes leading zeroes from number string
string removeLeadingZeros(const string &s) {
    int i = 0;
    // skips all leading zeroes except one if entire string is zeroes
    while (i < (int)s.size() - 1 && s[i] == '0') i++;
    return s.substr(i);   // return substring without leading zeroes
}

// adds two non-negative integers as strings
string addPositiveStrings(const string &a, const string &b) {
    string result = "";  // holds result
    int carry = 0;  // variable for carry-over digit

    int i = a.size() - 1;  // start from last digit of 'a'
    int j = b.size() - 1;  // start from last digit of 'b'

    // loop while there are digits or carry left
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;  // start with carry
        if (i >= 0) sum += a[i--] - '0';  // add digits from 'a'
        if (j >= 0) sum += b[j--] - '0';  // add digits from 'b'
        carry = sum / 10;  // calculate new carry
        result.push_back((sum % 10) + '0');  // store last digit of sum
    }

    reverse(result.begin(), result.end());  // reverse string to correct order
    return result;  // return final result
}

// adds two valid doubles as strings
string addDoubles(const string &aRaw, const string &bRaw) {
    bool negA = false, negB = false;  // flags for negative numbers
    string a = aRaw, b = bRaw;

    // handle signs
    if (a[0] == '+' || a[0] == '-') {
        negA = (a[0] == '-');
        a = a.substr(1);
    }
    if (b[0] == '+' || b[0] == '-') {
        negB = (b[0] == '-');
        b = b.substr(1);
    }

    // splits into integer and fraction parts
    string aInt = a, aFrac = "", bInt = b, bFrac = "";
    size_t dotA = a.find('.');
    size_t dotB = b.find('.');

    // if decimal point found, separate integer and fraction parts
    if (dotA != string::npos) {
        aInt = a.substr(0, dotA);
        aFrac = a.substr(dotA + 1);
    }
    if (dotB != string::npos) {
        bInt = b.substr(0, dotB);
        bFrac = b.substr(dotB + 1);
    }

    // make fractional parts same length
    size_t maxFracLen = max(aFrac.size(), bFrac.size());
    while (aFrac.size() < maxFracLen) aFrac.push_back('0');
    while (bFrac.size() < maxFracLen) bFrac.push_back('0');

    // add fraction parts
    string fracSum = addPositiveStrings(aFrac, bFrac);
    int carry = 0;

    // if fraction sum overflowed    
    if (fracSum.size() > maxFracLen) {
        carry = fracSum[0] - '0';  // store carry
        fracSum = fracSum.substr(1);  // remove overflow digit
    }

    // add integer parts
    string intSum = addPositiveStrings(aInt, bInt);
    if (carry) intSum = addPositiveStrings(intSum, "1");
    
    // remove trailing zeroes from fraction result
    while (!fracSum.empty() && fracSum.back() == '0')
        fracSum.pop_back();

    // combine integer and fraction parts
    string result = intSum;
    if (!fracSum.empty()) result += '.' + fracSum;

    // apply negative sign if both numbers were negative
    if (negA && negB)
        result = "-" + result;

    return result;  // return final sum
}

// reads two strings per line from file, validates, and adds
void processFile(const string &filename) {
    ifstream file(filename);  // open file
    if (!file.is_open()) {   // check if file opened
        cout << "error: cannot open file.\n";
        return;
    }

    string a, b;  // strings to hold each pair of numbers

    // read two strings at a time until end of file
    while (file >> a >> b) {
        bool validA = isValidDouble(a);   // validate first number
        bool validB = isValidDouble(b);   // validate second number

        // if either number is invalid, report and skip
        if (!validA || !validB) {
            cout << "Invalid number(s): ";
            if (!validA) cout << a << " ";
            if (!validB) cout << b;
            cout << endl;
            continue;  // move to next line
        }

        // if both are valid, add
        string sum = addDoubles(a, b);
        cout << a << " + " << b << " = " << sum << endl;
    }

    file.close();  // close file when done
}