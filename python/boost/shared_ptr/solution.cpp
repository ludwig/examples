#include "boost/python.hpp"
#include <string>

namespace boost{ 
    
    template<class T> 
    inline T* get_pointer( boost::shared_ptr<const T> const& p ){
        return const_cast< T* >( p.get() );
    }

}

namespace boost{ namespace python{    
    
    template<class T> 
    struct pointee< boost::shared_ptr<T const> >{
        typedef T type;
    };
    
} } //boost::python

struct info_t{
    //class info_t records in what function it was created information
    info_t( const std::string& n )
    : text( n )
    {}
    
    std::string text;
};

typedef boost::shared_ptr< info_t > ptr_t;
typedef boost::shared_ptr< const info_t > const_ptr_t;

ptr_t create_ptr(){
    return ptr_t( new info_t( "ptr" ) );
}

const_ptr_t create_const_ptr(){
    return const_ptr_t( new info_t( "const ptr" ) );
}

std::string read_ptr( ptr_t x ){
    if( !x )
        return "";
    return x->text;
}

std::string read_const_ptr( const_ptr_t x ){
    if( !x )
        return "";
    return x->text;
}

namespace bpl = boost::python;

namespace utils{
    
    template< class T >
    void register_shared_ptrs_to_python(){
        //small helper function, which will register shared_ptr conversions
        bpl::register_ptr_to_python< boost::shared_ptr< T > >();
        bpl::register_ptr_to_python< boost::shared_ptr< const T > >();
        bpl::implicitly_convertible< boost::shared_ptr< T >, boost::shared_ptr< const T > >();
    }
    
}

BOOST_PYTHON_MODULE( shared_ptr ){
    bpl::class_< info_t >( "info_t", bpl::init< std::string >() )
        .add_property( "text", &info_t::text );
    utils::register_shared_ptrs_to_python< info_t >();
    
    bpl::def( "create_ptr", &create_ptr );
    bpl::def( "create_const_ptr", &create_const_ptr );
    bpl::def( "read_ptr", &read_ptr );
    bpl::def( "read_const_ptr", &read_const_ptr );
    
}
