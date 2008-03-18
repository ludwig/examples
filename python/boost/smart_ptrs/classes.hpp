#ifndef classes_11_11_2006
#define classes_11_11_2006

#include "smart_ptr.h"

struct base_i{
public:
    virtual ~base_i() {}
    virtual int get_value() const = 0;
};

struct derived_t : base_i{
    derived_t(){}    
    virtual int get_value() const{ return 0xD; }
};

// Some smart pointer classes does not have reach interface as boost ones.
// In order to provide same level of convenience, users are forced to create
// classes, which derive from smart pointer class.
struct derived_ptr_t : public smart_pointers::smart_ptr_t< derived_t >{

    derived_ptr_t()
    : smart_pointers::smart_ptr_t< derived_t >()
    {}

    explicit derived_ptr_t(derived_t* rep)
    : smart_pointers::smart_ptr_t<derived_t>(rep)
    {}

    derived_ptr_t(const derived_ptr_t& r)
    : smart_pointers::smart_ptr_t<derived_t>(r) {}

    derived_ptr_t( const smart_pointers::smart_ptr_t< base_i >& r)
    : smart_pointers::smart_ptr_t<derived_t>()
    {
        m_managed = static_cast<derived_t*>(r.get());
        m_use_count = r.use_count_ptr();
        if (m_use_count)
        {
            ++(*m_use_count);
        }
    }

    derived_ptr_t& operator=(const smart_pointers::smart_ptr_t< base_i >& r)
    {
        if (m_managed == static_cast<derived_t*>(r.get()))
            return *this;
        release();
        m_managed = static_cast<derived_t*>(r.get());
        m_use_count = r.use_count_ptr();
        if (m_use_count)
        {
            ++(*m_use_count);
        }
        
        return *this;
    }
};

// Few functions that will be used to test custom smart pointer functionality
// from Python.
derived_ptr_t create_derived(){
    return derived_ptr_t( new derived_t() );
}

smart_pointers::smart_ptr_t< base_i > create_base(){
    return smart_pointers::smart_ptr_t< base_i >( new derived_t() );
}


// Next function could be exposed, but it could not be called from Python, when
// the argument is the instance of a derived class.
//
// This is the explanation David Abrahams gave:
//   Naturally; there is no instance of smart_pointers::smart_ptr_t<base_i> anywhere in the
//   Python object for the reference to bind to. The rules are the same as in C++:
//
//   int f(smart_pointers::smart_ptr_t<base>& x);
//   smart_pointers::smart_ptr_t<derived> y;
//   int z = f(y);               // fails to compile

inline int
ref_get_value( smart_pointers::smart_ptr_t< base_i >& a ){
    return a->get_value();
}

inline int
val_get_value( smart_pointers::smart_ptr_t< base_i > a ){
    return a->get_value();
}

inline int
const_ref_get_value( const smart_pointers::smart_ptr_t< base_i >& a ){
    return a->get_value();
}


struct numeric_t{
    numeric_t()
    : value(0)
    {}
        
    int value;
};

smart_pointers::smart_ptr_t< numeric_t > create_numeric( int value ){
    smart_pointers::smart_ptr_t< numeric_t > num( new numeric_t() );
    num->value = value;
    return num;
}

int get_numeric_value( smart_pointers::smart_ptr_t< numeric_t > n ){
    if( n.get() ){
        return n->value;
    }
    else{
        return 0;
    }
}



namespace shared_data{

// Boost.Python has small problem with user defined smart pointers and public
// member variables, exposed using def_readonly, def_readwrite functionality
// Read carefully "make_getter" documentation.
// http://boost.org/libs/python/doc/v2/data_members.html#make_getter-spec
// bindings.cpp contains solution to the problem.
    
struct buffer_t{ 
    buffer_t() : size(0) {}
    int size; 
};

struct buffer_holder_t{
    buffer_holder_t()
    : data( new buffer_t() )
    {}
        
    smart_pointers::smart_ptr_t< buffer_t > get_data(){ return data; }
    
    smart_pointers::smart_ptr_t< buffer_t > data;
};

}

#endif//classes_11_11_2006

