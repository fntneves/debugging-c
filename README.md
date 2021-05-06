## Introduction
A debugger is a program that allows you to see what is going on another program while it executes by:
- Controlling its execution (pause and resume)
- Applying break points or conditions to stop the execution and inspect the current state
- Viewing variable values and watching changes
- Executing the program line by line or instruction by instruction
- Examine what was happened, when the program stopped or crashed

### GNU Debugger
GDB is the GNU Project debugger we are going to use in this demo.
It supports a wide set of languages, including C, C++ and  Rust.
In this demo we will use C. (Hey Operating Systems guys!)

### How to start?
Well, debugging compiled programs is a mess...
Let me show how it looks like.

Let's compile the _factorial_ program using the typical command:

`gcc factorial.c -o factorial`

Now, let's debug it!

```
$ gdb factorial 5
...
(gdb) start
```

What a nightmare! I hope you love assembly. That's all, folks. 👋

---

No. Wait. I am just kidding.

In fact, you can see the source code. However, we first need to prepare our program for debugging. To do so, we need to use the `-g` flag to keep the [symbols](https://www.tutorialspoint.com/gnu_debugger/gdb_debugging_symbols.htm):

`gcc -g factorial.c -o factorial`

## Features Overview
- Execution Control
    - `start` to enter the `main` function
    - `run` to execute the entire program
    - `show args` to print the arguments of the program
    - `attach <pid/name>` to attach to a curring process
    - `step` to perform a single step _in_ (enter a function, for instance)
    - `next` to perform a single step _over_
    - `finish` to complete the current frame
    - `return <value>` to return immediately with a given value
- Breakpoints
    - `break <function>` to set a breakpoint in the given function
    - `break <file>:<line>` to set a breakpoint in the given line of the given file
    - `break <name> if strcmp(y,"hello") == 0` to set a conditional breakpoint
    - `info break` to list all current breakpoints and watchpoints
- Watchpoints
    - `watch <var>` to stop at every variable's value change
    - `watch -location <pointer> -x <byte_size>` to watch memory writes in the given `<pointer>` with `<byte_size>` bytes
    - `info break` to list all current breakpoints and watchpoints
- Variables
    - `info args` to see the arguments of the current frame
    - `info locals` to see all variables of the current frame
    - `p <var>` prints the variable's value
    - `p *ptr@10` print an array of 10 integers in memory, assuming a pointer `int * ptr` exists
- Examining Thread State
    - `up` to go up in the frame
    - `down` to go down in the frame
    - `info registers` to access the registers
    - `x <pointer>` read memory starting at the given pointer
- TUI (Text User Interface)
    - `tui enable` activates the Text User Interface
    - `tui disable` deactivates the Text User Interface
    - See all action keys [here](https://sourceware.org/gdb/onlinedocs/gdb/TUI-Overview.html#TUI-Overview)

## Debugging in Practice

### Case 1: Factorial
The `factorial.c` program calculates the factorial of a number given by argument:

`factorial <number>`

Currently, there is a bug that hangs the program and we want to know what is happening.
Let's analyse it using `gdb`.

```
$ gdb factorial
...
(gdb) run 5
```

The program does not terminate. Let's interrupt the execution by hitting CTRL+C and see what's happening.
When we hit CTRL+C, the debugger interrupts the program execution and displays the current line being executed.

Let's inspect the state of the program:
- `info args` to show factorial's function arguments
- `info locals` to show local variables

Why is the `total` variable always set to zero?
Damn, that initial value is zero! Anything multiplied by 0 is 0...
Let's change it to 1 in place, to see if it fixes our problem.
- `set total = 1`

The `total` variable is now updating, but there is no way to stop the loop.
5! is 120, and we are getting far away from that.

Aha! The `n` is not being updated. And that's why our program do not stop, we are not updating the `n` value in a favorable way to stop the loop!

Good catch! We're done with this!

### Case 2: Segmentation Fault
Another buggy program we have results in a _Segmentation Fault_! Oh, that famous error!

We have a struct `Person` defined in the `person.h` file.
The methods that manipulate this struct are in the `person.c` file.

The program `print_person` creates a Person and prints their details.
However, it exits with a _Segmentation Fault_ error before printing the details.

Let's inspect it:
```
$ gdb print_person
...
(gdb) run
```

Here it is.

Note that when a _Segmentation Fault_ occurs, the debugger not only displays the error but also permits to inspect the state of the program recorded before the process being killed by the operating system.

Looking that the function we are, we may be deep in the stack trace, maybe in a frame that is out of our control. We should enter the command `up` until we recognize the statement.

Well, apparently, the _Segmentation Fault_ came from the `strncpy` function call in `person.c:6`.

Inspecting the arguments of the `fill_person_data` using `info args`, we have:
```
person = 0x0
name = 0x100000f80 "Gilberto"
address = 0x100000f89 "Portugal"
```
> Note that addresses may vary on each execution.

Look, the `person` we received is a `NULL` address (i.e., `0x0`). And we are trying to operate over a memory address that does not exist. How dare we?

All we need is to understand how we got the `person` address as `0x0`?

Let's see the caller's frame by entering `up`.
We are now at the `main` function. This is the point where the `Person * person` is declared.

Aha! The `person` is not being initialized. That's why it the _Segmentation Fault_ error happens!

Let's change this by either:

1. Using `malloc`.

`Person * person = (Person *) malloc(sizeof(Person));`

> We can change it in-place by setting a breakpoint in `print_person.c:6` and entering `set person = malloc(sizeof(Person))`.

2. Declaring `person` as a `Person` instead of a `Person *` and passing its pointer to the `fill_person_data`.

```c
Person person;

fill_person_data(&person, ...);
```

Yay, no more _Segmentation Fault_!

### Case 3: Debugging Unexpected Behavior by Inspecting Memory

We resolved the _Segmentation Fault_. Yay!
However, when we execute the same program, we notice an unexpected output.

```
person name: GilbertoPortugal
person address: Portugal
```

The name is being printed along with the address, which is not expected.

In order to investigate the root cause of it, let's put a breakpoint right before calling the `print_person` function and see what is the state of the `person` struct.

`print person` prints a memory address. Let's visit the pointer's value.

`print *person` shows the current values of the `Person` struct.

```
(gdb) print *person
{name = "Gilberto", address = "Portugal"}
(gdb) print person->name
"Gilberto"
(gdb) print person->address
"Portugal"
```

The values are the expected ones.

Let's confirm what is the output produced by the `printf` function when it prints the name:
```
(gdb) call (void) printf("%s\n", person->name)
"GilbertoPortugal"
```

Well, it does not match what we have seen previously. That's a tough one...

We know that strings in C are delimited by the ``'\0'`` character. That's is the mark that tells when a string ends, which is mandatory for `printf` and string-related functions.

We should confirm the string ends with the `'\0'` char.
How can we do that?

Inspecting the memory of the Dump the allocated person struct should gives us any hint.

`x/16bc person` prints 16 bytes starting from the `person` address and represents them as characters.

```
0x100200000:	71 'G'	105 'i'	108 'l'	98 'b'	101 'e'	114 'r'	116 't'	111 'o'
0x100200008:	80 'P'	111 'o'	114 'r'	116 't'	117 'u'	103 'g'	97 'a'	108 'l'
```

We do not see any `'\0'` char (ASCII code is 0) for the name neither for the address. Wouldn't the address be printed with something concatenated also? Let's see what comes after the address space...

`x/17bc person` will now print 17 bytes..

```
0x100200000:	71 'G'	105 'i'	108 'l'	98 'b'	101 'e'	114 'r'	116 't'	111 'o'
0x100200008:	80 'P'	111 'o'	114 'r'	116 't'	117 'u'	103 'g'	97 'a'	108 'l'
0x100200010:	0 '\000'
```

Oh, we see. We are so lucky. Can we imagine what could happen without that `'\0'` after the address? We could get unpredictable output...

Then, we can actually replace the last name's character with `'\0'` byte to see what happens.

```
(gdb) set {char}0x100200007 = `'\0'` # Set the last char of the name.
(gdb) set {char}0x10020000f = `'\0'` # Set the last char of the address.
```

Stepping over the next statements (i.e., `next`), we get:
```
person name: Gilbert
person address: Portuga
```

OK, we need to fix this. Let's examine the code.
We allocate 8 bytes for each struct `Person`'s field: `name` and `address`.
When we call the `strncpy` in the `fill_person_data`, it is not guaranteed that it ends with the `'\0'` character, specially if the length of the string is equals to the allocated space. As we are filling the whole space (8 bytes) with the name `Gilbero`, there is no space for `'\0'` as the final character.

But we need to consider this character to have a delimited string, otherwise `printf` will never know when to the string ends and will try to print everything that follows until a `'\0'` is found.

Thus, we need to increase the amount allocated for name and address fields by one byte (in `person.h`), in order to include the `'\0'` resulting from the `strncpy` in the `fill_person_data`, and ensuring the last char of the string is `'\0'` after copying it.

```c
// person.h
typedef struct person {
    char name[MAX_NAME + 1];
    char address[MAX_ADDRESS + 1];
} Person;

// person.c
void fill_person_data(Person * person, char * name, char * address) {
    strncpy(person->name, name, MAX_NAME);
    strncpy(person->address, address, MAX_ADDRESS);

    person->name[MAX_NAME] = '\0';
    person->address[MAX_ADDRESS] = '\0';
}
```

Inspecting again the memory right before the `print_person` function call, we see there is a `'\0'` char that is now part of the name and the address.

```
(gdb) p/c *person
{name = {71 'G', 105 'i', 108 'l', 98 'b', 101 'e', 114 'r', 116 't', 111 'o', 0 '\000'}, address = {
    80 'P', 111 'o', 114 'r', 116 't', 117 'u', 103 'g', 97 'a', 108 'l', 0 '\000'}}
```

Note now that the `sizeof(Person)` is 18 bytes. Dumping the memory of the `person` address, we see that there is a `'\0'` char right at the end of each field.

```
(gdb) x/18bc person
0x100100690:	71 'G'	105 'i'	108 'l'	98 'b'	101 'e'	114 'r'	116 't'	111 'o'
0x100100698:	0 '\000'	80 'P'	111 'o'	114 'r'	116 't'	117 'u'	103 'g'	97 'a'
0x1001006a0:	108 'l'	0 '\000'
```

And letting the program run, we see the expected output!

Yay, we got rid of weird bugs!

## Final Notes

This is just an overview of the power of using GDB to debug C programs.

You are free to explore everything you want. Do not be afraid to experiment, explore and ruin your programs.

Let curiosity and adventure drive you in this journey.

See you there.

## References

[GDB - Debugging Symbols](https://www.tutorialspoint.com/gnu_debugger/gdb_debugging_symbols.htm)

[GDB Documentation](https://sourceware.org/gdb/onlinedocs/gdb/)
- [Examining Memory](https://sourceware.org/gdb/current/onlinedocs/gdb/Memory.html)

[GDB to LLDB command map](https://lldb.llvm.org/use/map.html#evaluating-expressions)

[Debugging programs with multiple processes](https://docs.huihoo.com/redhat/rhel-4-docs/rhel-gdb-en-4/processes.html)
