/*
 * This program matches the whole file
 * You have to put separate inputs to separate text files
 */

%option noyywrap

%{
  int hasInvalidChars = 0;
  int strlength = 0;
  char* matchedString = NULL;
%}


%%

^(a|b|\[(a|b)*\])* { strlength = yyleng; matchedString = (char*) malloc( (strlength + 1) * sizeof(char)); strcpy(matchedString, yytext);} /* This matches for each line. So if we reject when there is a new line/other un-matched characters, we can guarantee only the input that belongs to the language will be accepted*/
[ \n\t]+           { hasInvalidChars = 1; } 
.+                 { hasInvalidChars = 1; }

%%

int main(int argc, char *argv[]) {
  if ( argc > 1 ) {
    if ( (yyin = fopen(argv[1], "r")) == 0 ) {
      perror(argv[1]);
      exit(1);
    }
  }
  yylex();
  
  if(hasInvalidChars == 0){

    if(strlength == 0){  //Print nothing if it is an empty string
      printf("\n");
    }
    else{ //If the matched string is not empty
      int iterator = 0;

      //Go through the string
      while(iterator != strlength){
        if(matchedString[iterator] == '['){
          //Remember the start position of the bracketed substring
          int startPos = ++iterator;
          
          //Find the ending position of the iterator
          while(matchedString[iterator] != ']'){
            printf("%c", matchedString[iterator]); //Print as usual, if a closing bracket is not matched
            iterator++;
          }

          //After leaving the while-loop above, iterator points to the closing bracket character
          //we now use a for-loop to print the substring again
          for(int i = startPos; i < iterator; i++){
            printf("%c", matchedString[i]);
          }
        }
        else{
          printf("%c", matchedString[iterator]);
        }
        iterator++;
      }
      printf("\n");
    }
  }
  else{
    printf("Boom!\n");
  }

  free(matchedString);
  exit(0);
}