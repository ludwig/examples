#ifndef smart_ptr_t_19_09_2006
#define smart_ptr_t_19_09_2006

#include <assert.h>

//The smart_ptr_t class has been created based on Ogre::SharedPtr class
//http://www.ogre3d.org/docs/api/html/OgreSharedPtr_8h-source.html

namespace smart_pointers{

template<class T> 
class smart_ptr_t {    
protected:
    T* m_managed;
    unsigned int* m_use_count;
public:

    smart_ptr_t()
    : m_managed(0), m_use_count(0)
    {}

    //rep should not be NULL
    explicit smart_ptr_t(T* rep)
    : m_managed(rep), m_use_count( new unsigned int(1) )
    {}

    //Every custom smart pointer class should have copy constructor and 
    //assignment operator. Probably your smart pointer class already has this
    //functionality.

    smart_ptr_t(const smart_ptr_t& r)
    : m_managed(0), m_use_count(0)
    {
        m_managed = r.get();
        m_use_count = r.use_count_ptr();
        if(m_use_count){
            ++(*m_use_count);
        }
    }

    smart_ptr_t& operator=(const smart_ptr_t& r){
        if( m_managed == r.m_managed ){
            return *this;
        }
        release();
            
        m_managed = r.get();
        m_use_count = r.use_count_ptr();
        if(m_use_count){
            ++(*m_use_count);
        }
        return *this;
    }

    //Next two functions allows to construct smart pointer from an existing one,
    //which manages object with a different type.
    //For example:
    //    struct base{...};
    //    struct derived : base { ... };
    //    smart_ptr_t< base > b( smart_ptr_t<derived>() );
    //
    //This functionality is very important for C++ Python bindings. It will allow
    //you to register smart pointer conversion:
    //    boost::python::implicitly_convertible< smart_ptr_t< derived >, smart_ptr_t< base > >();
    template<class Y>
    smart_ptr_t(const smart_ptr_t<Y>& r)
    : m_managed(0), m_use_count(0)
    {
        m_managed = r.get();
        m_use_count = r.use_count_ptr();
        if(m_use_count){
            ++(*m_use_count);
        }
    }

    template< class Y>
    smart_ptr_t& operator=(const smart_ptr_t<Y>& r){
        if( m_managed == r.m_managed ){
            return *this;
        }
        release();
            
        m_managed = r.get();
        m_use_count = r.use_count_ptr();
        if(m_use_count){
            ++(*m_use_count);
        }
        return *this;
    }

    virtual ~smart_ptr_t() {
        release();
    }

    inline T& operator*() const {
        assert(m_managed); return *m_managed;
    }

    inline T* operator->() const {
        assert(m_managed); return m_managed;
    }

    inline T* get() const {
        return m_managed;
    }

    inline unsigned int* use_count_ptr() const {
        return m_use_count;
    }

protected:

    inline void release(void){
        bool destroy_this = false;

        if( m_use_count ){
            if( --(*m_use_count) == 0){
                destroy_this = true;
            }
        }
        if (destroy_this){
            destroy();
        }
    }

    virtual void destroy(void){
        delete m_managed;
        delete m_use_count;
    }
};

} //smart_pointers

#endif //smart_ptr_t_19_09_2006

