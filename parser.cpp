#include <cstring>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>

const char* ACCEPTED_CHARACTER_ALPHANUMERIC = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char* ACCEPTED_CHARACTER_ALPHABETICAL = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* ACCEPTED_CHARACTER_NUMERICAL = "0123456789";
const char* ACCEPTED_CHARACTER_IDENTIFIER = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

class Eater {
    public:
    
    int line = 0;
    const char *string;
    const char *c;
    
    Eater(const char *string) {
        this->string = string;
        this->c = this->string;
    }
    
    bool trimW() {
        bool trimmed = false;
        while(*c == ' ') {
            c++;
            trimmed = true;
        }
        return trimmed;
    }
    
    bool  trimWN() {
        bool trimmed = false;
        while(*c == ' ' || *c == '\n') {
            if(*c == '\n') line++;
            c++;
            trimmed = true;
        }
        return trimmed;
    }
    
    bool eatW(char e) {
        trimW();
        if(*c == e) {
            if(*c == '\n') line++;
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
    
    std::string eatWord(const char* acceptedCharacters) {
        std::string word;
        const char* character;
        
        if(*c == 0) goto label_return;
        
        label_eatCharacter:
        character = acceptedCharacters;
        while(*character != *c && *character != 0) {
            character++;
        }
        
        if(*character == *c) {
            word += *character;
            c++;
            goto label_eatCharacter;
        }
        
        if(*character == 0) goto label_return;
        
        label_return:
        return word;
    }
};

std::string readLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void problem(Eater* eater, const char* msg) {
    printf("Problem on line %d: %s\n", eater->line, msg);
    exit(1);
}



enum RuleNodeType {
    RNT_BLOCK,
    RNT_OPTIONAL_BLOCK,
    RNT_OPTION,
    RNT_REGEX,
    RNT_RULE,
    RNT_SYMBOL
};

enum RuleCount {
    RC_ZERO_OR_MORE,
    RC_ZERO_OR_ONE,
    RC_ONE,
    RC_ONE_OR_MORE
};

struct RuleNode {
    RuleNodeType type;
    RuleCount count = RC_ONE;
    
    union {
        std::vector<RuleNode*>* block;
        std::vector<RuleNode*>* optionalBlock;
        std::vector<RuleNode*>* option;
        std::regex* regex;
        char* rule;
        char* symbol;
    } data;
    
    RuleNode() {}
    
    RuleNode(const RuleNode& copy) {
        this->type = copy.type;
        this->count = copy.count;
        this->data = copy.data;
    }
    
    RuleNode(RuleNodeType type) {
        switch (type) {
            case RNT_BLOCK:
                data.block = new std::vector<RuleNode*>();
                break;
            case RNT_OPTIONAL_BLOCK:
                data.optionalBlock = new std::vector<RuleNode*>();
                break;
            case RNT_OPTION:
                data.option = new std::vector<RuleNode*>();
                break;
            case RNT_REGEX:
                data.regex = 0;
                break;
            case RNT_RULE:
                data.rule = 0;
                break;
            case RNT_SYMBOL:
                data.symbol = 0;
                break;
        }
    }
    
    void unallocate() {
        switch (type) {
            case RNT_BLOCK:
                for(RuleNode* node : *data.block) {
                    node->unallocate();
                }
                delete data.block;
                break;
            case RNT_OPTIONAL_BLOCK:
                for(RuleNode* node : *data.optionalBlock) {
                    node->unallocate();
                }
                delete data.optionalBlock;
                break;
            case RNT_OPTION:
                for(RuleNode* node : *data.option) {
                    node->unallocate();
                }
                delete data.option;
                break;
            case RNT_REGEX:
                delete data.regex;
                break;
            case RNT_RULE:
                free(data.rule);
                break;
            case RNT_SYMBOL:
                free(data.symbol);
                break;
        }
    }
    
    void allowMultiple() {
        if(count == RC_ZERO_OR_ONE) count = RC_ZERO_OR_MORE;
        else if(count == RC_ONE) count = RC_ONE_OR_MORE;
        else printf("compiler bug: can't increase node's current count");
    }
    
    ~RuleNode() {
        
    }
    
    RuleNode createBlockNode(std::vector<RuleNode*>* block) {
        RuleNode node;
        node.type = RNT_BLOCK;
        node.data.block = block;
        
        return node;
    }
    RuleNode createOptionalBlockNode(std::vector<RuleNode*>* optionalBlock) {
        RuleNode node;
        node.count = RC_ZERO_OR_ONE;
        node.type = RNT_OPTIONAL_BLOCK;
        node.data.optionalBlock = optionalBlock;
        
        return node;
    }
    RuleNode createOptionNode(std::vector<RuleNode*>* option) {
        RuleNode node;
        node.count = RC_ONE;
        node.type = RNT_OPTION;
        node.data.option = option;
        
        return node;
    }
    RuleNode createRegexNode(std::regex* regex) {
        RuleNode node;
        node.type = RNT_REGEX;
        node.data.regex = regex;
        
        return node;
    }
    RuleNode createRuleNode(char* rule) {
        RuleNode node;
        node.type = RNT_RULE;
        node.data.rule = rule;
        
        return node;
    }
    RuleNode createSymbolNode(char* symbol) {
        RuleNode node;
        node.type = RNT_SYMBOL;
        node.data.symbol = symbol;
        
        return node;
    }
};

RuleNode* parseRuleNode(Eater* eater) {
    RuleNode* node = new RuleNode();
    
    eater->trimWN();
    
    switch (*eater->c) {
        case '(': {
            eater->c++;
            
            std::vector<RuleNode*> *children = new std::vector<RuleNode*>();
            
            do {
                RuleNode* option = new RuleNode();
                option->createOptionNode(new std::vector<RuleNode *>());
                
                do {
                    RuleNode* node = parseRuleNode(eater);
                    
                    if(!node)   problem(eater, "Problem in block rule");
                    else        option->data.option->push_back();
                } while(eater->trimWN() || *eater->c != ')');
                
                option->data.option->push_back(node);
            } while(eater->trimWN() || *eater->c != '|');
            
            
            if(!eater->eatWN(')')) problem(eater, "Missing closing parenthesis");
            
            node->createBlockNode(children);
        }
        break;
        case '[': {
            eater->c++;
            
            std::vector<RuleNode*> *children = new std::vector<RuleNode*>();
            
            do {
                RuleNode* node = parseRuleNode(eater);
                
                if(!node)   problem(eater, "Problem in block rule");
                else        children->push_back(node);
            } while(eater->trimWN() && *eater->c != ']');
            
            if(!eater->eatWN(']')) problem(eater, "Missing closing braces");
            
            node->createOptionalBlockNode(children);
        }
        break;
        case '{': {
            eater->c++;
            
            std::string pattern;
            
            while(*eater->c != '}') {
                if(*eater->c == '\\') {
                    eater->c++;
                    switch (*eater->c) {
                        case '\\':
                            pattern += '\\';
                            break;
                        case '}':
                            pattern += '}';
                            break;
                        default:
                            problem(eater, "Unknown escape sequence");
                            break;
                    }
                } else {
                    pattern += *eater->c;
                }
                eater->c++;
            }
            
            if(!eater->eatWN('}')) problem(eater, "Missing closing brackets");
            
            std::regex* regex = new std::regex(pattern);
            node->createRegexNode(regex);
        }
        break;
        case '\'': {
            eater->c++;
            
            std::string text;
            
            while(*eater->c != '\'') {
                if(*eater->c == '\\') {
                    eater->c++;
                    switch (*eater->c) {
                        case '\\':
                            text += '\\';
                            break;
                        case '\'':
                            text += '\'';
                            break;
                        default:
                            problem(eater, "Unknown escape sequence");
                            break;
                    }
                } else {
                    text += *eater->c;
                }
                eater->c++;
            }
            
            if(!eater->eatWN('\'')) problem(eater, "Missing closing quote");
            
            char* symbol = (char*) malloc(text.size()+1);
            strcpy(symbol, text.c_str());
            
            node->createSymbolNode(symbol);
        }
        break;
        default: {
            // try reading identifier
            std::string word = eater->eatWord(ACCEPTED_CHARACTER_IDENTIFIER);
            
            char* symbol = (char*) malloc(word.size()+1);
            strcpy(symbol, word.c_str());
            
            if(word.empty()) problem(eater, "Expected an identifier");
            else node->createSymbolNode(symbol);
        }
        break;
    }
    
    return node;
}

void parseRule(Eater* eater, std::map<std::string, RuleNode*> rules) {
    eater->trimWN();
    
    std::string ruleName = eater->eatWord(ACCEPTED_CHARACTER_IDENTIFIER);
    
    if(ruleName.empty()) {
        problem(eater, "Rule must start with a name");
        return;
    }
    
    if(!eater->eatWN('=')) {
        problem(eater, "Expected '=' after rule name");
        return;
    }
    
    eater->trimWN();
    
    RuleNode* ruleNode;
    if(!(ruleNode = parseRuleNode(eater))) {
        problem(eater, "Expected a rule definition");
        return;
    }
    
    if(!eater->eatWN(';')) {
        problem(eater, "Expected a semicolon after rule definition");
        return;
    }
    
    if(rules[ruleName]) {
        problem(eater, "Rule name already taken");
        return;
    }
    
    rules[ruleName] = ruleNode;
}




char* readEntireFile(const char* name) {

    FILE *file = fopen(name, "rb");
    
    if(file == 0) {
        printf("file doesnt' exist\n");
        return 0;
    }

    // Find file size
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate big enough buffer (+ null-termiator)
    char* buffer = (char*)malloc(fsize + 1);
    fread(buffer, 1, fsize, file);
    buffer[fsize] = 0;

    fclose(file);

    return buffer;
}

int main(int charc, char** chars) {
    if(charc < 2) {
        printf("Expected a file path\n");
        return 0;
    }
    
    const char* file = readEntireFile(chars[1]);
    
    Eater eater = Eater(file);
    std::map<std::string, RuleNode*> rules;
    
    while(*eater.c != 0) {
        parseRule(&eater, rules);
    }
    
    return 0;
}

/*
bool running = true;
while(running) {
    std::string line = readLine();
    Eater eater = Eater(line.c_str());
    
    printf("ACCEPTED_CHARACTER_ALPHANUMERIC: %s\n", eater.eatWord(ACCEPTED_CHARACTER_ALPHANUMERIC).c_str());
    eater.c = eater.string;
    printf("ACCEPTED_CHARACTER_ALPHABETICAL: %s\n", eater.eatWord(ACCEPTED_CHARACTER_ALPHABETICAL).c_str());
    printf("ACCEPTED_CHARACTER_IDENTIFIER: %s\n", eater.eatWord(ACCEPTED_CHARACTER_IDENTIFIER).c_str());
    eater.c = eater.string;
    printf("ACCEPTED_CHARACTER_NUMERICAL: %s\n", eater.eatWord(ACCEPTED_CHARACTER_NUMERICAL).c_str());
    eater.c = eater.string;
}
*/