# -*- coding: utf-8 -*-
#
# This script is generated by Algorithm Elf 1.2.1

import sys
import pascal
from aftype import DRIVER, datapool
from aftype import Char, Integer, Boolean, Real
from aftype import String, Array
from aftype import Enum, Set, Record, Pointer
from aftype import Queue, Tree, Indexpointer


class AlgorithmInstance(object):

    def __init__(self):
        stlist.declare()

    def straightsort(self, r):
        DRIVER.simulate_function_call('straightsort', 5, sys._getframe())
        r.frame = sys._getframe()
        result = None

        n = Integer()
        n.declare()
        i = Indexpointer()
        i.declare()
        DRIVER.simulate_statement(9, sys._getframe())
        DRIVER.simulate_statement(10, sys._getframe())
        n.assign(pascal.high(r))
        DRIVER.simulate_statement(11, sys._getframe())
        i.assign(Pointer(r))
        i.assign(Integer(2) - Integer(1))
        _for_end = n
        while True:
            DRIVER.simulate_statement(12, sys._getframe())
            i.assign(i + Integer(1))
            if i > _for_end: break
            DRIVER.simulate_statement(13, sys._getframe())
            _straipass = self.straipass(r, i)
            DRIVER.simulate_function_return()
            DRIVER.simulate_statement(13, sys._getframe())
            _straipass
        DRIVER.simulate_statement(14, sys._getframe())

        r.frame = sys._getframe(1)

        n.destroy()
        i.destroy()

        return result

    def straipass(self, r, i):
        DRIVER.simulate_function_call('straipass', 16, sys._getframe())
        r.frame = sys._getframe()
        i = i.clone()
        i.declare()
        result = None

        x = Integer()
        x.declare()
        j = Indexpointer()
        j.declare()
        DRIVER.simulate_statement(20, sys._getframe())
        DRIVER.simulate_statement(21, sys._getframe())
        j.assign(Pointer(r))
        DRIVER.simulate_statement(22, sys._getframe())
        r[Integer(0)].assign(r[i])
        DRIVER.simulate_statement(23, sys._getframe())
        j.assign(i - Integer(1))
        DRIVER.simulate_statement(24, sys._getframe())
        x.assign(r[i])
        DRIVER.simulate_statement(25, sys._getframe())
        while x < r[j]:
            DRIVER.simulate_statement(27, sys._getframe())
            r[j + Integer(1)].assign(r[j])
            DRIVER.simulate_statement(28, sys._getframe())
            j.assign(j - Integer(1))
        DRIVER.simulate_statement(30, sys._getframe())
        r[j + Integer(1)].assign(r[Integer(0)])
        DRIVER.simulate_statement(31, sys._getframe())

        r.frame = sys._getframe(1)
        i.destroy()

        x.destroy()
        j.destroy()

        return result

    def run(self):
        DRIVER.simulate_program_entry('straitsort', 33)
        DRIVER.simulate_statement(34, sys._getframe())
        _straightsort = self.straightsort(stlist)
        DRIVER.simulate_function_return()
        DRIVER.simulate_statement(34, sys._getframe())
        _straightsort
        DRIVER.simulate_statement(35, sys._getframe())

def init():
    vlist = []
    global stlist
    vlist.append(('stlist', 'Queue'))
    stlist = datapool('stlist', 'Queue')

    return vlist
var_list = init()

# End of Class straitsortAlgorithm
