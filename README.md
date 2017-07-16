# BashClass [![Build Status](https://travis-ci.org/amirbawab/BashClass.svg?branch=master)](https://travis-ci.org/amirbawab/BashClass)

### About
BashClass is programming language developed to add the power of classes to BASH and to make BASH scripting closer to other 
programming languages like Java, C++ and others... . 
BashClass uses EasyCC-C++ library which provides the lexical and syntax analysis. 
The lexical and syntax analysis configuration files, located under the resources directory, 
serve as an input to EasyCC-C++ to parse the user files and validate it against a grammar determining the language syntax.
Check below for more details about the language features.

### Build
To build the project:
* Clone project
* Clone submodules: `git submodule update --init --recursive`
* Run `run_cmake.sh`
* Build dev mode: `run_make_dev.sh`
* Build pro mode: `run_make_pro.sh`

### Run
#### Developer mode
```
./bin/bashcdev -s resources/src/lexical_graph.json -c resources/src/lexical_config.json -e resources/src/lexical_errors.json -g resources/src/grammar.json -E resources/src/syntax_errors.json -o /tmp/test.sh -v path/to/file1 path/to/file2 ...
```
#### Production mode
```
./bin/bashc path/to/file1 path/to/file2 ... -o /tmp/test.sh
```
In production mode the list of files to compile can be stored in a file (e.g. `files.list`) and then passed to the application prefixed by an `@` symbol. (e.g. `./bin/bashc @files.list -o /tmp/test.sh`)

### Syntax
#### Examples
* Main example: <a href="resources/src/input/">Directory</a>  
* Library example: <a href="resources/src/lib/">Directory</a> 

#### Misc
* Function: `function void hello() {}`
* Variable: `var int i = 0`
* Class: `class MyClass{}`
* For loop: `var int i; for(i=0; i < 10; i=i+1){}`
* While loop: `var int i=0; while(i < 10) { i=i+1}`
* If-Elif-Else: `if(i == 1) {} elif (i == 2){} else {}`

### Some features
* Classes
* Global functions
* Class functions
* Global variables
* Class variables
* Type checking
* Comments (inline and block)
* Recursion
* Return primitive types, strings and objects
* While, For and If statements
* Create new instance of objects (e.g. myObj = new Object())
* Constructor is required to create instances
* Run bash code (inline and block)
* Run subshell (e.g. echo $(echo 1) )
* Arrays (1D, 2D, ... ND)
* Casting
* Inheritance
* No function overloading but can define a default value for params
* No polymorphism but can check type at runtime

### Limitations
* The application was tested only on Arch and Ubuntu
* Not much input files has been written to test the application
* Project was created for fun and is not guaranteed to work everywhere, unless users prove the opposite

### Contribution
* Feel free to <a href="CONTRIBUTING.md">contribute</a>

### Additional links
* UML for some classes (check tabs): https://drive.google.com/file/d/0B8fWEFscW3Z4c2pvSHNaSmRiVEE/view?usp=sharing
* Examples of BashClass language syntax: https://github.com/amirbawab/BashClass/tree/master/resources/src/input
* Contributing: https://github.com/amirbawab/BashClass/blob/master/CONTRIBUTING.md
* Code of conduct: https://github.com/amirbawab/BashClass/blob/master/CODE_OF_CONDUCT.md
