#include "boost/python.hpp"
#include "classes.hpp"

namespace bp = boost::python;

namespace smart_pointers{
    // "get_pointer" function returns pointer to the object managed by smart pointer
    // class instance

    template<class T>
    inline T * get_pointer(smart_pointers::smart_ptr_t<T> const& p){
        return p.get();
    }

    inline derived_t * get_pointer(derived_ptr_t const& p){
        return p.get();
    }
}

namespace boost{ namespace python{

    using boost::get_pointer;
    
    // "pointee" class tells Boost.Python the type of the object managed by smart 
    // pointer class.
    // You can read more about "pointee" class here:
    // http://boost.org/libs/python/doc/v2/pointee.html 

    template <class T>
    struct pointee< smart_pointers::smart_ptr_t<T> >{
        typedef T type;
    };

    template<>
    struct pointee< derived_ptr_t >{
        typedef derived_t type;
    };

} }


// "get_pointer" and "pointee" are needed, in order to allow Boost.Python to 
// work with user defined smart pointer

struct base_wrapper_t : base_i, bp::wrapper< base_i > {

    base_wrapper_t()
    : base_i(), bp::wrapper< base_i >()
    {}

    virtual int get_value(  ) const {
        bp::override func_get_value = this->get_override( "get_value" );
        return func_get_value(  );
    }

};

struct derived_wrapper_t : derived_t, bp::wrapper< derived_t > {

    derived_wrapper_t()
    : derived_t(), bp::wrapper< derived_t >()
    {}

    derived_wrapper_t(const derived_t& d)
    : derived_t(d), bp::wrapper< derived_t >()
    {}

    derived_wrapper_t(const derived_wrapper_t&)
    : derived_t(), bp::wrapper< derived_t >()
    {}

    virtual int get_value() const  {
        if( bp::override func_get_value = this->get_override( "get_value" ) )
            return func_get_value(  );
        else
            return derived_t::get_value(  );
    }

    int default_get_value() const  {
        return derived_t::get_value( );
    }

};

BOOST_PYTHON_MODULE( custom_sptr ){
    bp::class_< base_wrapper_t
                , boost::noncopyable
                , smart_pointers::smart_ptr_t< base_wrapper_t > >( "base_i" )
    // -----------^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // HeldType of the abstract class, which is managed by custom smart pointer
    // should be smart_pointers::smart_ptr_t< base_wrapper_t >.
        .def( "get_value", bp::pure_virtual( &base_i::get_value ) );

    // Register implicit conversion between smart pointers. Boost.Python library
    // can not discover relationship between classes. You have to tell about the 
    // relationship to it. This will allow Boost.Python to treat right, the 
    // functions, which expect to get as argument smart_pointers::smart_ptr_t< base_i > class 
    // instance, when smart_pointers::smart_ptr_t< derived from base_i > class instance is passed.
    //
    // For more information about implicitly_convertible see the documentation:
    // http://boost.org/libs/python/doc/v2/implicit.html .
    bp::implicitly_convertible< 
                  smart_pointers::smart_ptr_t< base_wrapper_t >
                , smart_pointers::smart_ptr_t< base_i > >();
    
    // The register_ptr_to_python functionality is explaned very well in the 
    // documentation:
    // http://boost.org/libs/python/doc/v2/register_ptr_to_python.html .
    bp::register_ptr_to_python< smart_pointers::smart_ptr_t< base_i > >();

    bp::class_< derived_wrapper_t
                , bp::bases< base_i >
                , smart_pointers::smart_ptr_t<derived_wrapper_t> >( "derived_t" )
    // -----------^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // Pay attention on the class HeldType. It will allow us to create new classes
    // in Python, which derive from the derived_t class.
        .def( "get_value", &derived_t::get_value, &derived_wrapper_t::default_get_value );

    // Now register all existing conversion:
    bp::implicitly_convertible< 
                  smart_pointers::smart_ptr_t< derived_wrapper_t >
                , smart_pointers::smart_ptr_t< derived_t > >();
    bp::implicitly_convertible< 
                  smart_pointers::smart_ptr_t< derived_t >
                  , smart_pointers::smart_ptr_t< base_i > >();
    bp::implicitly_convertible< 
                    derived_ptr_t
                  , smart_pointers::smart_ptr_t< derived_t > >();
    bp::register_ptr_to_python< derived_ptr_t >();

    bp::def( "const_ref_get_value", &::const_ref_get_value );
    bp::def( "ref_get_value", &::ref_get_value );
    bp::def( "val_get_value", &::val_get_value );
    bp::def( "create_derived", &::create_derived );
    bp::def( "create_base", &::create_base );


    bp::class_< numeric_t, smart_pointers::smart_ptr_t< numeric_t > >( "numeric_t" )
        .def_readwrite( "value", &numeric_t::value );

    bp::def( "create_numeric", &::create_numeric );
    bp::def( "get_numeric_value", &::get_numeric_value );

    // Work around for the public member variable, where type of the variable
    // is smart pointer problem
    bp::class_< shared_data::buffer_t >( "buffer_t" )
        .def_readwrite( "size", &shared_data::buffer_t::size );
    
    bp::register_ptr_to_python< smart_pointers::smart_ptr_t< shared_data::buffer_t > >();

    bp::class_< shared_data::buffer_holder_t >( "buffer_holder_t" )
        .def( "get_data", &shared_data::buffer_holder_t::get_data )
        .def_readwrite( "data_naive", &shared_data::buffer_holder_t::data )
        // If you will try to access "data_naive" you will get
        // TypeError: No Python class registered for C++ class smart_pointers::smart_ptr_t<shared_data::buffer_t>
        // Next lines of code contain work around
        .add_property( "data"
            , bp::make_getter( &shared_data::buffer_holder_t::data
                               , bp::return_value_policy<bp::copy_non_const_reference>() ) 
            , bp::make_setter( &shared_data::buffer_holder_t::data ) );

}
