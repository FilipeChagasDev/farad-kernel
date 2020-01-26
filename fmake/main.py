# Author: Filipe Chagas Ferraz
# Email: filipe.ferraz0@gmail.com
# Project: FMake
# GitHub repo: https://github.com/FilipeChagasDev/fmake
# Website: https://filipechagasdev.github.io/fmake/

import os
import sys

root_path = ""
fmake_path = ""

targets = {}

# check if filename has extension
def filename_extension(_filename, extension):
    splited = _filename.split(".")
    if len(splited) > 1:
        return _filename.split(".")[-1] == extension
    else:
        return False

# receives a list of filenames and returns a list with only filenames that have the specific extension
def filter_extension(_filename_list, extension):
    return list(filter(lambda fn: filename_extension(fn, extension), _filename_list))

# load all target modules dynamically
def load_targets():
    for root, dir, files in os.walk(globals()["fmake_path"] + "/targets"):
        for cfile in files:
            if filename_extension(cfile, "py"):
                print("target: " + cfile.split(".")[0])
                sys.path.append(root)
                targets[cfile.split(".")[0]] = __import__(cfile.split(".")[0])

# transforms the contents of a fmakelist file into a sources list (without paths)
def fmakelist_parse(_filename):
    file_obj = open(_filename)
    file_txt = file_obj.read()
    lines = file_txt.split("\n")
    lines = map(lambda s: s.strip(), lines) #remove spaces
    lines = filter(lambda s: s != "", lines) #discard blank lines
    return lines

# remove path from filename
def rmpath(fn):
    return fn.split("/")[-1]

# get a list of all sources specified by name.fmakelists in these directory tree 
def fmakelists(name):
    sources = []
    for root, dir, files in os.walk(globals()["root_path"]):
        for cfile in files:
            if cfile == name + ".fmakelist":
                sources += map(lambda s: root + "/" + s, fmakelist_parse(root + "/" + cfile))
    return sources

# returns a list of the filenames of the specified directory
def dir_files(dirpath, walk=False):
    all_files = []
    #fullpath = globals()["root_path"] + "/" + dirpath if dirpath != "" else dirpath
    for root, dir, files in os.walk(dirpath):
        all_files += map(lambda s: root + "/" + s, files)
        if walk == False:
            break
    return all_files

def make(path, _fmake_path, config_filename):
    
    print("FMake, by Filipe Chagas")
    print("-----------------------")

    globals()["root_path"] = path
    globals()["fmake_path"] = _fmake_path
    
    print("loading targets...")
    load_targets()
    print("done..")

    print("loading config...")
    config_txt = open(config_filename).read()

    try:
        config = eval(config_txt)
    except:
        print("---Failed to parse config---")
        sys.exit(1)

    assert(config["magic"] == "fmakeconfig")
    print("done..")


    print("initializing build...")
    steps = config["steps"]
    for step in steps:
        print("step " + str(step))
        step_config = config[step]
        step_target = step_config["target"]
        target_module = targets[step_target]
        target_module.check(step_config)
        target_module.make(step_config)
        print("done")
        try:
            config = eval(config_txt)
        except:
            print("Failed to refresh config")

