//
// Created by lee on 17/9/21.
//

#ifndef TESTEXPRESSIONTEMPLATE_EXPRESSION_HPP
#define TESTEXPRESSIONTEMPLATE_EXPRESSION_HPP

#include <tuple>
#include <utility>
#include <cstddef>
#include <type_traits>

#define DEF_MEMBER_OP(class_type, tagname, op) \
    template<typename U> \
    decltype(auto) operator op(U&& t) const \
    { \
        return expressions::expr<tags::tagname##_tag, class_type, traits::child_type<U>>(*this, t);\
    }
#define DEF_MEMBER_OPS(class_type) \
    DEF_MEMBER_OP(class_type, assign, =) \
    DEF_MEMBER_OP(class_type, subscript, []) \
    template<typename... U> \
    decltype(auto) operator ()(U&&... t) const \
    { \
        return expressions::expr<tags::function_tag, class_type, traits::child_type<U>...>(*this, std::forward<U>(t)...);\
    }

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
        
        struct function_tag
        {
            template<typename T, typename... U>
            decltype(auto) operator()(T&& t, U&&... u)
            {
                return std::forward<T>(t)(std::forward<U>(u)...);
            }
        };
    }
    
    namespace expressions
    {
        template<typename T> class literal;
        template<typename tag, typename... children_t> class expr;
    }
    
    namespace arg_names
    {
        template<std::size_t I> class argument;
    }
    
    namespace traits
    {
        // is_expr
        template<typename T> struct is_expr : std::false_type {};
        template<typename T> struct is_expr<expressions::literal<T>> : std::true_type {};
        template<std::size_t I> struct is_expr<arg_names::argument<I>> : std::true_type {};
        template<typename tag, typename... children_t>
        struct is_expr<expressions::expr<tag, children_t...>> : std::true_type {};
        
        template<typename T>
        static constexpr bool is_expr_v = is_expr<T>::value;
        
        template<typename T>
        using child_type = std::conditional_t<is_expr_v<T>, T, expressions::literal<T>>;
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
            
            DEF_MEMBER_OPS(literal<T>)
        };
        
        // expr
        template<typename tag, typename... children_t>
        class expr
        {
        public:
            // typedefs
            typedef tag tag_type;
            typedef std::tuple<children_t...> children_type;
            typedef expr<tag, children_t...> class_type;
        
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
            
            DEF_MEMBER_OPS(class_type)
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
            template<typename Arg1, typename... Args>
            decltype(auto) operator()(Arg1&&, Args&&... args)
            {
                return argument<I - 1>()(std::forward<Args>(args)...);
            }
            
            DEF_MEMBER_OPS(argument<I>)
        };
        
        template<>
        class argument<1>
        {
        public:
            // typedefs
            static constexpr std::size_t index = 1;
            
            // functions
            template<typename Arg1, typename... Args>
            decltype(auto) operator()(Arg1&& arg1, Args&&...)
            {
                return std::forward<Arg1>(arg1);
            }

            DEF_MEMBER_OPS(argument<1>)
        };
        
        // names
        const argument<1> arg1 = {};
        const argument<2> arg2 = {};
        const argument<3> arg3 = {};
        const argument<4> arg4 = {};
        const argument<5> arg5 = {};
    }
    
    namespace operators
    {
#define DEF_UNARY_OP(tagname, op) \
        template<typename T> \
        decltype(auto) operator op(const expressions::literal<T>& t) \
        { \
            return expressions::expr<tags::tagname##_tag, expressions::literal<T>>(t); \
        } \
        template<std::size_t I> \
        decltype(auto) operator op(const arg_names::argument<I>& t) \
        { \
            return expressions::expr<tags::tagname##_tag, arg_names::argument<I>>(t); \
        } \
        template<typename tag, typename... children_t> \
        decltype(auto) operator op(const expressions::expr<tag, children_t...>& c) \
        { \
            return expressions::expr<tags::tagname##_tag, expressions::expr<tag, children_t...>>(c); \
        }
        
        DEF_UNARY_OP(unary_plus, +);
        DEF_UNARY_OP(unary_minus, -);
        DEF_UNARY_OP(dereference, *);
        DEF_UNARY_OP(addressof, &);
        DEF_UNARY_OP(pre_increment, ++);
        DEF_UNARY_OP(pre_decrement, --);
        template<typename T>
        decltype(auto) operator ++(const expressions::literal<T>& t, int)
        {
            return expressions::expr<tags::post_increment_tag, expressions::literal<T>>(t);
        }
        template<std::size_t I>
        decltype(auto) operator ++(const arg_names::argument<I>& t, int)
        {
            return expressions::expr<tags::post_increment_tag, arg_names::argument<I>>(t);
        }
        template<typename tag, typename... children_t>
        decltype(auto) operator ++(const expressions::expr<tag, children_t...>& c, int)
        {
            return expressions::expr<tags::post_increment_tag, expressions::expr<tag, children_t...>>(c);
        }
        template<typename T>
        decltype(auto) operator --(const expressions::literal<T>& t, int)
        {
            return expressions::expr<tags::post_decrement_tag, expressions::literal<T>>(t);
        }
        template<std::size_t I>
        decltype(auto) operator --(const arg_names::argument<I>& t, int)
        {
            return expressions::expr<tags::post_decrement_tag, arg_names::argument<I>>(t);
        }
        template<typename tag, typename... children_t>
        decltype(auto) operator --(const expressions::expr<tag, children_t...>& c, int)
        {
            return expressions::expr<tags::post_decrement_tag, expressions::expr<tag, children_t...>>(c);
        }
        DEF_UNARY_OP(bitwise_not, ~);
        DEF_UNARY_OP(logical_not, !);
        
#undef DEF_UNARY_OP

#define DEF_BINARY_OP(tagname, op) \
        template<typename T, typename U> \
        decltype(auto) operator op(const expressions::literal<T>& t, U&& u) \
        { \
            return expressions::expr<tags::tagname##_tag, expressions::literal<T>, traits::child_type<U>>(t, u); \
        } \
        template<std::size_t I, typename U> \
        decltype(auto) operator op(const arg_names::argument<I>& t, U&& u) \
        { \
            return expressions::expr<tags::tagname##_tag, arg_names::argument<I>, traits::child_type<U>>(t, u); \
        } \
        template<typename tag, typename... child_type, typename U> \
        decltype(auto) operator op(const expressions::expr<tag, child_type...>& t, U&& u) \
        { \
            return expressions::expr<tags::tagname##_tag, expressions::expr<tag, child_type...>, traits::child_type<U>>(t, u); \
        } \
        template<typename T, typename U, typename = std::enable_if_t<!traits::is_expr_v<U>>> \
        decltype(auto) operator op(U&& t, const expressions::literal<T>& u) \
        { \
            return expressions::expr<tags::tagname##_tag, traits::child_type<U>, expressions::literal<T>>(t, u); \
        } \
        template<std::size_t I, typename U, typename = std::enable_if_t<!traits::is_expr_v<U>>> \
        decltype(auto) operator op(U&& t, const arg_names::argument<I>& u) \
        { \
            return expressions::expr<tags::tagname##_tag, traits::child_type<U>, arg_names::argument<I>>(t, u); \
        } \
        template<typename tag, typename... child_type, typename U, typename = std::enable_if_t<!traits::is_expr_v<U>>> \
        decltype(auto) operator op(U&& t, const expressions::expr<tag, child_type...>& u) \
        { \
            return expressions::expr<tags::tagname##_tag, traits::child_type<U>, expressions::expr<tag, child_type...>>(t, u); \
        }
        
        DEF_BINARY_OP(binary_plus, +);
        DEF_BINARY_OP(binary_minus, -);
        DEF_BINARY_OP(multiplies, *);
        DEF_BINARY_OP(divides, /);
        DEF_BINARY_OP(modulus, %);
        DEF_BINARY_OP(bitwise_and, &);
        DEF_BINARY_OP(bitwise_or, |);
        DEF_BINARY_OP(bitwise_xor, ^);
        DEF_BINARY_OP(shift_left, <<);
        DEF_BINARY_OP(shift_right, >>);
        DEF_BINARY_OP(plus_assign, +=);
        DEF_BINARY_OP(minus_assign, -=);
        DEF_BINARY_OP(multiplies_assign, *=);
        DEF_BINARY_OP(divides_assign, /=);
        DEF_BINARY_OP(modulus_assign, %=);
        DEF_BINARY_OP(bitwise_and_assign, &=);
        DEF_BINARY_OP(bitwise_or_assign, |=);
        DEF_BINARY_OP(bitwise_xor_assign, ^=);
        DEF_BINARY_OP(shift_left_assign, <<=);
        DEF_BINARY_OP(shift_right_assign, >>=);
        DEF_BINARY_OP(logical_and, &&);
        DEF_BINARY_OP(logical_or, ||);
        DEF_BINARY_OP(less, <);
        DEF_BINARY_OP(less_equal, <=);
        DEF_BINARY_OP(greater, >);
        DEF_BINARY_OP(greater_equal, >=);
        DEF_BINARY_OP(equal, ==);
        DEF_BINARY_OP(not_equal, !=);
        DEF_BINARY_OP(member_pointer_access, ->*);
        
#undef DEF_BINARY_OP
    }
}

#undef DEF_MEMBER_OP
#undef DEF_MEMBER_OPS

#endif //TESTEXPRESSIONTEMPLATE_EXPRESSION_HPP
