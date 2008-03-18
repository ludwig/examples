#include "boost/python.hpp"
#include "boost/python/object.hpp"  //len function
#include "boost/python/ssize_t.hpp" //ssize_t type definition
#include "boost/python/detail/none.hpp"
#include "tuples.hpp"

/**
 * Custom r-value converter example.
 *
 * Use-case description. I and few other developers work on Python bindings for
 * Ogre (http://ogre3d.org). The engine defines ColourValue class. This class
 * describes colour using 4 components: red, green, blue and transparency. The
 * class is used through the whole engine. One of the first features users ask
 * is to add an ability to pass a tuple, instead of the "ColourValue" instance.
 * This feature would allow them to write less code:
 *
 *   x.do_smth( (1,2,3,4) )
 *
 * instead of
 *
 *   x.do_smth( ogre.ColourValue( 1,2,3,4 ) )
 *
 * That's not all. They also wanted to be able to use ColourValue functionality.
 *
 * Solution.
 *
 * Fortunately, Boost.Python library provides enough functionality to implement
 * users requirements - r-value converters.
 *
 * R-Value converters allows to register custom conversion from Python type to
 * C++ type. The conversion will be handled by Boost.Python library automaticly
 * "on-the-fly".
 *
 * The example introduces "colour_t" class and few testers.
 *
 **/


struct colour_t{
    explicit colour_t( float red_=0.0, float green_=0.0, float blue_=0.0 )
    : red( red_ ), green( green_ ), blue( blue_ )
    {}

    bool operator==( const colour_t& other ) const{
        return red == other.red && green == other.green && blue == other.blue;
    }

    float red, green, blue;
};

struct desktop_t{
    bool is_same_colour( const colour_t& colour ) const{
        return colour == background;
    }
    colour_t background;
};

namespace bpl = boost::python;

struct pytuple2colour{

    typedef boost::tuples::tuple< float, float, float> colour_tuple_type;

    typedef bpl::from_py_sequence< colour_tuple_type > converter_type;

    static void* convertible(PyObject* obj){
        return converter_type::convertible( obj );
    }

    static void
    construct( PyObject* obj, bpl::converter::rvalue_from_python_stage1_data* data){
        typedef bpl::converter::rvalue_from_python_storage<colour_t> colour_storage_t;
        colour_storage_t* the_storage = reinterpret_cast<colour_storage_t*>( data );
        void* memory_chunk = the_storage->storage.bytes;

        float red(0.0), green(0.0), blue(0.0);
        boost::tuples::tie(red, green, blue) = converter_type::to_c_tuple( obj );

        colour_t* colour = new (memory_chunk) colour_t(red, green, blue);
        data->convertible = memory_chunk;
    }
};

void register_pytuple2colour(){
    bpl::converter::registry::push_back(  &pytuple2colour::convertible
                                         , &pytuple2colour::construct
                                         , bpl::type_id<colour_t>() );
}



bool test_val_010( colour_t colour ){
    return colour == colour_t( 0, 1, 0);
}

bool test_cref_000( const colour_t& colour ){
    return colour == colour_t( 0, 0, 0);
}

bool test_ref_111( colour_t& colour ){
    return colour == colour_t( 1, 1, 1);
}

bool test_ptr_101( colour_t* colour ){
    return colour && *colour == colour_t( 1, 0, 1);
}

bool test_cptr_110( const colour_t* colour ){
    return colour && *colour == colour_t( 1, 1, 0);
}

BOOST_PYTHON_MODULE( custom_rvalue ){
    bpl::class_< colour_t >( "colour_t" )
        .def( bpl::init< bpl::optional< float, float, float > >(
              ( bpl::arg("red_")=0.0, bpl::arg("green_")=0.0, bpl::arg("blue_")=0.0 ) ) )
        .def_readwrite( "red", &colour_t::red )
        .def_readwrite( "green", &colour_t::green )
        .def_readwrite( "blue", &colour_t::blue );
    register_pytuple2colour();

    bpl::class_< desktop_t >( "desktop_t" )
        //naive aproach that will not work - plain Python assignment
        //.def_readwrite( "background", &desktop_t::background )
        //You should use properties to force the conversion
        .add_property( "background"
                       , bpl::make_getter( &desktop_t::background )
                       , bpl::make_setter( &desktop_t::background ) )
        .def( "is_same_colour", &desktop_t::is_same_colour );

    bpl::def("test_val_010", &::test_val_010);
    bpl::def("test_cref_000", &::test_cref_000);
    bpl::def("test_ref_111", &::test_ref_111);
    bpl::def("test_ptr_101", &::test_ptr_101);
    bpl::def("test_cptr_110", &::test_cptr_110);
}
