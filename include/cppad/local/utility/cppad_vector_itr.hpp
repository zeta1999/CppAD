# ifndef CPPAD_LOCAL_UTILITY_CPPAD_VECTOR_ITR_HPP
# define CPPAD_LOCAL_UTILITY_CPPAD_VECTOR_ITR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cstddef>
# include <cppad/core/cppad_assert.hpp>
/*
------------------------------------------------------------------------------
$begin cppad_vector_itr_define$$
$spell
    Iterator
    cppad
    itr
    undef
    const
    endif
    hpp
$$

$section Vector Class Iterator Preprocessor Definitions$$

$head Syntax$$
$codep
# define CPPAD_CONST 0
# include <cppad/local/utility/cppad_vector_itr.hpp>
# undef CPPAD_LOCAL_UTILITY_CPPAD_VECTOR_ITR_HPP
# define CPPAD_CONST 1
# include <cppad/local/utility/cppad_vector_itr.hpp>
%$$

$head Beginning of cppad_vector_itr.hpp$$
The following preprocessor definition appears at the beginning of
$code cppad_vector_itr.hpp$$ and is used for the class definition in this file:
$codep
# if CPPAD_CONST
# define CPPAD_VECTOR_ITR const_cppad_vector_itr
# else
# define CPPAD_VECTOR_ITR cppad_vector_itr
# endif
$$

$head End of cppad_vector_itr.hpp$$
The following preprocessor definition appears at the end of
$code cppad_vector_itr.hpp$$ so that it can be included with a different
value for $code CPPAD_CONST$$:
$codep
# undef CPPAD_CONST
# undef CPPAD_VECTOR_ITR
$$

$end
*/
# if CPPAD_CONST
# define CPPAD_VECTOR_ITR const_cppad_vector_itr
# else
# define CPPAD_VECTOR_ITR cppad_vector_itr
# endif

