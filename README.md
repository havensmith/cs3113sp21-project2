# Memory Management Simulator in C
Haven Smith<br/>
## Usage
1. C compiler must be installed.
1. run command "make all" (this compiles the code)
2. run command ./manager [algorithim] [memory size] [file name]
3. to test another input, replace the file name with a new file
## How it Works
1. The program first loads instructions into a queue to be run. Commands contain elements such as subcommands, IDs, and size, so I created a structure to hold any possible instruction. When running instructions, the program only looks at the elements in the structure that correlate to the command. For example, the command "REQUEST" will signal the program to look at the instruction's ID, and size, while the command "LIST" will signal the program to look at the instruction's subcommand ("AVAILABLE" or "ASSIGNED").
2. The bestfit and worst fit algorithims use a min/max algorithim to find the smallest/largest possible space for memory to be assigned and the nextfit algorithim uses modulus indexing to loop around the array of memory. The firstfit algorithim looks for the first possible fit.
## Structure
The program uses a structure called "memory" which is comprised of "memory blocks". consecutive empty memory blocks are combined together using an array deletion algorithim, and an array insertion algorithim is used to create new blocks at specific locations.
## File Format
![commands](/images/commands.png)
### Example
![commands](/images/example.png)
![commands](/images/exampleout.png)