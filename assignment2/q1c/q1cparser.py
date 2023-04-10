import sys
from typing import Tuple

inputPtr = 0

#Function for parsing the start symbol of the grammar
#Returns a tuple of a boolean and a string. The boolean represents whether the parse is successful, and the string represents the output of the parse (if the parse is successful)
#Grammar: S -> aS | bS | [S]S | epsilon
def parseS(inputString: str) -> Tuple[bool,str]:
    global inputPtr

    #For the production rule S -> epsilon
    if(inputPtr == len(inputString)): #End of input
        return (True, "")
    elif inputString[inputPtr] == ']': #Notice that ']' belongs to the follow set of S
        return (True, "")
        
    #For the production rule S -> aS
    elif inputString[inputPtr] == 'a':
        inputPtr += 1
        parseSuccess, outputStr = parseS(inputString)
        return (True, "a" + outputStr) if parseSuccess else (False, None)
    
    #For the production rule S -> bS
    elif inputString[inputPtr] == 'b':
        inputPtr += 1
        parseSuccess, outputStr = parseS(inputString)
        return (True, "b" + outputStr) if parseSuccess else (False, None)
    
    #For the production rule S -> [S]S
    elif inputString[inputPtr] == '[':
        inputPtr += 1
        parseSuccess, outputStr = parseS(inputString)
        if not parseSuccess:
            return False, ""
        if inputPtr == len(inputString) or inputString[inputPtr] != "]": #Cannot find closing bracket!
            return False, ""
        #Succesfully parsed [S], then the remaining input is the S at the right
        inputPtr += 1
        repeatedString = outputStr + outputStr
        parseSuccess, outputStr = parseS(inputString)
        return (True, repeatedString + outputStr) if parseSuccess else (False, None)

    else:
        return (False, "")

#Main function. Entry point of the program.
if __name__ == "__main__":
    while True:
        inputString = input()
        if inputString == "quit":
            exit()
        else:
            inputPtr = 0 #Reset the ptr
            parseSuccess, outputStr = parseS(inputString)
            if(parseSuccess and inputPtr == len(inputString)): #If the parse did not go through the whole input string, there is an error.
                print(outputStr)
            else:
                print("Boom!")
                