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

root = Pointer()
root.declare()

class AlgorithmInstance(object):

    def __init__(self):
        bintree.declare()
        root.declare()

    def postorder(self, bt):
        DRIVER.simulate_function_call('postorder', 5, sys._getframe())
        bt = bt.clone()
        bt.declare()
        result = None

        DRIVER.simulate_statement(6, sys._getframe())
        DRIVER.simulate_statement(7, sys._getframe())
        if bt != Pointer():
            DRIVER.simulate_statement(9, sys._getframe())
            _postorder = self.postorder(bt.get()['lchild'])
            DRIVER.simulate_function_return()
            DRIVER.simulate_statement(9, sys._getframe())
            _postorder
            DRIVER.simulate_statement(10, sys._getframe())
            _postorder = self.postorder(bt.get()['rchild'])
            DRIVER.simulate_function_return()
            DRIVER.simulate_statement(10, sys._getframe())
            _postorder
            DRIVER.simulate_statement(11, sys._getframe())
            _visit = self.visit(bt)
            DRIVER.simulate_function_return()
            DRIVER.simulate_statement(11, sys._getframe())
            _visit
        DRIVER.simulate_statement(13, sys._getframe())

        bt.destroy()

        return result

    def visit(self, bt):
        DRIVER.simulate_function_call('visit', 15, sys._getframe())
        bt = bt.clone()
        bt.declare()
        result = None

        DRIVER.simulate_statement(16, sys._getframe())
        DRIVER.simulate_statement(17, sys._getframe())
        bt.get()['value'].active()
        DRIVER.simulate_statement(18, sys._getframe())

        bt.destroy()

        return result

    def run(self):
        DRIVER.simulate_program_entry('postorder', 21)
        DRIVER.simulate_statement(22, sys._getframe())
        root.assign(Pointer(bintree))
        DRIVER.simulate_statement(23, sys._getframe())
        _postorder = self.postorder(root)
        DRIVER.simulate_function_return()
        DRIVER.simulate_statement(23, sys._getframe())
        _postorder
        DRIVER.simulate_statement(24, sys._getframe())

def init():
    vlist = []
    global bintree
    vlist.append(('bintree', 'Tree'))
    bintree = datapool('bintree', 'Tree')

    return vlist
var_list = init()

# End of Class postorderAlgorithm