// BEGIN_CPPAD_LOCAL_UTILITY_NAMESPACE
namespace CppAD { namespace local { namespace utility {


// ==========================================================================
template <class Type> class CPPAD_VECTOR_ITR {
// ==========================================================================
/*
-----------------------------------------------------------------------------
$begin cppad_vector_itr_traits$$
$spell
    Iterator
$$

$section Vector Class Iterator Traits and Friends$$

$srccode%hpp% */
# if ! CPPAD_CONST
    template <class T> class const_cppad_vector_itr;
    friend const_cppad_vector_itr<Type>;
# endif
public:
    typedef std::bidirectional_iterator_tag    iterator_category;
    typedef Type                               value_type;
    typedef std::ptrdiff_t                     difference_type;
    typedef Type*                              pointer;
    typedef Type&                              reference;
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_itr_ctor$$
$spell
    Iterator
    ptr
    cppad
    Namespace
    CppAD
    const
    iterators
    itr
$$

$section Vector Class Iterator Member Data and Constructors$$

$head Constructors$$

$subhead Constant$$
$codei%const_cppad_vector_itr %itr%()
%$$
$codei%const_cppad_vector_itr %itr%(%data%, %length%, %index%)
%$$
$codei%const_cppad_vector_itr %itr%(%other%)
%$$
$codei%const_cppad_vector_itr %itr%(%not_const_other%)
%$$

$subhead Not Constant$$
$codei%cppad_vector_itr %itr%()
%$$
$codei%cppad_vector_itr %itr%(%data%, %length%, %index%)
%$$
$codei%cppad_vector_itr %itr%(%other%)
%$$

$head Namespace$$
These definitions are in the $code CppAD::local::utility$$ namespace.

$head Indirection$$
We use an extra level of indirection in this routine so that
the iterator has the same values as the vector even if the vector changes.

$head data_$$
is a pointer to a constant pointer to data for this vector
(used by operations that are not supported by constant iterators).

$head length_$$
is a pointer to the length of the corresponding vector.

$head index_$$
is the current vector index corresponding to this iterator.

$head check_element$$
generates an assert with a known cause when the $code index_$$
does not correspond go a valid element and
$code NDEBUG$$ is not defined.

$head Source$$
$srccode%hpp% */
private:
# if CPPAD_CONST
    const Type* const* data_;
# else
    Type* const*       data_;
# endif
    const size_t*      length_;
    size_t             index_;
    void check_element(void) const
    {   CPPAD_ASSERT_KNOWN( index_ < *length_,
            "CppAD vector iterator: accessing element out of range"
        );
    }
public:
    CPPAD_VECTOR_ITR(void)
    : data_(CPPAD_NULL), length_(CPPAD_NULL), index_(0)
    { }
# if CPPAD_CONST
    CPPAD_VECTOR_ITR(
        const Type* const* data, const size_t* length, size_t index)
    : data_(data), length_(length), index_(index)
    { }
# else
    CPPAD_VECTOR_ITR(Type* const* data, const size_t* length, size_t index)
    : data_(data), length_(length), index_(index)
    { }
# endif
    void operator=(const CPPAD_VECTOR_ITR& other)
    {   data_       = other.data_;
        length_     = other.length_;
        index_      = other.index_;
    }
    CPPAD_VECTOR_ITR(const CPPAD_VECTOR_ITR& other)
    {   *this = other; }
# if CPPAD_CONST
    // assign a const_iterator to an iterator
    void operator=(const cppad_vector_itr<Type>& not_const_other)
    {   data_       = not_const_other.data_;
        length_     = not_const_other.length_;
        index_      = not_const_other.index_;
    }
# endif
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_itr_inc$$
$spell
    Iterator
    itr
$$

$section Vector Class Iterator Increment Operators$$

$head Syntax$$
$codei%++%itr%
%$$
$codei%--%itr%
%$$
$icode%itr%++
%$$
$icode%itr%--
%$$

$head Source$$
$srccode%hpp% */
public:
    CPPAD_VECTOR_ITR& operator++(void)
    {   ++index_;
        return *this;
    }
    CPPAD_VECTOR_ITR& operator--(void)
    {   --index_;
        return *this;
    }
    CPPAD_VECTOR_ITR operator++(int)
    {   CPPAD_VECTOR_ITR ret(*this);
        ++index_;
        return ret;
    }
    CPPAD_VECTOR_ITR operator--(int)
    {   CPPAD_VECTOR_ITR ret(*this);
        --index_;
        return ret;
    }
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_itr_equal$$
$spell
    itr
    Iterator
$$

$section Vector Class Iterator Equality Operators$$

$head Syntax$$
$icode%itr% == %other%
%$$
$icode%itr% != %other%
%$$

$head Source$$
$srccode%hpp% */
public:
    bool operator==(const CPPAD_VECTOR_ITR& other) const
    {   bool ret = data_ == other.data_;
        ret      &= index_ == other.index_;
        return ret;
    }
    bool operator!=(const CPPAD_VECTOR_ITR& other) const
    {   return  ! ( *this == other ); }
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_itr_element$$
$spell
    itr
    Iterator
$$

$section Vector Class Iterator Access Elements$$

$head Syntax$$
$icode%element% = *%itr%
%$$
$codei%*%itr% = %element%
%$$

$head Source$$
$srccode%hpp% */
public:
    const Type& operator*(void) const
    {   check_element();
        return (*data_)[index_];
    }
# if ! CPPAD_CONST
    Type& operator*(void)
    {   check_element();
        return (*data_)[index_];
    }
# endif
/* %$$
$end
*/
// ==========================================================================
}; // END_TEMPLATE_CLASS_CPPAD_VECTOR_ITR
// ==========================================================================
} } } // END_CPPAD_LOCAL_UTILITY_NAMESPACE

# undef CPPAD_CONST
# undef CPPAD_VECTOR_ITR
# endif