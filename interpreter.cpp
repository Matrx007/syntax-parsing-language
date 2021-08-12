#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>

class Eater {
    public:
    
    int line;
    const char *string;
    const char *c;
    
    void trimW() {
        while(*c == ' ') {
            c++;
        }
    }
    
    void trimWN() {
        while(*c == ' ' || *c == '\n') {
            if(*c == '\n') line++;
            c++;
        }
    }
    
    bool eatW(char e) {
        trimW();
        if(*c == e) {
            if(e == '\n') line++;
            c++;
            return true;
        }
        return false;
    }
    
    bool eatWN(char e) {
        trimWN();
        if(*c == e) {
            if(*c == '\n') line++;
            c++;
            return true;
        }
        return false;
    }
    
    
    bool isW(char e) {
        trimW();
        if(*c == '\n') line++;
        return (*c == e);
    }
    
    bool isWN(char e) {
        trimWN();
        if(*c == '\n') line++;
        return (*c == e);
    }
};

std::string readLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}



enum SyntaxTokenType {
    STT_NONE,
    STT_IDENTIFIER,
    STT_ASSIGNMENT,
    STT_STRING,
    STT_INT,
    STT_FLOAT,
    STT_OPERATION,
    STT_KEYWORD,
    STT_ADD,
    STT_SUBTRACT,
    STT_MULTIPLY,
    STT_DIVIDE,
    STT_PARENTHESIS_OPEN,
    STT_PARENTHESIS_CLOSE
};

std::map<SyntaxTokenType, std::regex> tokenMatchers = {
    { STT_IDENTIFIER,           std::regex("[a-zA-Z_]+") },
    { STT_ASSIGNMENT,           std::regex("=") },
    { STT_STRING,               std::regex("[^\\\\]\"([^\"]*)\"") },
    { STT_INT,                  std::regex("[0-9]+") },
    { STT_FLOAT,                std::regex("[0-9]*\\.[0-9]+") },
    { STT_OPERATION,            std::regex("[a-z]+") },
    { STT_KEYWORD,              std::regex("[a-z]+") },
    { STT_ADD,                  std::regex("\\+") },
    { STT_SUBTRACT,             std::regex("\\-") },
    { STT_MULTIPLY,             std::regex("\\*") },
    { STT_DIVIDE,               std::regex("\\/") },
    { STT_PARENTHESIS_OPEN,     std::regex("\\(") },
    { STT_PARENTHESIS_CLOSE,    std::regex("\\)") }
};

struct SyntaxToken {
    SyntaxTokenType type;
    std::string content;
};

enum StatementType {
    ST_NONE,
    ST_ASSIGNMENT,
    ST_OUT,
    ST_VALUE
};

enum StatementComponentType {
    SCT_STATEMENT_TYPE,
    SCT_SYNTAX_TOKEN_TYPE
};

struct StatementComponent {
    StatementComponentType type;
    // union {
        StatementType statementType;
        SyntaxTokenType syntaxTokenType;
    // };
};

std::map<StatementType, std::vector<StatementComponent>> syntax = {
    { ST_VALUE 
    },
    { ST_ASSIGNMENT, {
        { SCT_SYNTAX_TOKEN_TYPE, ST_NONE, STT_IDENTIFIER }, 
        { SCT_SYNTAX_TOKEN_TYPE, ST_NONE, STT_ASSIGNMENT }, 
        { SCT_STATEMENT_TYPE, ST_VALUE, STT_NONE } 
    }}
};



enum DataType {
    DT_STRING,
    DT_INT,
    DT_FLOAT
};

class Variable {
    public: 
    
    DataType type;
    void* dataPointer;
    
    void freeData() {
        
    }
};

struct ParserState {
    std::map<std::string, Variable> variables;
    
    std::string parseBuffer;
};

bool parseInput(std::string line) {
    
}



int main(int charc, char** chars) {
    bool running = true;
    
    while(running) {
        std::string line = readLine();
        
        parseInput(line);
    }
    
    return 0;
}