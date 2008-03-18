import unittest
import tuples
import custom_rvalue

class tuplesersion_tester_t( unittest.TestCase ):
    def __init__( self, *args ):
        unittest.TestCase.__init__( self, *args )

    def test_tuples( self ):
        self.failUnless( (0,0,0) == tuples.triplet_ret_val_000() )
        self.failUnless( (1,0,1) == tuples.triplet_ret_val_101() )
        self.failUnless( (0,1,0) == tuples.triplet_ret_ref_010() )
        self.failUnlessRaises( TypeError, tuples.triplet_ret_ptr_110 )
        self.failUnless( tuples.test_triplet_val_000( (0,0,0) ) )
        self.failUnless( tuples.test_triplet_cref_010( (0,1,0) ) )
        self.failUnless( tuples.test_triplet_val_000( [0,0,0] ) )
        self.failUnless( tuples.test_triplet_cref_010( [0,1,0] ) )
        self.failUnlessRaises( TypeError, tuples.test_triplet_ref_110, (1,1,0) )
        self.failUnlessRaises( TypeError, tuples.test_triplet_ptr_101, (1,0,1) )

    def test_from_sequence( self ):
        self.failUnless( custom_rvalue.test_val_010( (0,1,0) ) )
        self.failUnless( custom_rvalue.test_cref_000( (0,0,0) ) )
        self.failUnless( custom_rvalue.test_val_010( [0,1,0] ) )
        self.failUnless( custom_rvalue.test_cref_000( [0,0,0] ) )
        self.failUnlessRaises( Exception, custom_rvalue.test_ref_111, (1,1,1) )
        self.failUnlessRaises( Exception, custom_rvalue.test_ptr_101, (1,0,1) )
        self.failUnlessRaises( Exception, custom_rvalue.test_cptr_110, (1,1,0) )

    def test_from_class( self ):
        color = custom_rvalue.colour_t
        self.failUnless( custom_rvalue.test_val_010( color(0,1,0) ) )
        self.failUnless( custom_rvalue.test_cref_000( color(0,0,0) ) )
        self.failUnless( custom_rvalue.test_ref_111( color(1,1,1) ) )
        self.failUnless( custom_rvalue.test_ptr_101( color(1,0,1) ) )
        self.failUnless( custom_rvalue.test_cptr_110( color(1,1,0) ) )

    def cmp_colours( self, c1, c2 ):
        return c1.red == c2.red and c1.green == c2.green and c1.blue == c2.blue

    def test_from_class_property( self ):
        colour = custom_rvalue.colour_t
        desktop = custom_rvalue.desktop_t()
        self.failUnless( self.cmp_colours( desktop.background, colour() ) )
        desktop.background = (1,0,1)
        self.failUnless( self.cmp_colours( desktop.background, colour(1,0,1) ) )
        self.failUnless( desktop.is_same_colour( (1,0,1) ) )
        self.failUnless( desktop.is_same_colour( colour(1,0,1 ) ) )

def create_suite():
    suite = unittest.TestSuite()
    suite.addTest( unittest.makeSuite(tuplesersion_tester_t))
    return suite

def run_suite():
    unittest.TextTestRunner(verbosity=2).run( create_suite() )

if __name__ == "__main__":
    run_suite()
