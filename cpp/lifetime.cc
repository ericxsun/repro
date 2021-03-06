// Goal: Empirically determine lifetimes
// Purpose: Too lazy to fully parse:
// @ref http://en.cppreference.com/w/cpp/language/lifetime

#include <iostream>
#include <utility>

using std::cout;
using std::endl;

#define EVAL(x) \
    std::cout << ">>> " #x ";" << std::endl; \
    x; \
    cout << std::endl
#define EVAL_SCOPED(x) \
    std::cout << ">>> scope { " #x " ; }" << std::endl; \
    { \
        x; \
        std::cout << "   <<< [ exiting scope ]" << std::endl; \
    } \
    std::cout << std::endl

template <int T>
class Lifetime {
public:
    Lifetime() {
        cout << T << ": ctor ()" << endl;
    }
    Lifetime(const Lifetime&) {
        cout << T << ": copy ctor (const lvalue)" << endl;
    }
    Lifetime(Lifetime&& other) {
        cout << T << ": copy ctor (rvalue)" << endl;
        other.moved = true;
    }
    template <int U>
    Lifetime(const Lifetime<U>&) {
        cout
             << T << ": ctor (const Lifetime<" << U << ">&)" << endl;
    }
    template <int U>
    Lifetime(Lifetime<U>&& other) {
        cout
             << T << ": ctor (Lifetime<" << U << ">&&)" << endl;
        other.moved = true;
    }
    ~Lifetime() {
        cout << T << ": dtor";
        if (moved)
            cout << " <-- was moved";
        cout << endl;
    }

    bool moved {false};
protected:
    using Base = Lifetime<T>;
};

void func_in_const_lvalue(const Lifetime<3>&) {
    cout << "func_in_const_lvalue" << endl;
}

Lifetime<4> func_out_value() {
    cout << "func_out_value" << endl;
    return Lifetime<4>();
}

const Lifetime<4>& func_out_const_lvalue() {
    return Lifetime<4>(); // Warning shown
}

Lifetime<4>&& func_out_rvalue() {
    return Lifetime<4>(); // Warning shown
}

const Lifetime<4>& func_thru_const_lvalue(const Lifetime<4>& in) {
    cout << "func_thru_const_lvalue" << endl;
    return in;
}
// // Invalid
// Lifetime<4>&& func_thru_rvalue(Lifetime<4>&& in) {
//     cout << "func_thru_rvalue" << endl;
//     return in;
// }

auto func_thru_auto(const Lifetime<5>& in) {
    return in;
}

void section(const char* name) {
    cout << endl << "--- " << name << " ---" << endl << endl;
}

int main() {
    section("Standard");
    EVAL(Lifetime<1> obj1{}; Lifetime<2> obj2{} );
    EVAL_SCOPED( Lifetime<1>(); Lifetime<2>() );
    EVAL_SCOPED( Lifetime<1> tmp1{}; Lifetime<2> tmp2{} );
    EVAL_SCOPED( Lifetime<2> copy = obj2 );
    EVAL_SCOPED( Lifetime<2> copy = obj1 );
    EVAL_SCOPED( Lifetime<2> copy = Lifetime<1>() );

    section("In: const T&");
    EVAL( func_in_const_lvalue(Lifetime<3>()) );
    
    section("Out: T");
    EVAL( func_out_value() );
    EVAL( func_in_const_lvalue(func_out_value()) );
    EVAL_SCOPED( const Lifetime<4>& ref = func_out_value() );
    EVAL_SCOPED( Lifetime<4>&& ref = func_out_value() );
    EVAL_SCOPED( const Lifetime<4>& ref = func_thru_const_lvalue(func_out_value()); cout << &ref << endl; ); // No warning???
    cout << "  ^ loses lifetime, no warning???" << endl;

    section("Out: const T&");
    EVAL( func_out_const_lvalue() );
    EVAL_SCOPED( const Lifetime<4>& ref = func_out_const_lvalue() );

    section("Out: T&&");
    EVAL( func_out_rvalue() );
    EVAL_SCOPED( Lifetime<4>&& ref = func_out_rvalue() );
    EVAL_SCOPED( func_in_const_lvalue(func_out_rvalue()) );
    cout << "  ^ undefined behavior, was warned" << endl;

    section("Thru: const T&");
    EVAL( func_thru_const_lvalue(Lifetime<3>()) );
    EVAL_SCOPED( const auto& ref = func_thru_const_lvalue(Lifetime<3>()) );

    // section("Thru: T&&");
    // EVAL( func_thru_rvalue(Lifetime<4>()) );
    // EVAL_SCOPED( const auto& ref = func_thru_rvalue(Lifetime<4>()) );

    section("Thru: auto");
    EVAL( func_thru_auto(Lifetime<5>()) );
    EVAL_SCOPED( const auto& ref = func_thru_auto(Lifetime<5>()) );

    return 0;
}
