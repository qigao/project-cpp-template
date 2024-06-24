#include "diceparser.h"

namespace DP{
    %%{
        machine RollFormula;
        
        action error{
            throw RollFormulaError("[ERROR w/ formula] incorrect formula");
            fret;
        }
        action leftBracket{
            numPut = true;
            operation.push(static_cast<RollOperation>(fc));
        }
        action rightBracket{
            numPut = true;
            char topOfStack = operation.empty() ? ' ' : operation.top();
            if (operation.empty()) fcall errors;
            else if (operation.top() == '(')
            operation.pop();
            else{
                RollFormula rollFormula(static_cast<RollOperation>(topOfStack));
                rf.addEnd(rollFormula);
                operation.pop();
                fhold;
            }
        }
        action digitOfNumber{
            intVal = intVal * 10 + (fc - '0');
        } 
        action endOfNumber{
            RollFormula rollFormula(intVal);
            rf.addEnd(intVal);
            intVal = 0;
            numPut = true;
        }
        action operationLvl5{
            numPut = false;
            char topOfStack = operation.empty() ? ' ' : operation.top();
            if (operation.empty() || topOfStack == '+' || topOfStack == '-' || topOfStack == '(' ){
                operation.push(static_cast<RollOperation>(fc));
                exclamationMark = false;
            }
            else{
                RollFormula rollFormula(static_cast<RollOperation>(topOfStack));
                rf.addEnd(rollFormula);
                operation.pop();
                fhold;
            }
        }
        action operationLvl6{
            numPut = false;
            char topOfStack = operation.empty() ? ' ' : operation.top();
            if (operation.empty() || topOfStack == '(' ){
                operation.push(static_cast<RollOperation>(fc));
                exclamationMark = false;
            }
            else{
                RollFormula rollFormula(static_cast<RollOperation>(topOfStack));
                rf.addEnd(rollFormula);
                operation.pop();
                fhold;
            }
        }
        action operationD{
            char topOfStack = operation.empty() ? ' ' : operation.top();
            if (operation.empty() || topOfStack == '+' || topOfStack == '-' || topOfStack == '*' || topOfStack == '/' || topOfStack == '(' || topOfStack == '%'){
                if (!numPut){
                    RollFormula rollFormula(1);
                    rf.addEnd(rollFormula);
                    numPut = false;
                }
                operation.push(static_cast<RollOperation>(fc));
            }
            else{
                RollFormula rollFormula(static_cast<RollOperation>(topOfStack));
                rf.addEnd(rollFormula);
                operation.pop();
                fhold;
            }
        }
        action operationMm{
            char topOfStack = operation.empty() ? ' ' : operation.top();
            if (topOfStack == 'd' && !exclamationMark){
                operation.push(static_cast<RollOperation>(fc));
            }
            else fcall errors;
        }
        action operationExclamationMark{
            char topOfStack = operation.empty() ? ' ' : operation.top();
            if (topOfStack == 'd'){
                RollFormula rollFormula(static_cast<RollOperation>(fc));
                rf.addEnd(rollFormula);
                exclamationMark = true;
            }else fcall errors;
        }
        action end{
            if (!operation.empty()){
                if (operation.top() == '('){
                    fcall errors;
                }else{
                    while (!operation.empty()){
                        RollFormula rollFormula(operation.top());
                        rf.addEnd(rollFormula);
                        operation.pop();
                    }
                    fhold;
                }
            }
        }
        
        errors := []
        $~error;
        
        brackets = ('(' @leftBracket | ')' @rightBracket);
        operations = ([\-\+] @operationLvl6 | [\*\/\%] @operationLvl5 | 'd' @operationD);
        numbers = ([1-9][0-9]**) @digitOfNumber %endOfNumber;
        modifications =([mM] @operationMm |'!' @operationExclamationMark);
        eol = [\n\0] $end;
        main := ((brackets | operations | numbers | modifications | ' ')** . eol) @!error;
    }%%
    %% write data;

    std::istream &operator>>(std::istream &in, RollFormula &rf){
        std::stack<RollOperation> operation = std::stack<RollOperation>();
        bool numPut = false;
        int intVal = 0;
        bool exclamationMark = false;
        char stack[100];
        int cs;
        int top;

        %% write init;

        try{
            std::string str;
            std::getline(in, str);
            const char *p = str.c_str();
            const char *pe = p + str.length() + 1;
            const char *eof = pe;
            const char *ts = nullptr;

            %% write exec;

        }catch (RollFormulaError &e){
            throw e;
        }
        while (!operation.empty())
            operation.pop();
        return in;
    }
}
