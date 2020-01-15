# Author: Filipe Chagas Ferraz
# Email: filipe.ferraz0@gmail.com
# Project: FMake
# GitHub repo: https://github.com/FilipeChagasDev/fmake
# Website: https://filipechagasdev.github.io/fmake/

import os
import sys

#INTERFACE: checks if the received configuration dictionary is valid
def check(config):
    assert(config["target"] == "command_foreach")
    assert(type(config["command"]) == str)
    assert(type(config["list"]) == list)
    
    for tag in config:
        if tag == "list":
            continue
        elif tag == "each":
            raise Exception("invalid command_foreach config: \'each\' is an internal variable and cannot be used as a tag. ")
        elif type(config[tag]) != str:
            raise Exception("invalid command_foreach config");

#INTERFACE: build
def make(config_dict):
    foreach_list = config_dict["list"]

    for each in foreach_list:
        config_dict["each"] = str(each)
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
                    raise Exception("command_foreach: Cannot use \'command\' tag as a variable")
                elif var_name == "list":
                    raise Exception("command_foreach: Cannot use \'list\' tag as a variable. The \'each\' variable should be used to get list elements.")

                try:
                    cmd_post += config_dict[var_name]
                except:
                    raise Exception("command_foreach: Cannot read %" + var_name + "% variable")
                var_name = ""
            elif reading_var_name == True:
                var_name += c
            else:
                cmd_post += c
                
        os.system(cmd_post)
