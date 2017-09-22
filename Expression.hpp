//
// Created by lee on 17/9/21.
//

#ifndef TESTEXPRESSIONTEMPLATE_EXPRESSION_HPP
#define TESTEXPRESSIONTEMPLATE_EXPRESSION_HPP

#include <tuple>
#include <utility>
#include <cstddef>

namespace molly
{
    namespace tags
    {
        // tags
#define DEF_UNARY_TAG(tagname, op) \
        struct tagname##_tag \
        { \
            enum { Arity = 1 }; \
            template<typename T> \
            decltype(auto) operator()(T&& t) \
            { \
                return op t; \
            } \
        }
        
        DEF_UNARY_TAG(unary_plus, +);
        DEF_UNARY_TAG(unary_minus, -);
        DEF_UNARY_TAG(dereference, *);
        DEF_UNARY_TAG(addressof, &);
        DEF_UNARY_TAG(pre_increment, ++);
        DEF_UNARY_TAG(pre_decrement, --);
        struct post_increment_tag
        {
            enum { Arity = 1 };
            template<typename T>
            decltype(auto) operator()(T&& t)
            {
                return t++;
            }
        };
        struct post_decrement_tag
        {
            enum { Arity = 1 };
            template<typename T>
            decltype(auto) operator()(T&& t)
            {
                return t--;
            }
        };
        DEF_UNARY_TAG(bitwise_not, ~);
        DEF_UNARY_TAG(logical_not, !);
        
#undef DEF_UNARY_TAG

#define DEF_BINARY_TAG(tagname, op) \
        struct tagname##_tag \
        { \
            enum { Arity = 2 }; \
            template<typename T, typename U> \
            decltype(auto) operator()(T&& t, U&& u) \
            { \
                return t op u; \
            } \
        }
        
        DEF_BINARY_TAG(binary_plus, +);
        DEF_BINARY_TAG(binary_minus, -);
        DEF_BINARY_TAG(multiplies, *);
        DEF_BINARY_TAG(divides, /);
        DEF_BINARY_TAG(modulus, %);
        DEF_BINARY_TAG(bitwise_and, &);
        DEF_BINARY_TAG(bitwise_or, |);
        DEF_BINARY_TAG(bitwise_xor, ^);
        DEF_BINARY_TAG(shift_left, <<);
        DEF_BINARY_TAG(shift_right, >>);
        DEF_BINARY_TAG(assign, =);
        DEF_BINARY_TAG(plus_assign, +=);
        DEF_BINARY_TAG(minus_assign, -=);
        DEF_BINARY_TAG(multiplies_assign, *=);
        DEF_BINARY_TAG(divides_assign, /=);
        DEF_BINARY_TAG(modulus_assign, %=);
        DEF_BINARY_TAG(bitwise_and_assign, &=);
        DEF_BINARY_TAG(bitwise_or_assign, |=);
        DEF_BINARY_TAG(bitwise_xor_assign, ^=);
        DEF_BINARY_TAG(shift_left_assign, <<=);
        DEF_BINARY_TAG(shift_right_assign, >>=);
        DEF_BINARY_TAG(logical_and, &&);
        DEF_BINARY_TAG(logical_or, ||);
        DEF_BINARY_TAG(less, <);
        DEF_BINARY_TAG(less_equal, <=);
        DEF_BINARY_TAG(greater, >);
        DEF_BINARY_TAG(greater_equal, >=);
        DEF_BINARY_TAG(equal, ==);
        DEF_BINARY_TAG(not_equal, !=);
        struct subscript_tag
        {
            enum { Arity = 2 };
            template<typename T, typename U>
            decltype(auto) operator()(T&& t, U&& u)
            {
                return t[u];
            }
        };
        DEF_BINARY_TAG(member_pointer_access, ->*);
        struct comma_tag
        {
            enum { Arity = 2 };
            template<typename T, typename U>
            decltype(auto) operator()(T&& t, U&& u)
            {
                return t, u;
            }
        };
        
#undef DEF_BINARY_TAG
        
        struct ternary_tag
        {
            enum { Arity = 3 };
            template<typename T, typename U, typename V>
            decltype(auto) operator()(T&& t, U&& u, V&& v)
            {
                return t ? u : v;
            };
        };
    }
    
    namespace arg_names
    {
        // argument
        template<std::size_t I>
        class argument
        {
        public:
            // typedefs
            static constexpr std::size_t index = I;
            
            // functions
            template<typename... Args>
            decltype(auto) operator()(Args&&... args)
            {
                return std::get<index - 1>(std::make_tuple(std::forward<Args>(args)...));
            }
        };
        
        // names
        const argument<1> _1 = {};
        const argument<2> _2 = {};
        const argument<3> _3 = {};
        const argument<4> _4 = {};
        const argument<5> _5 = {};
    }
    
    namespace expressions
    {
        // literal
        template<typename T>
        class literal
        {
            // members
            T value;
            
        public:
            // typedefs
            typedef T value_type;
            
            // functions
            literal(const value_type& v) : value(v) {}
            
            const T& get() const { return value; }
            T& get() { return value; }
            
            template<typename... Args>
            decltype(auto) operator()(Args&&...)
            {
                return value;
            }
        };
        
        // expr
        template<typename tag, typename... children_t>
        class expr
        {
        public:
            // typedefs
            typedef tag tag_type;
            typedef std::tuple<children_t...> children_type;
        
            // enums
            enum { Arity = sizeof...(children_t) };
    
        private:
            // members
            children_type children;
            
            // functions
            template<typename... Args, std::size_t... I>
            decltype(auto) operator_impl(std::index_sequence<I...>, Args&&... args)
            {
                // tricky part
                return tag_type()(std::forward<children_t>(std::get<I>(children))(std::forward<Args>(args)...)...);
            }
    
        public:
            // functions
            explicit expr(const children_t&... child)
                : children(child...)
            {}
        
            template<std::size_t I>
            decltype(auto) get()
            {
                return std::get<I>(children);
            }
    
            template<typename... Args>
            decltype(auto) operator()(Args&&... args)
            {
                typedef std::make_index_sequence<Arity> Seq;
                return operator_impl<Args...>(Seq(), std::forward<Args>(args)...);
            }
        };
        
        // helper functions
        template<typename T>
        decltype(auto) constant(const T& t)
        {
            return literal<T>(t);
        }
        
        template<typename T>
        decltype(auto) ref_constant(T& t)
        {
            return literal<T&>(t);
        }
        
        template<typename T>
        decltype(auto) cref_constant(const T& t)
        {
            return literal<const T&>(t);
        }
    }
}

#endif //TESTEXPRESSIONTEMPLATE_EXPRESSION_HPP
