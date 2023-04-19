/*
Author: Preedhi Garg
Course: 5910 01 Software Refactoring & Design
Date: 13 March 2023

CodeSmellDetector.cpp : This file contains the 'main' function. Program execution begins and ends there.
-> Detects 5 types of code smell
-> Takes "input.cpp" file as input 
-> I have included two extra code smells for extra credit.

Welcome to Code Smell Detection!
The file you input contains the following methods / functions: fun1 fun9 fun2 fun4 fun3 camelCase 
Please choose what you want to do now:
1. Long Method/Function Detection
2. Long Parameter List Detection
3. Duplicated Code Detection
4. Bad Naming(CamelCase) Detection
5. Long Method Name Detection
6. Quit

To run:
g++ -o CodeSmellDetector CodeSmellDetector.cpp
./CodeSmellDetector


*/

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <set>

using namespace std;

/*
if a method/function contains 16 or more lines of code,
we will say the code smell Long Method/Function exists
*/
const int longMethodThreshold = 15;

/*
if a method/function contains 4 or more parameters, we will
say this method/function has a long parameter list.
*/
const int longParameterThreshold = 3;

/*
we define 0.75 as the threshold. There are several metrics you can use here, such as Jaccard Similarity.
*/
const double similarityThreshold = 0.75; 

/*
if a method/function's name is more than 20 characters
*/
const int longMethodNameThreshold = 20;

struct Function {
    std::string fullName;
    std::string name;
    std::vector<std::string> arguments;
    std::string body;
};


std::string extract_function_body(const std::string& code, const std::string& function_declaration) {
    // Find the position of the function declaration in the code
    size_t pos = code.find(function_declaration);
    if (pos == std::string::npos) {
        // Function not found, return empty string
        return "";
    }

    // Find the position of the opening brace after the function declaration
    size_t brace_pos = code.find('{', pos);
    if (brace_pos == std::string::npos) {
        // Opening brace not found, return empty string
        return "";
    }

    // Find the position of the closing brace after the opening brace
    size_t open_braces = 1;
    size_t close_brace_pos = brace_pos + 1;
    while (open_braces > 0 && close_brace_pos < code.length()) {
        if (code[close_brace_pos] == '{') {
            open_braces++;
        }
        else if (code[close_brace_pos] == '}') {
            open_braces--;
        }
        close_brace_pos++;
    }

    // Extract the function body between the opening and closing braces
    std::string body = code.substr(brace_pos + 1, close_brace_pos - brace_pos - 2);

    //// Trim leading and trailing whitespace
    //body.erase(0, body.find_first_not_of(" \n\r\t"));
    //body.erase(body.find_last_not_of(" \n\r\t") + 1);

    // Return the function body
    return body;
}


std::vector<Function> detectFunctions(const std::string& input) {
    std::vector<Function> functions;

    // Regular expression to match function declarations
    //std::regex functionRegex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\s+[a-zA-Z_][a-zA-Z0-9_]*\\s*\\((.*?)\\)\\s*\\{([^}]+)\\}");
    //std::regex functionRegex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\s+[a-zA-Z_][a-zA-Z0-9_]*\\s*\\((.*?)\\)\\s*\\{([^}]+)\\}");
    std::regex functionRegex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\s+[a-zA-Z_][a-zA-Z0-9_]*\\s*\\((.*?)\\)\\s*\\{");
    
    

    // Search for function declarations in input string
    std::sregex_iterator iter(input.begin(), input.end(), functionRegex);
    std::sregex_iterator end;

    for (; iter != end; ++iter) {
        Function function;

        // Extract function name
        std::string functionName = iter->str();
        function.fullName = functionName;
        functionName.erase(functionName.find("("));
        functionName = functionName.substr(functionName.rfind(" ") + 1);
        function.name = functionName;

        // Extract function arguments
        std::string arguments = iter->str(1);
        std::regex argumentRegex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\s+[a-zA-Z_][a-zA-Z0-9_]*\\b");
        std::sregex_iterator argIter(arguments.begin(), arguments.end(), argumentRegex);
        std::sregex_iterator argEnd;
        for (; argIter != argEnd; ++argIter) {
            function.arguments.push_back(argIter->str());
        }

        function.body = extract_function_body(input, function.fullName);
        functions.push_back(function);
    }

    return functions;
}

void print_instructions(std::vector<string> functionList)
{
    cout<<endl;
    cout<<"*******************************************************************"<<endl;
    cout << endl<< "Welcome to Preedhi's Code Smell Detection!" << endl;
    cout<<"*******************************************************************"<<endl;
    cout<<endl;
    cout << "The file you input contains the following methods / functions: ";
    for (auto fname : functionList)
        cout << fname << " ";
    cout << endl;
    cout << "Please choose what you want to do now:" << endl;
    cout << "1. Long Method/Function Detection" << endl;
    cout << "2. Long Parameter List Detection" << endl;
    cout << "3. Duplicated Code Detection" << endl;
    cout << "4. Bad Naming(CamelCase) Detection" << endl;
    cout << "5. Long Method Name Detection" << endl;
    cout << "6. Quit" << endl;
    cout << endl;
    std::cout << "Enter your choice... ";
}


int count_lines(const std::string& input) {
    int lines = 1; // Start with one line since there's always at least one
    for (char c : input) {
        if (c == '\n') { // Check for newline characters
            ++lines;
        }
    }
    return lines;
}

void validate_longMethod(vector<Function> functions)
{
    bool longMethodFound = false;
    for (auto func : functions)
    {
        int noOfLines = count_lines(func.body);
        if (noOfLines > longMethodThreshold)
        {
            longMethodFound = true;
            cout << "The " << func.name <<" function is a Long Function. It contains " << noOfLines << " lines of code" << endl;
        }
    }

    if (!longMethodFound)
    {
        cout << "No function is a Long Function." << endl;
    }

}

void validate_longParameter(vector<Function> functions)
{
    bool longParameterList = false;
    for (auto func : functions)
    {
        int paramCount = func.arguments.size();
        if (paramCount > longParameterThreshold)
        {
            longParameterList = true;
            cout << func.name << " has a Long Parameter List. Its parameter list contains " << paramCount << " parameters." << endl;
        }
    }

    if (!longParameterList)
    {
        cout << "No function has a long parameter list." << endl;
    }
}

double jaccard_similarity(string s1, string s2) {
    set<char> set1(s1.begin(), s1.end());
    set<char> set2(s2.begin(), s2.end());
    set<char> intersection;
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(intersection, intersection.begin()));
    set<char> union_set;
    set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(union_set, union_set.begin()));
    return (double)intersection.size() / (double)union_set.size();
}

