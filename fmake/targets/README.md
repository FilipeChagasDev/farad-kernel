# Target-modules

Target-modules are small plug-ins that perform the most important part of the compilation process. For each build step described in the fmake configuration file, a target is specified and a target-module is used.

# Configurations

Each build step has a configuration sub-dictionary in the fmake configurations file dictionary. These sub-dictionary contain information that is passed to the required target module.

# Module structure

every target-modulo must have the following functions:

```python
#INTERFACE: checks if the received configuration dictionary is valid
def check(config):
```

As previously described, the target module receives a dictionary with the build step settings. What the check function should do is basically check for invalid information in this dictionary. If the dictionary has invalid information, an exception should be thrown with the raise directive.

```python
#INTERFACE: build
def make(config_dict):
```

The make function must perform the compilation step. Within it, one or more command lines can be constructed and passed to the shell via the os.system function.

# Default target-modules

## shell_command

submit arbitrary command to shell

The body of the command must be in the "command" tag.

Variables can be identified between two '%', as follows: "%variable%".

With the exception of "command", all tags can be treated as variables.

```python
# example of configurations sub-dictionary for shell_command target
{
        "target": "shell_command",
        "name": "Filipe",
        "surname": "Chagas Ferraz",
	"alpha": ",".join(["a", "b", "c", "d", "..."]),
        "command": "echo %name% %surname% %apha%"
}
```

The only obrigatory tags are "command" and "target". All others are optional.

## gcc_object

compiles an object file for each C code file specified in the sources list

```python
# example of configurations sub-dictionary for gcc_object target
{
	"target": "gcc_object", #required
	"flags": "-O2 -lm", #gcc flags
	"sources": ["main.c", "module0.c"], #code files to compiler
	"outdir": "obj" #directory where the .o files will be placed
}
```

## gpp_object

compiles an object file for each C/C++ code file specified in the sources list

```python
# example of configurations sub-dictionary for gcc_object target
{
	"target": "gpp_object", #required
	"flags": "-O2 -lm", #gcc flags
	"sources": ["main.cpp", "module0.cpp"], #code files to compiler
	"outdir": "obj" #directory where the .o files will be placed
}
```

## gcc_exec

merge a set of .o files into a single executable

```python
# example of configurations sub-dictionary for gcc_exec target
{
	"target": "gcc_exec", #required
	"flags": "", #gcc flags
	"objects": ["obj/main.o", "obj/module0.o"], #object files to compiler
	"out": "program.elf" #output filename
}
```

