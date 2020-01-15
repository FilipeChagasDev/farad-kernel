# Author: Filipe Chagas Ferraz
# Email: filipe.ferraz0@gmail.com
# Project: FMake
# GitHub repo: https://github.com/FilipeChagasDev/fmake
# Website: https://filipechagasdev.github.io/fmake/

import os
import sys

# "root/dirname/filename" -> "filename"
def rmpath(name):
    return name.split("/")[-1]

# "filename.c" -> "filename.o"
def outname(inname):
    return inname.split(".")[0] + ".o"

#INTERFACE: checks if the received configuration dictionary is valid
def check(config):
    assert(config["target"] == "gcc_object")
    tag_ty = [
        ("flags", str),
        ("sources", list),
        ("outdir", str)
    ]
    for tag, ty in tag_ty:
        if type(config[tag]) != ty:
            raise Exception("Invalid gcc_object config")

#INTERFACE: build
def make(config_dict):
    sources_list = config_dict["sources"]
    for source in sources_list:
        in_filename = rmpath(source)
        out_filename = outname(in_filename)
        outpath = config_dict["outdir"] + "/" + out_filename
        command = "gcc -c " + source + " -o "  + outpath
        os.system(command)
