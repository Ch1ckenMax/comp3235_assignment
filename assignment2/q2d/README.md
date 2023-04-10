# Info
- Development Platform: Windows 10 WSL => Ubuntu 20.04 LTS

# How to use
- The program accepts exactly one input from a file
- To compile the parser, write on the terminal: `make q2`
- To run the parser, write on the terminal: `./q2 <filename>`
- You can clean the intermediate files in the compilation process by `make clean`

# CAUTION
- Note: If the length of the input exceeds 2048, you may need to change the constant variable `MAX_STR_SIZE` in `q2.y` to 2 times the length of the input.
- This is due to the fact that enough space has to be pre-allocated in the heap for the strcat() function to work in the actions of each production.
