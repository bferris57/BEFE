#!/usr/bin/python3
#coding=utf-8
#==============================================================================
#
# File: run.py - General purpose run function
#

import subprocess
import shlex

#-------------------------------------------------------------------------------
#
# Function: run - Run a shell command, with optionally redirection and piping
#
# Usage:    result = run(cmd,outprefix=None,errprefix=None)
#
# Where:    outprefix - str: String to proceed displayed stdout lines with
#                              (None = "Don't display")
#           errprefix - str: String to proceed display stderr lines with
#                              (None = "Don't display")
#
#           result    - obj: run.Result object with retcode, stdout, and stderr
#                            attributes containing return code, stdout, stderr output
#                            respectively.
#
# Note:     This function is typically used when you want to use ">", ">&2",
#           "|", "(...)", "&&", "||" etc. in a command.
#

def run(cmd,outprefix=None,errprefix=None):

    class Result(object):
        def __init__(self):
            self.stdout = ''
            self.stderr = ''
            self.retcode = -999999999

    def dumpit(stuff,prefix=None):
        parts = stuff.split('\n')
        for line in parts:
            if prefix:
                print(prefix,end='')
            print(line)

    if 'run' in dir(subprocess):
        p = subprocess.run(shlex.split(cmd),
                           shell=True,
                           universal_newlines=True,
                           stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        
        val = Result()
        val.retcode = p.returncode
        val.stdout  = p.stdout
        val.stderr  = p.stderr
    else:
        retcode,stdout,stderr = run_backport(shlex.split(cmd),shell=True,universal_newlines=True,
                                             stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        val = Result()
        val.retcode = retcode
        val.stdout  = stdout
        val.stderr  = stderr

    if outprefix != None: dumpit(val.stdout,outprefix)
    if errprefix != None: dumpit(val.stderr,errprefix)

    return val

# We had to backport subprocess.run for Python 3.4...
def run_backport(*popenargs, **kwargs):

    #if 'input' not in kwargs:
    #  kwargs['input'] = None
    #if 'check' not in kwargs:
    #  kwargs['check'] = False

    if input is not None:
        if 'stdin' in kwargs:
            raise ValueError('stdin and input arguments may not both be used.')
        kwargs['stdin'] = subprocess.PIPE

    process = subprocess.Popen(*popenargs, **kwargs)
    try:
        stdout, stderr = process.communicate(input)
    except:
        process.kill()
        process.wait()
        raise
    retcode = process.poll()
    if check and retcode:
        raise subprocess.CalledProcessError(
            retcode, process.args, output=stdout, stderr=stderr)
    return retcode, stdout, stderr

#-------------------------------------------------------------------------------
#
#  __main__
#

if __name__ == "__main__":

    cmd = "ls -al"
    ret = run(cmd)
    print("ret.retcode = %s"%ret.retcode)
    if ret.stdout:
        if ret.stdout[-1] == '\n': ret.stdout = ret.stdout[:-1]
        print("***stdout...")
        print(ret.stdout)
        print("...stdout***")
    if ret.stderr:
        if ret.stderr[-1] == '\n': ret.stderr = ret.stderr[:-1]
        print("***stderr...")
        print(ret.stderr)
        print("...stderr***")
