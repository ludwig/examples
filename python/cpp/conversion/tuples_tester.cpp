#include "boost/python.hpp"
#include "boost/tuple/tuple_comparison.hpp"
#include "tuples.hpp"

/**
 * Content:
 *  * few testers for  boost::tuples::tuple<...> to\from Python tuple conversion
 *    functionality
 *
 *  * example of custom r-value converter for a registered class
 *
 *
 **/

typedef boost::tuple< int, int, int > triplet_type;

triplet_type triplet_ret_val_000() {
    return triplet_type(0,0,0);
}

triplet_type triplet_ret_val_101() {
    return triplet_type(1,0,1);
}

triplet_type& triplet_ret_ref_010(){
    static triplet_type pt( 0,1,0 );
    return pt;
}

triplet_type* triplet_ret_ptr_110(){
    static triplet_type pt( 1,1,0 );
    return &pt;
}

bool test_triplet_val_000( triplet_type pt ){
    return pt == triplet_type( 0,0,0 );
}

bool test_triplet_cref_010( const triplet_type& pt ){
    return pt == triplet_type( 0,1,0 );
}

bool test_triplet_ref_110( triplet_type& pt ){
    return pt == triplet_type( 1,1,0 );
}

bool test_triplet_ptr_101( triplet_type* pt ){
    return pt && *pt == triplet_type( 1,0,1 );
}


namespace bpl = boost::python;

BOOST_PYTHON_MODULE( tuples ){

    bpl::register_tuple< triplet_type >();

    bpl::def("triplet_ret_val_000", &::triplet_ret_val_000);
    bpl::def("triplet_ret_val_101", &::triplet_ret_val_101);
    bpl::def("triplet_ret_ref_010"
             , &::triplet_ret_ref_010
             , bpl::return_value_policy<bpl:: copy_non_const_reference>() );
    bpl::def( "triplet_ret_ptr_110"
              , &::triplet_ret_ptr_110
              , bpl::return_value_policy<bpl::return_by_value>() );
    bpl::def("test_triplet_val_000", &::test_triplet_val_000);
    bpl::def("test_triplet_cref_010", &::test_triplet_cref_010);
    bpl::def("test_triplet_ref_110", &::test_triplet_ref_110);
    bpl::def("test_triplet_ptr_101", &::test_triplet_ptr_101);

}

