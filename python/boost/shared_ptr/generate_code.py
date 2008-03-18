#The code contained in this file will show you how to instruct Py++ to generate 
#code, that uses your smart pointer class.
#

#Advise:
#Using your favorite editor create regular C++ header file and to it next code.
#You don't really need to generate it every time.
#
# template<class T>
# inline T * get_pointer(smart_ptr_t<T> const& p){
#     return p.get();
# }
#
# template <class T>
# struct pointee< smart_ptr_t<T> >{
#     typedef T type;
# };


HELD_TYPE_TMPL = \
"""
namespace boost{ namespace python{

%(class_name)s* get_pointer( %(class_ptr_name)s const& p ){
    return p.get();
}

template <>
struct pointee< %(class_ptr_name)s >{
    typedef %(class_name)s type;
};

}}// namespace boost::python
"""

REGISTER_PTR_TO_PYTHON_TMPL = \
"""
boost::python::register_ptr_to_python< %(sp_inst_class_name)s >();
"""

IMPLICITLY_CONVERTIBLE_TMPL = \
"""
boost::python::implicitly_convertible< %(derived)s, %(base)s >();
"""


def get_pointee( sp_instantiation ):
    #sp_instantiation - reference to smart_ptr_t<XXX>
    #returns reference to XXX type/declaration
    #I will find m_managed member variable and will find out its type
    no_ptr = declarations.remove_pointer( sp_instantiation.variable ('m_managed').type )
    no_alias = declarations.remove_alias( no_ptr )
    return declarations.remove_declarated( no_alias )

def expose_single( sp_instantiation ):
    #This function instructs Py++ how to expose pointee class functionality
    #related to your smart pointer
    #sp_instantiation - reference to smart_ptr_t<XXX>
    
    # You don't need to expose smart_ptr_t< X > class
    sp_instantiation.exclude() 

    pointee = get_pointee( sp_instantiation )
    
    #Our example defines derived_ptr_t class:
    #struct derived_ptr_t : public smart_ptr_t< derived_t >
    #{...};
    #Next if checks that smart_ptr_t< XXX > class has a derived class
    #and "exposes" it.
    if sp_instantiation.derived:
        assert 1 == len( sp_instantiation.derived )
        sp_derived = sp_instantiation.derived[0].related_class
        #You don't want to expose it
        sp_derived.exclude()

        #Adding your custom code to pointee class. 
        #You don't have to warry about order or place of generated code,
        #Py++ does it right.
        
        #Telling Boost.Python how to work with your smart pointer
        pointee.add_declaration_code(
            HELD_TYPE_TMPL % { 'class_name': pointee.decl_string
                                       , 'class_ptr_name': sp_derived.decl_string } )

        #Telling Boost.Python about relationship between classes
        pointee.add_registration_code(
            IMPLICITLY_CONVERTIBLE_TMPL % { 'derived' : sp_derived.decl_string
                                         , 'base' : sp_instantiation.decl_string }
            , works_on_instance=False )

        pointee.add_registration_code(
            REGISTER_PTR_TO_PYTHON_TMPL % { 'sp_inst_class_name' : sp_derived.decl_string }
            , works_on_instance=False )

    #Setting class held type
    pointee.held_type = 'smart_ptr_t< %s >' % pointee.wrapper_alias
    
    #Registering relationships between classes
    pointee.add_registration_code(
        IMPLICITLY_CONVERTIBLE_TMPL % { 'derived' : pointee.held_type
                                     , 'base' : sp_instantiation.decl_string }
        , works_on_instance=False )

    pointee.add_registration_code(
        REGISTER_PTR_TO_PYTHON_TMPL % { 'sp_inst_class_name' : sp_instantiation.decl_string }
        , works_on_instance=False )

    #Registering relationships between classes
    base_classes = filter( lambda hierarchy_info: hierarchy_info.access_type == 'public', pointee.bases )
    for base in base_classes:
        pointee.add_registration_code(
            IMPLICITLY_CONVERTIBLE_TMPL % { 'derived' : sp_instantiation.decl_string
                                         , 'base' : 'smart_ptr_t< %s >' % base.related_class.decl_string }
            , works_on_instance=False)

def expose(mb):
    sp_instantiations = mb.classes( lambda decl: decl.name.startswith( 'smart_ptr_t' ) )
    map( lambda sp: expose_single( sp ), sp_instantiations )


