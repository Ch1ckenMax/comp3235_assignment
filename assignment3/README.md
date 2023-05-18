#Important Notes
- This program has to be run and compiled in Linux.
- I have used some Linux system calls such as ``pipe()``, ``dup2()``... in ``c6c.c``. This program will not work on Windows OS. It probably works on MacOS but I am not sure.
- I have followed the approach in Tutorial 4 and used nas2 as the target machine.
- My Progress on this assignment: I finished feature 1 and feature 2. Did not finish the bonus part.

#How to run
```
make c6c
make nas2
./c6c <program >program.e
./nas2 test.e
```

#Versions of the software used 
- Development Platform: Ubuntu 20.04.1 on Windows WSL
- GCC: gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
- Flex: 2.6.4
- Bison: 3.5.1
- Target Machine: nas2
