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

#INTERFACE: checks if the received configuration dictionary is valid
def check(config):
    assert(config["target"] == "gcc_exec")
    tag_ty = [
        ("flags", str),
        ("objects", list),
        ("out", str)
    ]
    for tag, ty in tag_ty:
        if type(config[tag]) != ty:
            raise Exception("Invalid gcc_object config")

#INTERFACE: build
def make(config_dict):
    object_list = config_dict["objects"]
    object_str = " ".join(object_list)
    outfn = config_dict["out"]
    command = "gcc " + object_str + " -o " + outfn 
    os.system(command)
