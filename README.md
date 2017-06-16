# BashClass

### TLDR; 
BashClass is a programming language with classes and functions, and it compiles into BASH 4.3+.
The project is still not released but can be used, check below for more details about the features.

### About
BashClass is programming language developed to add the power of classes to BASH and to make BASH scripting closer to other 
programming languages like Java, C++ and others... . 
The project is still not released but can be used, check below for more details about the features.
BashClass uses EasyCC-C++ library which provides the lexical and syntax analysis. 
The lexical and syntax analysis configuration files, located under the resources directory, 
serve as an input to EasyCC-C++ to parse the user files and validate it against a grammar determining the language syntax.

### Features
* Classes
* Global functions
* Class functions
* Global variables
* Class variables
* Type checking
* Comments (inline and block)
* Recursion
* Return primitive types, strings and objects
* Easy while and if statements
* Create new instance of objects (e.g. myObj = new Object())
* Constructor is required to create instances
* Variable of type `any` can hold a value of any type, but can't access its members (if any)
* Run bash code (inline and block)
* Run subshell (e.g. echo $(echo 1) ). They can be assigned to variables of type `string`.
* No arrays (In process)
* No casting
* No function overloading
* No inheritance
* Can't declare and initialize on the same line

### Additional links
* UML for some classes (check tabs): https://drive.google.com/file/d/0B8fWEFscW3Z4c2pvSHNaSmRiVEE/view?usp=sharing
* Examples of BashClass language syntax: https://github.com/amirbawab/BashClass/tree/master/resources/src/input
