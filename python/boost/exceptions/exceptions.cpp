#include "boost/python.hpp"
#include <stdexcept>
#include <iostream>

/**
 * Content:
 *  * example, which explain how to create custom exception class, which gives
 *    expected behaviour to exceptions exposed using Boost.Python library.
 * 
 *  The example also allows you to map your exception classes to the Python 
 *  built-in ones.
 *
 **/

class application_error : public std::exception{
public:

    application_error()
    : std::exception(), m_msg()
    {}

    application_error( const std::string& msg )
    : std::exception(), m_msg( msg )
    {}

    application_error( const application_error& other )
    : std::exception(other), m_msg( other.m_msg )
    {}
        
    const std::string& message() const
    { return m_msg; }
    
    virtual ~application_error() throw(){}

    std::string application_name() const
    { return "my_exceptions module"; }
        
private:
    const std::string m_msg;
};

//small dummy function that will conditionaly throws exception
void check_preconditions( bool raise_error ){
    if( raise_error ){
        throw application_error( "xyz" );
    }
}

//test function for custom r-value converter
std::string get_application_name( const application_error& err ){
    return err.application_name();
}

namespace bpl = boost::python;

struct exception_translator{    
    
    exception_translator(){

        bpl::register_exception_translator<application_error>(&exception_translator::translate);

        //Register custom r-value converter
        //There are situations, where we have to pass the exception back to 
        //C++ library. This will do the trick
        bpl::converter::registry::push_back( &exception_translator::convertible
                                            , &exception_translator::construct
                                            , bpl::type_id<application_error>() );
    }
    
    static void 
    translate( const application_error& err ){
        bpl::object pimpl_err( err );
        bpl::object pyerr_class = pimpl_err.attr( "py_err_class" );
        bpl::object pyerr = pyerr_class( pimpl_err );
        PyErr_SetObject( pyerr_class.ptr(), bpl::incref( pyerr.ptr() ) );
    }

    //Sometimes, exceptions should be passed back to the library.
    static void*
    convertible(PyObject* py_obj){
        if( 1 != PyObject_IsInstance( py_obj, PyExc_Exception ) ){
            return 0;
        }
        
        if( !PyObject_HasAttrString( py_obj, "_pimpl" ) ){
            return 0;
        }
        
        bpl::object pyerr( bpl::handle<>( bpl::borrowed( py_obj ) ) );        
        bpl::object pimpl = bpl::getattr( pyerr, "_pimpl" );
        bpl::extract<application_error> type_checker( pimpl );
        if( !type_checker.check() ){
            return 0;
        }
        return py_obj;
    }

    static void
    construct( PyObject* py_obj, bpl::converter::rvalue_from_python_stage1_data* data){
        typedef bpl::converter::rvalue_from_python_storage<application_error> storage_t;
        
        bpl::object pyerr( bpl::handle<>( bpl::borrowed( py_obj ) ) );        
        bpl::object pimpl = bpl::getattr( pyerr, "_pimpl" );
        
        storage_t* the_storage = reinterpret_cast<storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;
        application_error* cpp_err 
            = new (memory_chunk) application_error( bpl::extract<application_error>(pimpl) );
        
        data->convertible = memory_chunk;
    }

};

BOOST_PYTHON_MODULE( my_exceptions ){
    
    typedef bpl::return_value_policy< bpl::copy_const_reference > return_copy_const_ref;
    bpl::class_< application_error >( "_application_error_" )
        .def( bpl::init<const std::string&>() )
        .def( bpl::init<const application_error&>() )
        .def( "application_name", &application_error::application_name)
        .def( "message", &application_error::message, return_copy_const_ref() )
        .def( "__str__", &application_error::message, return_copy_const_ref() );
    
    exception_translator();
    
    bpl::def( "check_preconditions", &::check_preconditions );
    bpl::def( "get_application_name", &::get_application_name );
}

