//
// Created by lee on 17/8/15.
//

#ifndef TESTEXPRESSIONTEMPLATE_EXPRESSION_HPP
#define TESTEXPRESSIONTEMPLATE_EXPRESSION_HPP

#include <utility>
#include <tuple>
#include <type_traits>

#define DEFINE_MEMBER_BINARY_OP(op, tag_name, ClassType) \
template<typename U> \
auto operator op(const U& u) \
{ \
    return BinaryExpr<expression::tag_name##_tag, ClassType, typename detail::ExprTraits<U>::MemberType>(*this, u); \
}

#define DEFINE_MEMBER_BINARY_OPS(ClassType) \
DEFINE_MEMBER_BINARY_OP(=, assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(+=, plus_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(-=, minus_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(*=, multiplies_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(/=, divides_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(%=, modulus_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(&=, bitwise_and_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(|=, bitwise_or_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(^=, bitwise_xor_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(<<=, shift_left_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP(>>=, shift_right_assign, ClassType) \
DEFINE_MEMBER_BINARY_OP([], subscript_access, ClassType) \
auto operator ++() \
{ \
    return UnaryExpr<expression::pre_increment_tag, ClassType>(*this); \
} \
auto operator --() \
{ \
    return UnaryExpr<expression::pre_decrement_tag, ClassType>(*this); \
} \
auto operator ++(int) \
{ \
    return UnaryExpr<expression::post_increment_tag, ClassType>(*this); \
} \
auto operator --(int) \
{ \
    return UnaryExpr<expression::post_decrement_tag, ClassType>(*this); \
}

namespace molly
{
    // forward declarations
    namespace expression
    {
        template<typename T> struct terminal;
        template<typename T> struct reference;
    }
    
    namespace arg_names
    {
        template<std::size_t N> struct argument;
    }
    
    template<typename Tag, typename C0Type> struct UnaryExpr;
    template<typename Tag, typename C0Type, typename C1Type> struct BinaryExpr;
    template<typename Tag, typename C0Type, typename C1Type, typename C2Type> struct TernaryExpr;
    template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type> struct FourArgExpr;
    template<typename Tag, typename... CTypes> struct NArgExpr;
    
    namespace detail
    {
        // ExprTraits
        // simple trait for expressions
        template<typename T>
        struct ExprTraits
        {
            typedef std::conditional_t<std::is_copy_constructible<T>::value,
                expression::terminal<T>,
                expression::reference<T>> MemberType;
        };
        template<typename T, std::size_t N>
        struct ExprTraits<T[N]>
        {
            typedef expression::reference<const T[N]> MemberType;
        };
        template<typename T>
        struct ExprTraits<T &>
        {
            typedef expression::reference<T> MemberType;
        };
        template<typename T>
        struct ExprTraits<expression::terminal<T>>
        {
            typedef expression::terminal<T> MemberType;
        };
        template<typename T>
        struct ExprTraits<expression::reference<T>>
        {
            typedef expression::reference<T> MemberType;
        };
        template<std::size_t N>
        struct ExprTraits<arg_names::argument<N>>
        {
            typedef arg_names::argument<N> MemberType;
        };
        template<typename Tag, typename C0Type>
        struct ExprTraits<UnaryExpr<Tag, C0Type>>
        {
            typedef UnaryExpr<Tag, C0Type> MemberType;
        };
        template<typename Tag, typename C0Type, typename C1Type>
        struct ExprTraits<BinaryExpr<Tag, C0Type, C1Type>>
        {
            typedef BinaryExpr<Tag, C0Type, C1Type> MemberType;
        };
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type>
        struct ExprTraits<TernaryExpr<Tag, C0Type, C1Type, C2Type>>
        {
            typedef TernaryExpr<Tag, C0Type, C1Type, C2Type> MemberType;
        };
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type>
        struct ExprTraits<FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>>
        {
            typedef FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> MemberType;
        };
        template<typename Tag, typename... CTypes>
        struct ExprTraits<NArgExpr<Tag, CTypes...>>
        {
            typedef NArgExpr<Tag, CTypes...> MemberType;
        };
        
        template<typename... Ts>
        struct is_copy_constructible;
        
        template<typename T1, typename... Tr>
        struct is_copy_constructible<T1, Tr...>
        {
            enum {value = std::is_copy_constructible<T1>::value &&
                is_copy_constructible<Tr...>::value};
        };
        
        template<typename T1>
        struct is_copy_constructible<T1> : std::is_copy_constructible<T1>
        {};
    }
    namespace expression
    {
        // eval a tag
        template<typename Tag>
        struct eval;
        
        // tags
#define DEFINE_UNARY_EXPR_TAG(name, ret_type, signature, body) \
        struct name##_tag \
        { \
            enum {arity = 1}; \
        }; \
        template<> \
        struct eval<name##_tag> \
        { \
            template<typename T> \
            ret_type operator() signature \
            { \
                body; \
            } \
        }
#define DEFINE_BINARY_EXPR_TAG(name, ret_type, signature, body) \
        struct name##_tag \
        { \
            enum {arity = 2}; \
        }; \
        template<> \
        struct eval<name##_tag> \
        { \
            template<typename T, typename U> \
            ret_type operator() signature \
            { \
                body; \
            } \
        }
#define DEFINE_TERNARY_EXPR_TAG(name, ret_type, signature, body) \
        struct name##_tag \
        { \
            enum {arity = 3}; \
        }; \
        template<> \
        struct eval<name##_tag> \
        { \
            template<typename T, typename U, typename V> \
            ret_type operator() signature \
            { \
                body; \
            } \
        }
#define DEFINE_FOUR_ARG_EXPR_TAG(name, ret_type, signature, body) \
        struct name##_tag \
        { \
            enum {arity = 4}; \
        }; \
        template<> \
        struct eval<name##_tag> \
        { \
            template<typename T, typename U, typename V, typename W> \
            ret_type operator() signature \
            { \
                body; \
            } \
        }
#define DEFINE_NARG_EXPR_TAG(name, ret_type, signature, body) \
        struct name##_tag \
        { \
        }; \
        template<> \
        struct eval<name##_tag> \
        { \
            template<typename... Args> \
            ret_type operator() signature \
            { \
                body; \
            } \
        }

        // operators
        // unary
        DEFINE_UNARY_EXPR_TAG(pre_increment, auto, (T& t), return ++t);
        DEFINE_UNARY_EXPR_TAG(post_increment, auto, (T& t), return t++);
        DEFINE_UNARY_EXPR_TAG(pre_decrement, auto, (T& t), return --t);
        DEFINE_UNARY_EXPR_TAG(post_decrement, auto, (T& t), return t--);
        DEFINE_UNARY_EXPR_TAG(dereference, auto, (const T& t), return *t);
        DEFINE_UNARY_EXPR_TAG(address_of, auto, (const T& t), return &t);
        DEFINE_UNARY_EXPR_TAG(positive, auto, (const T& t), return +t);
        DEFINE_UNARY_EXPR_TAG(negative, auto, (const T& t), return -t);
        DEFINE_UNARY_EXPR_TAG(bitwise_not, auto, (const T& t), return ~t);
        DEFINE_UNARY_EXPR_TAG(logical_not, auto, (const T& t), return !t);
        
        // binary
        DEFINE_BINARY_EXPR_TAG(assign, auto&, (T& t, const U& u), return t = u);
        DEFINE_BINARY_EXPR_TAG(plus, auto, (const T& t, const U& u), return t + u); DEFINE_BINARY_EXPR_TAG(plus_assign, auto&, (T& t, const U& u), return t += u);
        DEFINE_BINARY_EXPR_TAG(minus, auto, (const T& t, const U& u), return t - u); DEFINE_BINARY_EXPR_TAG(minus_assign, auto&, (T& t, const U& u), return t -= u);
        DEFINE_BINARY_EXPR_TAG(multiplies, auto, (const T& t, const U& u), return t * u); DEFINE_BINARY_EXPR_TAG(multiplies_assign, auto&, (T& t, const U& u), return t *= u);
        DEFINE_BINARY_EXPR_TAG(divides, auto, (const T& t, const U& u), return t / u); DEFINE_BINARY_EXPR_TAG(divides_assign, auto&, (T& t, const U& u), return t /= u);
        DEFINE_BINARY_EXPR_TAG(modulus, auto, (const T& t, const U& u), return t % u); DEFINE_BINARY_EXPR_TAG(modulus_assign, auto&, (T& t, const U& u), return t %= u);
        DEFINE_BINARY_EXPR_TAG(bitwise_and, auto, (const T& t, const U& u), return t & u); DEFINE_BINARY_EXPR_TAG(bitwise_and_assign, auto&, (T& t, const U& u), return t &= u);
        DEFINE_BINARY_EXPR_TAG(bitwise_or, auto, (const T& t, const U& u), return t | u); DEFINE_BINARY_EXPR_TAG(bitwise_or_assign, auto&, (T& t, const U& u), return t |= u);
        DEFINE_BINARY_EXPR_TAG(bitwise_xor, auto, (const T& t, const U& u), return t ^ u); DEFINE_BINARY_EXPR_TAG(bitwise_xor_assign, auto&, (T& t, const U& u), return t ^= u);
        DEFINE_BINARY_EXPR_TAG(shift_left, auto&, (T& t, const U& u), return t << u); DEFINE_BINARY_EXPR_TAG(shift_left_assign, auto&, (T& t, const U& u), return t <<= u);
        DEFINE_BINARY_EXPR_TAG(shift_right, auto&, (T& t, U& u), return t >> u); DEFINE_BINARY_EXPR_TAG(shift_right_assign, auto&, (T& t, const U& u), return t >>= u);
        DEFINE_BINARY_EXPR_TAG(equal_to, auto, (const T& t, const U& u), return t == u);
        DEFINE_BINARY_EXPR_TAG(not_equal_to, auto, (const T& t, const U& u), return t != u);
        DEFINE_BINARY_EXPR_TAG(less, auto, (const T& t, const U& u), return t < u);
        DEFINE_BINARY_EXPR_TAG(greater, auto, (const T& t, const U& u), return t > u);
        DEFINE_BINARY_EXPR_TAG(less_equal_to, auto, (const T& t, const U& u), return t <= u);
        DEFINE_BINARY_EXPR_TAG(greater_equal_to, auto, (const T& t, const U& u), return t >= u);
        DEFINE_BINARY_EXPR_TAG(logical_and, auto, (const T& t, const U& u), return t && u);
        DEFINE_BINARY_EXPR_TAG(logical_or, auto, (const T& t, const U& u), return t || u);
        DEFINE_BINARY_EXPR_TAG(subscript_access, auto, (const T& t, const U& u), return t[u]);
        DEFINE_BINARY_EXPR_TAG(member_pointer_access, auto, (const T& t, const U& u), return t->*u);
        DEFINE_BINARY_EXPR_TAG(comma, const auto&, (const T& t, const U& u), return u);
        
        // ternary
        DEFINE_TERNARY_EXPR_TAG(ternary, auto, (const T& t, const U& u, const V& v), return t ? u : v);

#undef DEFINE_UNARY_EXPR_TAG
#undef DEFINE_BINARY_EXPR_TAG
#undef DEFINE_TERNARY_EXPR_TAG
#undef DEFINE_FOUR_ARG_EXPR_TAG
#undef DEFINE_NARG_EXPR_TAG
    }
    
    // expression class
    // unary, binary, ternary, and four-arg, and n-arg
    template<typename Tag, typename C0Type>
    class UnaryExpr
    {
#ifdef STATIC_ASSERT_ARITY
        static_assert(Tag::arity == 1);
#endif
    public:
        // typedefs
        typedef C0Type Child0Type;
        typedef Tag TagType;
        typedef UnaryExpr<Tag, C0Type> ClassType;
        
    private:
        // members
        Child0Type child0;
        
    public:
        // constructors
        explicit UnaryExpr(const Child0Type &c0)
            : child0(c0)
        {}
        
        template<typename... Args>
        auto operator()(const Args &... args)
        -> std::conditional_t<detail::is_copy_constructible<Args...>::value,
            decltype(expression::eval<TagType>()(child0(args...))),
            decltype(expression::eval<TagType>()(child0(args...)))&>
        {
            return expression::eval<TagType>()(child0(args...));
        }
        
        DEFINE_MEMBER_BINARY_OPS(ClassType);
    };
    
    template<typename Tag, typename C0Type, typename C1Type>
    class BinaryExpr
    {
#ifdef STATIC_ASSERT_ARITY
        static_assert(Tag::arity == 2);
#endif
    public:
        // typedefs
        typedef C0Type Child0Type;
        typedef C1Type Child1Type;
        typedef Tag TagType;
        typedef BinaryExpr<Tag, C0Type, C1Type> ClassType;
        
    private:
        // members
        Child0Type child0;
        Child1Type child1;
        
    public:
        // constructors
        BinaryExpr(const Child0Type &c0, const Child1Type &c1)
            : child0(c0), child1(c1)
        {}
        
        template<typename... Args>
        auto operator()(const Args &... args)
        -> std::conditional_t<detail::is_copy_constructible<Args...>::value,
            decltype(expression::eval<TagType>()(child0(args...), child1(args...))),
            decltype(expression::eval<TagType>()(child0(args...), child1(args...)))&>
        {
            return expression::eval<TagType>()(child0(args...),
                                               child1(args...));
        }
         
        DEFINE_MEMBER_BINARY_OPS(ClassType);
    };
    
    template<typename Tag, typename C0Type, typename C1Type, typename C2Type>
    class TernaryExpr
    {
#ifdef STATIC_ASSERT_ARITY
        static_assert(Tag::arity == 3);
#endif
    public:
        // typedefs
        typedef C0Type Child0Type;
        typedef C1Type Child1Type;
        typedef C2Type Child2Type;
        typedef Tag TagType;
        typedef TernaryExpr<Tag, C0Type, C1Type, C2Type> ClassType;
        
    private:
        // members
        Child0Type child0;
        Child1Type child1;
        Child2Type child2;
        
    public:
        // constructors
        TernaryExpr(const Child0Type &c0, const Child1Type &c1, const Child2Type &c2)
            : child0(c0), child1(c1), child2(c2)
        {}
        
        template<typename... Args>
        auto operator()(const Args &... args)
        -> std::conditional_t<detail::is_copy_constructible<Args...>::value,
            decltype(expression::eval<TagType>()(child0(args...), child1(args...), child2(args...))),
            decltype(expression::eval<TagType>()(child0(args...), child1(args...), child2(args...)))&>
        {
            return expression::eval<TagType>()(child0(args...),
                                               child1(args...),
                                               child2(args...));
        }
        
        DEFINE_MEMBER_BINARY_OPS(ClassType);
    };
    
    template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type>
    class FourArgExpr
    {
#ifdef STATIC_ASSERT_ARITY
        static_assert(Tag::arity == 4);
#endif
    public:
        // typedefs
        typedef C0Type Child0Type;
        typedef C1Type Child1Type;
        typedef C2Type Child2Type;
        typedef C3Type Child3Type;
        typedef Tag TagType;
        typedef FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> ClassType;
        
    private:
        // members
        Child0Type child0;
        Child1Type child1;
        Child2Type child2;
        Child3Type child3;
        
    public:
        // constructors
        FourArgExpr(const Child0Type &c0, const Child1Type &c1, const Child2Type &c2, const Child3Type &c3)
            : child0(c0), child1(c1), child2(c2), child3(c3)
        {}
        
        template<typename... Args>
        auto operator()(const Args &... args)
        -> std::conditional_t<detail::is_copy_constructible<Args...>::value,
            decltype(expression::eval<TagType>()(child0(args...), child1(args...), child2(args...), child3(args...))),
            decltype(expression::eval<TagType>()(child0(args...), child1(args...), child2(args...), child3(args...)))&>
        {
            return expression::eval<TagType>()(child0(args...),
                                               child1(args...),
                                               child2(args...),
                                               child3(args...));
        }
        
        DEFINE_MEMBER_BINARY_OPS(ClassType);
    };
    
    template<typename Tag, typename... CTypes>
    class NArgExpr
    {
#ifdef STATIC_ASSERT_ARITY
        static_assert(std::is_same<Tag, expression::function_tag>::value || Tag::arity == sizeof...(CTypes));
#endif
        // a bit tricky
    public:
        // typedefs
        typedef std::tuple<CTypes...> ChildTypes;
        typedef Tag TagType;
        typedef NArgExpr<Tag, CTypes...> ClassType;
        
    private:
        // members
        ChildTypes children;
        // constructors
        NArgExpr(const CTypes &... cs)
            : children(cs...)
        {}
        
    public:
        template<typename... Args>
        auto operator()(const Args &... args)
        {
            return operator_impl(std::make_index_sequence<std::tuple_size<ChildTypes>::value>(),
                                 args...);
        }
        
        DEFINE_MEMBER_BINARY_OPS(ClassType);
        
    private:
        template<typename... Args, std::size_t... I>
        auto operator_impl(std::index_sequence<I...>, const Args &... args)
        -> std::conditional_t<detail::is_copy_constructible<Args...>::value,
            decltype(expression::eval<TagType>()(std::get<I>(children)(args...)...)),
            decltype(expression::eval<TagType>()(std::get<I>(children)(args...)...))&>
        {
            // tricky part
            return expression::eval<TagType>()(
                std::get<I>(children)(args...)...
            );
        }
    };
    
    namespace arg_names
    {
        // argument
        // return n-th argument
        template<std::size_t N>
        struct argument
        {
            typedef argument<N> ClassType;
            
            template<typename Arg1, typename... Args>
            auto operator()(const Arg1 &, const Args &... args) const
            {
                return argument<N - 1>()(args...);
            }
    
            DEFINE_MEMBER_BINARY_OPS(ClassType);
        };
        template<>
        struct argument<1>
        {
            typedef argument<1> ClassType;
            
            template<typename Arg1, typename... Args>
            auto operator()(const Arg1 &arg1, const Args &...) const
            -> std::conditional_t<std::is_copy_constructible<Arg1>::value, Arg1, Arg1&>
            {
                return arg1;
            }
    
            DEFINE_MEMBER_BINARY_OPS(ClassType);
        };
        
        // convenient const variables
        const argument<1> arg1 = {};
        const argument<2> arg2 = {};
        const argument<3> arg3 = {};
        const argument<4> arg4 = {};
        const argument<5> arg5 = {};
    }
    
    namespace expression
    {
        // wrappers
        // terminal & reference
        template<typename T>
        struct terminal
        {
            // typedefs
            typedef T TerminalType;
            typedef terminal<T> ClassType;
            
            // ctors
            terminal(const T& t)
                : obj(t)
            {}
            
            template<typename... Args>
            T operator()(const Args &...) const
            {
                return obj;
            }
    
            DEFINE_MEMBER_BINARY_OPS(ClassType);
            
        private:
            // members
            T obj;
        };
        template<typename T>
        struct reference
        {
            // typedefs
            typedef T& TerminalType;
            typedef reference<T> ClassType;
            
            // ctors
            reference(T& t)
                : ref(t)
            {}
            
            template<typename... Args>
            T& operator()(const Args &...) const
            {
                return ref;
            }
    
            DEFINE_MEMBER_BINARY_OPS(ClassType);
            
        private:
            // members
            T& ref;
        };

        // utility maker functions
        template<typename T>
        expression::terminal<T> val(const T& t)
        {
            return expression::terminal<T>(t);
        }
        template<typename T>
        expression::terminal<T> val(const expression::reference<T>& t)
        {
            return expression::terminal<T>(t());
        }
        template<typename T, std::size_t N>
        expression::reference<const T[N]> val(T (&t)[N])
        {
            return expression::reference<const T[N]>(t);
        };
        template<typename T>
        expression::reference<T> ref(T& t)
        {
            return expression::reference<T>(t);
        }
        template<typename T>
        expression::reference<T> ref(expression::terminal<T>& t)
        {
            return expression::reference<T>(t());
        }
        template<typename T, std::size_t N>
        expression::reference<const T[N]> ref(T (&t)[N])
        {
            return expression::reference<const T[N]>(t);
        };
        template<typename T>
        expression::reference<const T> cref(const T& t)
        {
            return expression::reference<const T>(t);
        }
        template<typename T>
        expression::reference<const T> cref(const expression::terminal<T>& t)
        {
            return expression::reference<const T>(t());
        }
        template<typename T, std::size_t N>
        expression::reference<const T[N]> cref(T (&t)[N])
        {
            return expression::reference<const T[N]>(t);
        };
    }
    
    namespace operators
    {
        // operators
        // unary
#define DEFINE_UNARY_OP_REFONLY(op, tag_name) \
        template<typename T> \
        auto operator op(const expression::reference<T> & t) \
        { \
            return UnaryExpr<expression::tag_name##_tag, expression::reference<T>>(t); \
        } \
        template<size_t N> \
        auto operator op(const arg_names::argument<N> & t) \
        { \
            return UnaryExpr<expression::tag_name##_tag, arg_names::argument<N>>(t); \
        } \
        template<typename Tag, typename C0Type> \
        auto operator op(const UnaryExpr<Tag, C0Type> & t) \
        { \
            return UnaryExpr<expression::tag_name##_tag, UnaryExpr<Tag, C0Type>>(t); \
        } \
        template<typename Tag, typename C0Type, typename C1Type> \
        auto operator op(const BinaryExpr<Tag, C0Type, C1Type> & t) \
        { \
            return UnaryExpr<expression::tag_name##_tag, BinaryExpr<Tag, C0Type, C1Type>>(t); \
        } \
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type> \
        auto operator op(const TernaryExpr<Tag, C0Type, C1Type, C2Type> & t) \
        { \
            return UnaryExpr<expression::tag_name##_tag, TernaryExpr<Tag, C0Type, C1Type, C2Type>>(t); \
        } \
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type> \
        auto operator op(const FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> & t) \
        { \
            return UnaryExpr<expression::tag_name##_tag, FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>>(t); \
        } \
        template<typename Tag, typename... CTypes> \
        auto operator op(const NArgExpr<Tag, CTypes...> & t) \
        { \
            return UnaryExpr<expression::tag_name##_tag, NArgExpr<Tag, CTypes...>>(t); \
        }
#define DEFINE_UNARY_OP(op, tag_name) \
        template<typename T> \
        UnaryExpr<expression::tag_name##_tag, expression::terminal<T>> operator op(const expression::terminal<T> & t) \
        { \
            return UnaryExpr<expression::tag_name##_tag, expression::terminal<T>>(t); \
        } \
        DEFINE_UNARY_OP_REFONLY(op, tag_name)
        DEFINE_UNARY_OP(+, positive);
        DEFINE_UNARY_OP(-, negative);
        //DEFINE_UNARY_OP_REFONLY(++, pre_increment);
        //DEFINE_UNARY_OP_REFONLY(--, pre_decrement);
        DEFINE_UNARY_OP(*, dereference);
        DEFINE_UNARY_OP(&, address_of);
        DEFINE_UNARY_OP(~, bitwise_not);
        DEFINE_UNARY_OP(!, logical_not);
#undef DEFINE_UNARY_OP
#undef DEFINE_UNARY_OP_REFONLY
        
        // binary
#define DEFINE_BINARY_OP_REFONLY(op, tag_name) \
        template<typename T, typename U> \
        auto operator op(const expression::reference<T> & t, const U & u) \
        { \
            return BinaryExpr<expression::tag_name##_tag, expression::reference<T>, typename detail::ExprTraits<U>::MemberType>(t, u); \
        } \
        template<size_t N, typename U> \
        auto operator op(const arg_names::argument<N> & t, const U & u) \
        { \
            return BinaryExpr<expression::tag_name##_tag, const arg_names::argument<N>, typename detail::ExprTraits<U>::MemberType>(t, u); \
        } \
        template<typename Tag, typename C0Type, typename U> \
        auto operator op(const UnaryExpr<Tag, C0Type> & t, const U & u) \
        { \
            return BinaryExpr<expression::tag_name##_tag, UnaryExpr<Tag, C0Type>, typename detail::ExprTraits<U>::MemberType>(t, u); \
        } \
        template<typename Tag, typename C0Type, typename C1Type, typename U> \
        auto operator op(const BinaryExpr<Tag, C0Type, C1Type> & t, const U & u) \
        { \
            return BinaryExpr<expression::tag_name##_tag, BinaryExpr<Tag, C0Type, C1Type>, typename detail::ExprTraits<U>::MemberType>(t, u); \
        } \
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename U> \
        auto operator op(const TernaryExpr<Tag, C0Type, C1Type, C2Type> & t, const U & u) \
        { \
            return BinaryExpr<expression::tag_name##_tag, TernaryExpr<Tag, C0Type, C1Type, C2Type>, typename detail::ExprTraits<U>::MemberType>(t, u); \
        } \
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type, typename U> \
        auto operator op(const FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> & t, const U & u) \
        { \
            return BinaryExpr<expression::tag_name##_tag, FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>, typename detail::ExprTraits<U>::MemberType>(t, u); \
        } \
        template<typename Tag, typename... CTypes, typename U> \
        auto operator op(const NArgExpr<Tag, CTypes...> & t, const U & u) \
        { \
            return BinaryExpr<expression::tag_name##_tag, NArgExpr<Tag, CTypes...>, typename detail::ExprTraits<U>::MemberType>(t, u); \
        }
#define DEFINE_BINARY_OP(op, tag_name) \
        template<typename T, typename U> \
        auto operator op(const expression::terminal<T> & t, const U & u) \
        { \
            return BinaryExpr<expression::tag_name##_tag, expression::terminal<T>, typename detail::ExprTraits<U>::MemberType>(t, u); \
        } \
        template<typename T, typename U> \
        auto operator op(const U & u, const expression::terminal<T> & t) \
        { \
            return BinaryExpr<expression::tag_name##_tag, typename detail::ExprTraits<U>::MemberType, expression::terminal<T>>(u, t); \
        } \
        template<typename T, typename U> \
        auto operator op(const U & u, const expression::reference<T> & t) \
        { \
            return BinaryExpr<expression::tag_name##_tag, typename detail::ExprTraits<U>::MemberType, expression::reference<T>>(u, t); \
        } \
        template<size_t N, typename U> \
        auto operator op(const U & u, const arg_names::argument<N> & t) \
        { \
            return BinaryExpr<expression::tag_name##_tag, typename detail::ExprTraits<U>::MemberType, arg_names::argument<N>>(u, t); \
        } \
        template<typename Tag, typename C0Type, typename U> \
        auto operator op(const U & u, const UnaryExpr<Tag, C0Type> & t) \
        { \
            return BinaryExpr<expression::tag_name##_tag, typename detail::ExprTraits<U>::MemberType, UnaryExpr<Tag, C0Type>>(u, t); \
        } \
        template<typename Tag, typename C0Type, typename C1Type, typename U> \
        auto operator op(const U & u, const BinaryExpr<Tag, C0Type, C1Type> & t) \
        { \
            return BinaryExpr<expression::tag_name##_tag, typename detail::ExprTraits<U>::MemberType, BinaryExpr<Tag, C0Type, C1Type>>(u, t); \
        } \
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename U> \
        auto operator op(const U & u, const TernaryExpr<Tag, C0Type, C1Type, C2Type> & t) \
        { \
            return BinaryExpr<expression::tag_name##_tag, typename detail::ExprTraits<U>::MemberType, TernaryExpr<Tag, C0Type, C1Type, C2Type>>(u, t); \
        } \
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type, typename U> \
        auto operator op(const U & u, const FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> & t) \
        { \
            return BinaryExpr<expression::tag_name##_tag, typename detail::ExprTraits<U>::MemberType, FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>>(u, t); \
        } \
        template<typename Tag, typename... CTypes, typename U> \
        auto operator op(const U & u, const NArgExpr<Tag, CTypes...> & t) \
        { \
            return BinaryExpr<expression::tag_name##_tag, typename detail::ExprTraits<U>::MemberType, NArgExpr<Tag, CTypes...>>(u, t); \
        } \
        DEFINE_BINARY_OP_REFONLY(op, tag_name)
        //DEFINE_BINARY_OP_REFONLY(=, assign);
        DEFINE_BINARY_OP(+, plus); //DEFINE_BINARY_OP_REFONLY(+=, plus_assign);
        DEFINE_BINARY_OP(-, minus); //DEFINE_BINARY_OP_REFONLY(-=, minus_assign);
        DEFINE_BINARY_OP(*, multiplies); //DEFINE_BINARY_OP_REFONLY(*=, multiplies_assign);
        DEFINE_BINARY_OP(/, divides); //DEFINE_BINARY_OP_REFONLY(/=, divides_assign);
        DEFINE_BINARY_OP(%, modulus); //DEFINE_BINARY_OP_REFONLY(%=, modulus_assign);
        DEFINE_BINARY_OP(&, bitwise_and); //DEFINE_BINARY_OP_REFONLY(&=, bitwise_and_assign);
        DEFINE_BINARY_OP(|, bitwise_or); //DEFINE_BINARY_OP_REFONLY(|=, bitwise_or_assign);
        DEFINE_BINARY_OP(^, bitwise_xor); //DEFINE_BINARY_OP_REFONLY(^=, bitwise_xor_assign);
        //DEFINE_BINARY_OP(<<, shift_left); //DEFINE_BINARY_OP_REFONLY(<<=, shift_left_assign);
        //DEFINE_BINARY_OP(>>, shift_right); //DEFINE_BINARY_OP_REFONLY(>>=, shift_right_assign);
        DEFINE_BINARY_OP(==, equal_to);
        DEFINE_BINARY_OP(!=, not_equal_to);
        DEFINE_BINARY_OP(<, less);
        DEFINE_BINARY_OP(>, greater);
        DEFINE_BINARY_OP(<=, less_equal_to);
        DEFINE_BINARY_OP(>=, greater_equal_to);
        DEFINE_BINARY_OP(&&, logical_and);
        DEFINE_BINARY_OP(||, logical_or);
        //DEFINE_BINARY_OP([], subscript_access);
        DEFINE_BINARY_OP(->*, member_pointer_access);
        template<typename T, typename U>
        auto operator ,(const expression::reference<T> & t, const U & u)
        {
            return BinaryExpr<expression::comma_tag, expression::reference<T>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<size_t N, typename U>
        auto operator ,(const arg_names::argument<N> & t, const U & u)
        {
            return BinaryExpr<expression::comma_tag, const arg_names::argument<N>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename U>
        auto operator ,(const UnaryExpr<Tag, C0Type> & t, const U & u)
        {
            return BinaryExpr<expression::comma_tag, UnaryExpr<Tag, C0Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename U>
        auto operator ,(const BinaryExpr<Tag, C0Type, C1Type> & t, const U & u)
        {
            return BinaryExpr<expression::comma_tag, BinaryExpr<Tag, C0Type, C1Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename U>
        auto operator ,(const TernaryExpr<Tag, C0Type, C1Type, C2Type> & t, const U & u)
        {
            return BinaryExpr<expression::comma_tag, TernaryExpr<Tag, C0Type, C1Type, C2Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type, typename U>
        auto operator ,(const FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> & t, const U & u)
        {
            return BinaryExpr<expression::comma_tag, FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename... CTypes, typename U>
        auto operator ,(const NArgExpr<Tag, CTypes...> & t, const U & u)
        {
            return BinaryExpr<expression::comma_tag, NArgExpr<Tag, CTypes...>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename T, typename U>
        auto operator ,(const expression::terminal<T> & t, const U & u)
        {
            return BinaryExpr<expression::comma_tag, expression::terminal<T>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename T, typename U>
        auto operator ,(U & u, const expression::terminal<T> & t)
        {
            return BinaryExpr<expression::comma_tag, typename detail::ExprTraits<U>::MemberType, expression::terminal<T>>(u, t);
        }
        template<typename T, typename U>
        auto operator ,(U & u, const expression::reference<T> & t)
        {
            return BinaryExpr<expression::comma_tag, typename detail::ExprTraits<U>::MemberType, expression::reference<T>>(u, t);
        }
        template<size_t N, typename U>
        auto operator ,(U & u, const arg_names::argument<N> & t)
        {
            return BinaryExpr<expression::comma_tag, typename detail::ExprTraits<U>::MemberType, arg_names::argument<N>>(u, t);
        }
        template<typename Tag, typename C0Type, typename U>
        auto operator ,(U & u, const UnaryExpr<Tag, C0Type> & t)
        {
            return BinaryExpr<expression::comma_tag, typename detail::ExprTraits<U>::MemberType, UnaryExpr<Tag, C0Type>>(u, t);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename U>
        auto operator ,(U & u, const BinaryExpr<Tag, C0Type, C1Type> & t)
        {
            return BinaryExpr<expression::comma_tag, typename detail::ExprTraits<U>::MemberType, BinaryExpr<Tag, C0Type, C1Type>>(u, t);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename U>
        auto operator ,(U & u, const TernaryExpr<Tag, C0Type, C1Type, C2Type> & t)
        {
            return BinaryExpr<expression::comma_tag, typename detail::ExprTraits<U>::MemberType, TernaryExpr<Tag, C0Type, C1Type, C2Type>>(u, t);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type, typename U>
        auto operator ,(U & u, const FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> & t)
        {
            return BinaryExpr<expression::comma_tag, typename detail::ExprTraits<U>::MemberType, FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>>(u, t);
        }
        template<typename Tag, typename... CTypes, typename U>
        auto operator ,(U & u, const NArgExpr<Tag, CTypes...> & t)
        {
            return BinaryExpr<expression::comma_tag, typename detail::ExprTraits<U>::MemberType, NArgExpr<Tag, CTypes...>>(u, t);
        }
        template<typename T, typename U>
        auto operator <<(const expression::reference<T> & t, const U & u)
        {
            return BinaryExpr<expression::shift_left_tag, expression::reference<T>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<size_t N, typename U>
        auto operator <<(const arg_names::argument<N> & t, const U & u)
        {
            return BinaryExpr<expression::shift_left_tag, const arg_names::argument<N>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename U>
        auto operator <<(const UnaryExpr<Tag, C0Type> & t, const U & u)
        {
            return BinaryExpr<expression::shift_left_tag, UnaryExpr<Tag, C0Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename U>
        auto operator <<(const BinaryExpr<Tag, C0Type, C1Type> & t, const U & u)
        {
            return BinaryExpr<expression::shift_left_tag, BinaryExpr<Tag, C0Type, C1Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename U>
        auto operator <<(const TernaryExpr<Tag, C0Type, C1Type, C2Type> & t, const U & u)
        {
            return BinaryExpr<expression::shift_left_tag, TernaryExpr<Tag, C0Type, C1Type, C2Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type, typename U>
        auto operator <<(const FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> & t, const U & u)
        {
            return BinaryExpr<expression::shift_left_tag, FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename... CTypes, typename U>
        auto operator <<(const NArgExpr<Tag, CTypes...> & t, const U & u)
        {
            return BinaryExpr<expression::shift_left_tag, NArgExpr<Tag, CTypes...>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename T, typename U>
        auto operator <<(const expression::terminal<T> & t, const U & u)
        {
            return BinaryExpr<expression::shift_left_tag, expression::terminal<T>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename T, typename U>
        auto operator <<(U & u, const expression::terminal<T> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, expression::terminal<T>>(u, t);
        }
        template<typename T, typename U>
        auto operator <<(U & u, const expression::reference<T> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, expression::reference<T>>(u, t);
        }
        template<size_t N, typename U>
        auto operator <<(U & u, const arg_names::argument<N> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, arg_names::argument<N>>(u, t);
        }
        template<typename Tag, typename C0Type, typename U>
        auto operator <<(U & u, const UnaryExpr<Tag, C0Type> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, UnaryExpr<Tag, C0Type>>(u, t);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename U>
        auto operator <<(U & u, const BinaryExpr<Tag, C0Type, C1Type> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, BinaryExpr<Tag, C0Type, C1Type>>(u, t);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename U>
        auto operator <<(U & u, const TernaryExpr<Tag, C0Type, C1Type, C2Type> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, TernaryExpr<Tag, C0Type, C1Type, C2Type>>(u, t);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type, typename U>
        auto operator <<(U & u, const FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>>(u, t);
        }
        template<typename Tag, typename... CTypes, typename U>
        auto operator <<(U & u, const NArgExpr<Tag, CTypes...> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, NArgExpr<Tag, CTypes...>>(u, t);
        }
        template<typename T, typename U>
        auto operator >>(const expression::reference<T> & t, U & u)
        {
            return BinaryExpr<expression::shift_left_tag, expression::reference<T>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<size_t N, typename U>
        auto operator >>(const arg_names::argument<N> & t, U & u)
        {
            return BinaryExpr<expression::shift_left_tag, const arg_names::argument<N>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename U>
        auto operator >>(const UnaryExpr<Tag, C0Type> & t, U & u)
        {
            return BinaryExpr<expression::shift_left_tag, UnaryExpr<Tag, C0Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename U>
        auto operator >>(const BinaryExpr<Tag, C0Type, C1Type> & t, U & u)
        {
            return BinaryExpr<expression::shift_left_tag, BinaryExpr<Tag, C0Type, C1Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename U>
        auto operator >>(const TernaryExpr<Tag, C0Type, C1Type, C2Type> & t, U & u)
        {
            return BinaryExpr<expression::shift_left_tag, TernaryExpr<Tag, C0Type, C1Type, C2Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type, typename U>
        auto operator >>(const FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> & t, U & u)
        {
            return BinaryExpr<expression::shift_left_tag, FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename Tag, typename... CTypes, typename U>
        auto operator >>(const NArgExpr<Tag, CTypes...> & t, U & u)
        {
            return BinaryExpr<expression::shift_left_tag, NArgExpr<Tag, CTypes...>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename T, typename U>
        auto operator >>(const expression::terminal<T> & t, U & u)
        {
            return BinaryExpr<expression::shift_left_tag, expression::terminal<T>, typename detail::ExprTraits<U>::MemberType>(t, u);
        }
        template<typename T, typename U>
        auto operator >>(U & u, const expression::terminal<T> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, expression::terminal<T>>(u, t);
        }
        template<typename T, typename U>
        auto operator >>(U & u, const expression::reference<T> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, expression::reference<T>>(u, t);
        }
        template<size_t N, typename U>
        auto operator >>(U & u, const arg_names::argument<N> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, arg_names::argument<N>>(u, t);
        }
        template<typename Tag, typename C0Type, typename U>
        auto operator >>(U & u, const UnaryExpr<Tag, C0Type> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, UnaryExpr<Tag, C0Type>>(u, t);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename U>
        auto operator >>(U & u, const BinaryExpr<Tag, C0Type, C1Type> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, BinaryExpr<Tag, C0Type, C1Type>>(u, t);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename U>
        auto operator >>(U & u, const TernaryExpr<Tag, C0Type, C1Type, C2Type> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, TernaryExpr<Tag, C0Type, C1Type, C2Type>>(u, t);
        }
        template<typename Tag, typename C0Type, typename C1Type, typename C2Type, typename C3Type, typename U>
        auto operator >>(U & u, const FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, FourArgExpr<Tag, C0Type, C1Type, C2Type, C3Type>>(u, t);
        }
        template<typename Tag, typename... CTypes, typename U>
        auto operator >>(U & u, const NArgExpr<Tag, CTypes...> & t)
        {
            return BinaryExpr<expression::shift_left_tag, typename detail::ExprTraits<U>::MemberType, NArgExpr<Tag, CTypes...>>(u, t);
        }
#undef DEFINE_BINARY_OP
#undef DEFINE_BINARY_OP_REFONLY
        
        // ternary
        // because we can't overload operator?:, just use if_else function
        template<typename C0Type, typename C1Type, typename C2Type>
        auto if_else(const C0Type& cond, const C1Type& if_true, const C2Type& if_false)
        {
            return TernaryExpr<expression::ternary_tag,
                typename detail::ExprTraits<C0Type>::MemberType,
                typename detail::ExprTraits<C1Type>::MemberType,
                typename detail::ExprTraits<C2Type>::MemberType>(cond, if_true, if_false);
        };
    }
}

#undef DEFINE_MEMBER_BINARY_OP
#undef DEFINE_MEMBER_BINARY_OPS

#endif //TESTEXPRESSIONTEMPLATE_EXPRESSION_HPP
