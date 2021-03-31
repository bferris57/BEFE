#!/usr/bin/python3
# coding=utf-8
#===============================================================================
#
# File: MinExps - Implementation of a dictionary of MinExps
#
#===============================================================================

from MinExp import MinExp

class MinExps():

    def __init__(self,lst=None):

        self.map = {}
        self.values = {}
        if lst:
            if type(lst) == str:
                keys = lst.split(',')
                for i in range(0,len(keys)): keys[i] = keys[i].strip()
                for entry in keys:
                    self.add(entry)
            elif type(lst) == list:
                for entry in lst:
                    self.add(entry)
            else:
                raise KeyError(str(lst))

    def __len__(self):

        return len(self.map)

    def __iter__(self):

        for key in self.map.keys():
            yield key

    def __contains__(self,key):

        return key in self.map or key in self.values

    def add(self,value):

        minexp = MinExp(value)

        # Make sure no duplicates...
        for poss in minexp:
            if poss in self.map:
                raise KeyError("Key %s already declared for %s"%(repr(poss),repr(self.map[poss])))
            pass

        # Add the keys...
        for poss in minexp:
            self.map[poss] = value
        self.values[value] = None

    def __getitem__(self,index):

        if index in self.values:
            return self.values[index]

        if index not in self.map:
            raise KeyError(str(index))

        return self.values[self.map[index]]

    def __setitem__(self,index,value):

        if type(index) is not str:
            raise KeyError("Expected str index")

        if index not in self.values and index not in self.map:
            self.add(index)

        if index in self.values:
            self.values[index] = value
            return

        realkey = self.map[index]
        self.values[realkey] = value

    def keys(self):

        keys = list(self.map.keys())
        keys.sort()

        return tuple(keys)

    def realkeys(self):

        keys = list(self.values.keys())
        keys.sort()
        return tuple(keys)

    def realkey(self,key):

        if key in self.map:
            return self.map[key]

        if  key in self.values:
            return key

        return None

#===============================================================================
#
# Main
#
#===============================================================================

if __name__ == '__main__':

    exps = ['s[top]','e[xit]|x','h[ello]t[here]']

    me = MinExps()

    idx = -1
    while idx < len(exps)-1:
        idx += 1
        print("me[%s = %s..."%(repr(exps[idx]),repr(idx)))
        me[exps[idx]] = idx

    print("Keys: %s"%repr(me.keys()))

    for key in me.realkeys():
        print("%s = %s"%(repr(key),repr(me[key])))
