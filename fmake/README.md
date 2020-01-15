# About

FMake v1.2.0 (November/2019)

Author: Filipe Chagas Ferraz

GitHub repo: https://github.com/FilipeChagasDev/fmake

FMake website: https://filipechagasdev.github.io/fmake/

# Operation

The fmake/main.py script contains the *make* function. This function must be called by a build.py script to start the build process.

```python
def make(path, _fmake_path, config_filename):
```

One of the first things that happens when make is invoked is the loading of target-modules into the target directory. This operation is performed by the *load_targets* function.

Immediately afterwards, the configurations file is loaded and parsed by the *eval* function.

If there are no syntax errors in the configurations file, the step-by-step compilation will start.

# Configurations file

The configurations file is a text file that contains a python dictionary with the compilation process information.

The configuration file must contain:
* A "magic" tag with the value "fmakeconfig".
* A "steps" tag with a list containing the tags of the compilation steps in the order in which they should be performed.
* Arbitrary tags (can be strings or numbers) containing the information of the compilation steps. Each must contain a sub-dictionary with the build target settings.

View https://github.com/FilipeChagasDev/fmake/blob/master/src/fmake/targets/README.md for more informations.

Example:
```python
{
    "magic": "fmakeconfig",

    "steps": [0, 1],

    0: {
        "target": "gcc_object",
        "flags": "-O2 -lm",
        "sources": fmakelists("csources"),
        "outdir": "obj"
    },

    1: {
        "target": "gcc_exec",
        "flags": "",
        "objects": filter_extension(dir_files("./obj", False), "o"),
        "out": "prog.elf"
    }
}
```
