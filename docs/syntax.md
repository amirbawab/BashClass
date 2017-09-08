## Variable
```
var int id;
```
* Variable must have a type.
* Variable can be initialized when declared.

## Function
```
function void myFunction(var int arg1=0) {
    // body
}
```
* Function must have a type.
* Parameters can have default values.
* Each program must have a `main` function of type `int` with:
    * No parameters, or
    * `var int argc, var char[][] argv`

## For loop
```
var int i;
for(i=0; i < 10; i=i+1) {
    // body
}
```
* The counter must be declared before the for loop statement.

## While loop
```
while(true) {
    // body
}
```

## If, Elif and Else
```
if(condition1) {
    // body1
} elif( condition2) {
    // body2
} else {
    // body3
}
```

## Class
```
class MyClass {
    constructor MyClass() {
        // body
    }
    // functions and variables
}
```
* One constructor function is required for each class and must share the class name.
* A class can have [functions](#funtion) and [variables](#variable).
* All class members for instances are accessible.

## Inheritance
```
class Child extends Parent {
    constructor Child() {
        super_constructor();
        // body
    }
}
```
* Parent constructor must be called from child constructor

## Arrays
```
var MyClass[] data = new MyClass[];
```
* Multi-dimensional arrays are created similarly

## Casting
```
var int fakeInt = ('a'):int;
```
* Variable `fakeInt` will not convert `a` to its corresponding ascii number, instead it will just trick the compiler to think it holds an `int`. 
* Casting to wrong types might lead to an undefined behavior (example above).

## Bash code
### Inline bash script
```
># echo Hello world
```

### Block of bash script
```
>@
    echo Hello
    echo World
@<
```

### Subshell char[]
```
var char[] helloWorld = >$echo hello world$<;
```
* Equivalent to: `helloWorld="$(echo hello world)"`

### Subshell int
```
var int one = >%echo 1%<;
```
* Equivalent to: `one="$(echo 1)"`
