import unittest
import my_exceptions

class application_error(Exception):
    def __init__( self, app_error ):
        Exception.__init__( self )
        self._pimpl = app_error
    
    def __str__( self ):
        return self._pimpl.message()

    def __getattribute__(self, attr):
        my_pimpl = super(application_error, self).__getattribute__("_pimpl")
        try:
            return getattr(my_pimpl, attr)
        except AttributeError:
            return super(application_error,self).__getattribute__(attr)

my_exceptions.application_error = application_error
my_exceptions._application_error_.py_err_class = application_error

class tester_t( unittest.TestCase ):
    def __init__( self, *args ):
        unittest.TestCase.__init__( self, *args )
    
    def test_function_call( self ):
        my_exceptions.check_preconditions( False )
    
    def test_concrete_error( self ):
        try:
            my_exceptions.check_preconditions( True )
        except my_exceptions.application_error, err:
            self.failUnless( str( err ) == "xyz" )

    def test_base_error( self ):
        try:
            my_exceptions.check_preconditions( True )
        except Exception, err:
            self.failUnless( str( err ) == "xyz" )

    def test_redirection( self ):
        try:
            my_exceptions.check_preconditions( True )
        except Exception, err:
            self.failUnless( err.application_name() == "my_exceptions module" )

    def test_converter( self ):
        try:
            my_exceptions.check_preconditions( True )
        except my_exceptions.application_error, err:
            app_name = my_exceptions.get_application_name( err )
            self.failUnless( "my_exceptions module" == app_name )
            

def create_suite():
    suite = unittest.TestSuite()
    suite.addTest( unittest.makeSuite(tester_t))
    return suite

def run_suite():
    unittest.TextTestRunner(verbosity=2).run( create_suite() )

if __name__ == "__main__":
    run_suite()
