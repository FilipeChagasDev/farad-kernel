# Author: Filipe Chagas Ferraz
# Email: filipe.ferraz0@gmail.com
# Project: FMake
# GitHub repo: https://github.com/FilipeChagasDev/fmake
# Website: https://filipechagasdev.github.io/fmake/

import os
import sys

#INTERFACE: checks if the received configuration dictionary is valid
def check(config):
    assert(config["target"] == "shell_command")
    assert(type(config["command"]) == str)

    for tag in config:
        if type(config[tag]) != str:
            raise Exception("invalid shell_command config");

#INTERFACE: build
def make(config_dict):
    cmd_pre = config_dict["command"]
    cmd_post = ""

    var_name = ""
    reading_var_name = False
    for c in cmd_pre:
        if c == "%" and reading_var_name == False:
            reading_var_name = True
        elif c == "%" and reading_var_name == True:
            reading_var_name = False
            if var_name == "command":
                raise Exception("shell_command: Cannot use \'command\' tag as a variable")
            try:
                cmd_post += config_dict[var_name]
            except:
                raise Exception("shell_command: Cannot read %" + var_name + "% variable")
            var_name = ""
        elif reading_var_name == True:
            var_name += c
        else:
            cmd_post += c
            
    os.system(cmd_post)
