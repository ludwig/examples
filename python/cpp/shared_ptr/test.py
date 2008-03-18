import unittest
import shared_ptr


class tester_t( unittest.TestCase ):
    def __init__( self, *args ):
        unittest.TestCase.__init__( self, *args )

    def test( self ):
        ptr = shared_ptr.create_ptr()
        self.failUnless( ptr.text == "ptr" )
        self.failUnless( shared_ptr.read_ptr( ptr ) == "ptr" )
        
        const_ptr = shared_ptr.create_const_ptr()
        self.failUnless( const_ptr.text == "const ptr" )
        self.failUnless( shared_ptr.read_const_ptr( const_ptr ) == "const ptr" )
        
        #testing conversion functionality
        self.failUnless( shared_ptr.read_const_ptr( ptr ) == "ptr" )

def create_suite():
    suite = unittest.TestSuite()
    suite.addTest( unittest.makeSuite(tester_t))
    return suite

def run_suite():
    unittest.TextTestRunner(verbosity=2).run( create_suite() )

if __name__ == "__main__":
    run_suite()