void validate_duplicateCode(vector<Function> functions)
{
    bool duplicateExists = false;
    for (int i = 0;i < functions.size();i++)
    {
        for (int j = i + 1;j < functions.size();j++)
        {
            double jaccardValue = jaccard_similarity(functions[i].body, functions[j].body);

            if (jaccardValue > similarityThreshold)
            {
                duplicateExists = true;
                cout << functions[i].name << " and " << functions[j].name << " are duplicated." << endl;
            }
        }
    }

    if (!duplicateExists)
    {
        cout << "No functions are duplicated." << endl;
    }
}

bool isCamelCase(const std::string& str) {
    // Check if the string is empty
    if (str.empty()) {
        return false;
    }

    // Check if the first character is lowercase
    if (!islower(str[0])) {
        return false;
    }

    // Check if the string contains an uppercase character
    bool hasUppercase = false;
    for (char c : str) {
        if (isupper(c)) {
            hasUppercase = true;
            break;
        }
    }
    if (!hasUppercase) {
        return false;
    }

    // Check if the string contains any whitespace
    for (char c : str) {
        if (isspace(c)) {
            return false;
        }
    }

    // Check if the string contains any punctuation
    for (char c : str) {
        if (ispunct(c)) {
            return false;
        }
    }

    // Check if the string contains any consecutive uppercase characters
    for (int i = 1; i < str.length(); i++) {
        if (isupper(str[i]) && isupper(str[i - 1])) {
            return false;
        }
    }

    return true;
}

void validate_camelcase(vector<Function> functions)
{
    bool validCamelCase = true;
    for (auto func : functions)
    {
        if (!isCamelCase(func.name))
        {
            validCamelCase = false;
            cout << func.name << " is not CamelCased" << endl;
        }
    }

    if (validCamelCase)
    {
        cout << "Every function is CamelCased" << endl;
    }
}

void validate_longmethodname(vector<Function> functions)
{
    bool longMethodNameExist = false;
    for (auto func : functions)
    {
        int functionNameLen = func.name.length();
        if (functionNameLen > longMethodNameThreshold)
        {
            longMethodNameExist = true;
            cout << func.name << " is too long. It's name contains " << functionNameLen << "characters." << endl;

        }
    }

    if (!longMethodNameExist)
    {
        cout << "No method name is too long." << endl;
    }
}

void start_application(vector<Function> functions)
{
    std::vector<string> functionList;
    for (auto func : functions)
        functionList.push_back(func.name);

    std::string input;

    while (true) {
        print_instructions(functionList);
        
        std::getline(std::cin, input);

        // Perform some action for each input, for example:
        std::cout << endl<< "You entered: " << input << std::endl;
        
        if (input == "1") {
            validate_longMethod(functions);
        }
        else if (input == "2") {
            validate_longParameter(functions);
        }
        else if (input == "3")
        {
            validate_duplicateCode(functions);
        }
        else if (input == "4")
        {
            validate_camelcase(functions);
        }
        else if (input == "5")
        {
            validate_longmethodname(functions);
        }
        else if (input == "6") {
            break; // Exit the loop if the user enters "6"
        }
        else
        {
            cout << "Invalid input" << endl;
        }

    }

    std::cout << "Exiting program." << std::endl;
}

void print_functions(vector<Function> functions)
{
    for (const auto& function : functions) {
        std::cout << "FullName: " << function.fullName << std::endl;
        std::cout << "Name: " << function.name << std::endl;
        std::cout << "Arguments: ";
        for (const auto& argument : function.arguments) {
            std::cout << argument << " ";
        }
        cout << endl;
        std::cout << "Body:" << function.body << std::endl;
    }
}


int main() {

    // Read input file
    string filePath = "input.cpp"; // input file to operate on
    ifstream file(filePath);

    if (!file) {
        cerr << "Unable to open file " << filePath << endl;
        return 1;
    }

    string fileContent((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
    file.close();

    // Parse input file
    std::vector<Function> functions = detectFunctions(fileContent);
    start_application(functions);

    return 0;
}

