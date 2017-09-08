# <img src="logo.png" width="200"> [![Build Status](https://travis-ci.org/amirbawab/BashClass.svg?branch=master)](https://travis-ci.org/amirbawab/BashClass)

## Github
Link: [https://github.com/amirbawab/BashClass](https://github.com/amirbawab/BashClass)

## About
BashClass is a programming language supporting Object Oriented Programming, and compiles to BASH 4.4. Class members written in BashClass are converted to an associative array with unique indices in the generated BASH script. 

BashClass uses [EasyCC-C++](http://amirbawab.github.io/EasyCC-CPP) library which provides the lexical and syntax analysis. The lexical and syntax analysis configuration files, located under the resources directory, serve as input to [EasyCC-C++](http://amirbawab.github.io/EasyCC-CPP) to parse the user files and validate it against a grammar determining the language syntax. 


## Build project
### Docker
* Clone repository
* Build image: `sudo docker build -t bashclass:latest .`
* Run container: `docker run --name bashclass-tutorial -it bashclass:latest`

### Host
* Clone repository
* Clone submodules: `git submodule update --init --recursive`
* Run cmake:
```bash
cmake . \
    -DSYNTAX_ERRORS="resources/src/syntax_errors.json"\
    -DSYNTAX_GRAMMAR="resources/src/grammar.json"\
    -DLEXICAL_ERRORS="resources/src/lexical_errors.json"\
    -DLEXICAL_CONFIG="resources/src/lexical_config.json"\
    -DLEXICAL_STATE_MACHINE="resources/src/lexical_graph.json"
```
* Run make (developer mode):
```bash
make bashcdev
```
* Run make (production mode):
```bash
make generate_files # Embed JSON configurations into C++ files
# Run the `cmake` command again to include the generated files
# cmake . ...
make bashc
```

## Usage
### Developer mode
```bash
./bin/bashcdev \
    -s resources/src/lexical_graph.json \
    -c resources/src/lexical_config.json \
    -e resources/src/lexical_errors.json \
    -g resources/src/grammar.json \
    -E resources/src/syntax_errors.json \
    -o /tmp/test.sh \
    -v \
    path/to/file1 path/to/file2 ...
```
### Production mode
```bash
./bin/bashc -o gen_bash.sh \
    path/to/file1 path/to/file2 ... 
```
In production mode the list of files to compile can be stored in a file (e.g. `files.list`) and then passed to the application prefixed by an `@` symbol. (e.g. `./bin/bashc @files.list -o /tmp/test.sh`)

## Limitations
* The application was tested only on Arch and Ubuntu
* Not much input files has been written to test the application
* Project was created for fun and is not guaranteed to work everywhere, unless users prove the opposite ;)

## Contribution
* Feel free to contribute
