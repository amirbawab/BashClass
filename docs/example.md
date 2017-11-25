# Example project

## Demo
<img src="../tutorial.gif" alt="tutorial.gif">

## Project structure
```
example
├── files.list
├── input
│   └── Tutorial.bc
└── lib
    ├── Integer.bc
    ├── List.bc
    ├── Object.bc
    ├── String.bc
    └── Utils.bc
```
* The `main` function is located in `Tutorial.bc` file.
* Files in `libs/` contain data structures and helper functions.

## Prerequisite
* Build BashClass. For more details check the [home page](../).

## Compile files
```bash
cd example
../bin/bashc @files.list -o tutorial.sh
```

## Run generated bash script
```
./tutorial.sh "James"

Hi, James
Let's do some examples ...
Want to do 'List' example? (y/n)
> y
Add items to a list. Type exit to stop.
> Apple
> Orange
> exit
Total number of items added to the list: 2
Want to do 'Casting' example? (y/n)
> y
Adding the following two items to a list: 123, "Orange"
Printing items: 
Item at index 0 is an Integer
123
Item at index 1 is a String
Orange
Congratulations James! You've completed all the examples!
Thank you for trying BashClass :)
```
