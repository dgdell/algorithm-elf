# -*- coding: utf-8 -*-
#This file was originally generated by PyScripter's unitest wizard

import sys
import gettext

sys.path.append("")
sys.path.append("../src")
sys.path.append("../src/algorithm")

import unittest
from dsException import dsError
import baseAlgorithm

class TestbaseAlgorithm(unittest.TestCase):

    def setUp(self): 
        _ = str
        gettext.NullTranslations().install(unicode=True)
        self.testedObject = baseAlgorithm.baseAlgorithm()


    def tearDown(self): 
        pass

    def testbuildTree(self):
        
        _strTree0 = ""
        _tree = self.testedObject.buildTreeList(_strTree0)
        self.assertEqual(
            _tree.__str__(),
            ""
            )

        _strTree1 = "A:[B, C]; B:[D]"
        _tree = self.testedObject.buildTreeList(_strTree1)
        self.assertEqual(
            _tree.__str__(),
            u'A : [ B, C ]; B : [ D ]; D; C'
            )
            
        _strTree2 = "A:[B, C]; B:[D]; C:[E, F]; E:[G, H]; F:[I]"
        _tree = self.testedObject.buildTreeList(_strTree2)
        self.assertEqual(
            _tree.__str__(),
            u'A : [ B, C ]; B : [ D ]; D; C : [ E, F ]; E : [ G, H ]; G; H; F : [ I ]; I'
            )
        self.assertEqual(
            _tree.getChildByName('C').getString(),
            'C'
            )

        _strTree = 'A(value=x):[B,C]; B(value=y, title=me); C(value=z) : [D]'
        _tree = self.testedObject.buildTreeList(_strTree)
        self.assertEqual(
            _tree.getString(),
            'x'
            )
        self.assertEqual(
            _tree.getChildByName('B').getProperty('title'),
            'me'
            )
        self.assertEqual(
            _tree.getChildByName('C').getString(),
            'z'
            )
        
        _strTreeE1 = "1A:[D]"
        self.assertRaises(
            dsError,
            self.testedObject.buildTreeList,
            _strTreeE1
            )

    def testBuildGraph(self):
        strgraph = ''
        _expected = []
        _graph = self.testedObject.buildGraphList(strgraph)
        self.assertEqual(
            _graph.getNodes(),
            _expected
            )

        strgraph = ("A;B;C;D;E;F : <A, B>(weight=6); <A, C>(weight=1); <A, D>(weight=5);"        
                   "<B, C> (weight=5); <B, E>(weight=3); <C, D>(weight=6); "
                   "<C, E>(weight=6); <C, F>(weight=4); <D, F>(weight=2); "
                   "<E, F>(weight=2);")

        _expected = [u'A', u'B', u'C', u'D', u'E', u'F']
        _graph = self.testedObject.buildGraphList(strgraph)
        self.assertEqual(
            _graph.getNodes(),
            _expected
            )

        _expected = [(u'A', u'B'), (u'A', u'C'), (u'A', u'D'), (u'B', u'C'), (u'B', u'E'), (u'C', u'D'), (u'C', u'E'), (u'C', u'F'), (u'D', u'F'), (u'E', u'F')]
        self.assertEqual(
            _graph.getEdges(),
            _expected
            )

        self.assertEqual(
            int(_graph.getEdgeProperty('C', 'F', 'weight')),
            4
            )

        
if __name__ == '__main__':
    unittest.main()
    
