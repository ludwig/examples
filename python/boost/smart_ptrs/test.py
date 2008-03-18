import unittest
import custom_sptr

class py_derived_t( custom_sptr.base_i ):
    def __init__( self ):
        custom_sptr.base_i.__init__( self )

    def get_value( self ):
        return 28

class py_double_derived_t( custom_sptr.derived_t ):
    def __init__( self ):
        custom_sptr.derived_t.__init__( self )

    def get_value( self ):
        return 0xDD

class tester_t( unittest.TestCase ):
    def __init__( self, *args ):
        unittest.TestCase.__init__( self, *args )

    def __test_ref( self, inst ):
        try:
            custom_sptr.ref_get_value( inst )
            self.fail( 'ArgumentError was not raised.' )
        except Exception, error:
            self.failUnless( error.__class__.__name__ == 'ArgumentError' )

    def __test_ref_fine( self, inst, val ):
        self.assertEqual( custom_sptr.ref_get_value( inst ), val )

    def __test_val( self, inst, val ):
        self.assertEqual( custom_sptr.val_get_value( inst ), val )

    def __test_const_ref( self, inst, val ):
        self.assertEqual( custom_sptr.const_ref_get_value( inst ), val )

    def __test_impl( self, inst, val ):
        self.__test_ref( inst )
        self.__test_val( inst, val )
        self.__test_const_ref( inst, val )

    def test_derived( self ):
        self.__test_impl( custom_sptr.derived_t(), 0xD )

    def test_py_derived( self ):
        self.__test_impl( py_derived_t(), 28 )

    def test_py_double_derived( self ):
        self.__test_impl( py_double_derived_t(), 0xDD )

    def test_created_derived( self ):
        self.__test_impl( custom_sptr.create_derived(), 0xD )

    def test_created_base( self ):
        inst = custom_sptr.create_base()
        val = 0xD
        self.__test_ref_fine( inst, val )
        self.__test_val( inst, val )
        self.__test_const_ref( inst, val )

    def test_mem_var_access( self ):
        holder = custom_sptr.buffer_holder_t()
        self.failUnless( holder.get_data().size == 0 )
        self.failUnless( holder.data.size == 0 )
        try:
            self.failUnless( holder.data_naive.size == 0 )
            self.fail("TypeError exception was not raised.")
        except TypeError:
            pass
    
    def test_numeric( self ):
        numeric = custom_sptr.create_numeric(21)
        self.failUnless( 21 == numeric.value ) 
        self.failUnless( 21 == custom_sptr.get_numeric_value(numeric) )
        numeric = custom_sptr.numeric_t()
        self.failUnless( 0 == numeric.value ) 
        self.failUnless( 0 == custom_sptr.get_numeric_value(numeric) )
        
def create_suite():
    suite = unittest.TestSuite()
    suite.addTest( unittest.makeSuite(tester_t))
    return suite

def run_suite():
    unittest.TextTestRunner(verbosity=2).run( create_suite() )

if __name__ == "__main__":
    run_suite()
