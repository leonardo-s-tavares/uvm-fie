/*
 * uvm_call_generate.h
 *
 *  Created on: Jun 29, 2018
 *      Author: alexis
 */


//#define FI_LIB_NAME
//#define FI_LIB_NS

#ifndef FI_LIB_NS
  #define FI_LIB_NS
  #define FI_LIB_NS_COMMA
#else
  #define FI_LIB_NS_COMMA ,
#endif /* FI_LIB_NS*/

template<class T FI_LIB_NS_COMMA
                 #define FI_LIB_XL(n) class T##n
                 #include "uvm_call_generate_x.h"
> class FI_LIB_RT_N : public base_uvm_call<T> {
public:
  FI_LIB_RT_N(T (*f)(
                     #define FI_LIB_XL(n) T##n
                     #include "uvm_call_generate_x.h"
              ) FI_LIB_NS_COMMA
              #define FI_LIB_XL(n) uvm_value* a##n
              #include "uvm_call_generate_x.h"
  ) : f(f)  FI_LIB_NS_COMMA
      #define FI_LIB_XL(n) a##n(a##n)
      #include "uvm_call_generate_x.h"
  {}
  virtual ~FI_LIB_RT_N() {}

  void visit_args(uvm_visitor& visitor)  {
    #define FI_LIB_X(n) a##n->visit(visitor);
    #include "uvm_call_generate_x.h"
  }

  virtual void* get() {
    T* ptr = (T*)malloc(sizeof(T));
    #define FI_LIB_X(n) T##n *v##n = (T##n*)a##n->get();
    #include "uvm_call_generate_x.h"
    new (ptr) T((*f)(
      #define FI_LIB_XL(n) *v##n
      #include "uvm_call_generate_x.h"
    ));
    #define FI_LIB_X(n) free(v##n);
    #include "uvm_call_generate_x.h"
    return ptr;
  }

  virtual void set(void* value) {}
private:
  #define FI_LIB_X(n) uvm_value* a##n;
  #include "uvm_call_generate_x.h"

  T (*f)(
         #define FI_LIB_XL(n) T##n
         #include "uvm_call_generate_x.h"
  );
};

namespace uvm_dsl {

template<class T FI_LIB_NS_COMMA
         #define FI_LIB_XL(n) class N##n
         #include "uvm_call_generate_x.h"
> struct FI_LIB_DSL_N
: public node<FI_LIB_DSL_N <T FI_LIB_NS_COMMA
                            #define FI_LIB_XL(n) N##n
                            #include "uvm_call_generate_x.h"
                           >,
              T> {
T (*f)(
  #define FI_LIB_XL(n) typename N##n::type
  #include "uvm_call_generate_x.h"
);
#define FI_LIB_X(n) N##n node##n;
#include "uvm_call_generate_x.h"
  FI_LIB_DSL_N (T (*f)(
             #define FI_LIB_XL(n) typename N##n::type
             #include "uvm_call_generate_x.h"
           ) FI_LIB_NS_COMMA
           #define FI_LIB_XL(n) const N##n& node##n
           #include "uvm_call_generate_x.h"
) : f(f) FI_LIB_NS_COMMA
   #define FI_LIB_XL(n) node##n(node##n)
   #include "uvm_call_generate_x.h"
{}
static FI_LIB_DSL_N  create(T (*f)(
    #define FI_LIB_XL(n) typename N##n::type
    #include "uvm_call_generate_x.h"
  ) FI_LIB_NS_COMMA
  #define FI_LIB_XL(n) const N##n& node##n
  #include "uvm_call_generate_x.h"
) {
    return basic_call(f FI_LIB_NS_COMMA
                      #define FI_LIB_XL(n) node##n
                      #include "uvm_call_generate_x.h"
    );
  }
  uvm_value* build() const {
    return new FI_LIB_RT_N<T FI_LIB_NS_COMMA
                           #define FI_LIB_XL(n) typename N##n::type
                           #include "uvm_call_generate_x.h"
    >(f FI_LIB_NS_COMMA
      #define FI_LIB_XL(n) dynamic_cast<uvm_value*>(node##n.build())
      #include "uvm_call_generate_x.h"
    );
  }
};

template<class T FI_LIB_NS_COMMA
         #define FI_LIB_XL(n) class N##n
         #include "uvm_call_generate_x.h"
> FI_LIB_DSL_N<T FI_LIB_NS_COMMA
               #define FI_LIB_XL(n) N##n
               #include "uvm_call_generate_x.h"
>
call(T (*f)(
            #define FI_LIB_XL(n) typename N##n::type
            #include "uvm_call_generate_x.h"
     ) FI_LIB_NS_COMMA
     #define FI_LIB_XL(n) const N##n& node##n
     #include "uvm_call_generate_x.h"
) {
  return FI_LIB_DSL_N<T FI_LIB_NS_COMMA
                      #define FI_LIB_XL(n) N##n
                      #include "uvm_call_generate_x.h"
      >(f FI_LIB_NS_COMMA
        #define FI_LIB_XL(n) node##n
        #include "uvm_call_generate_x.h"
  );
}

} //namespace uvm_dsl

#undef FI_LIB_NS_COMMA

#undef FI_LIB_X
#undef FI_LIB_XL
#undef FI_LIB_RT_N
#undef FI_LIB_DSL_N
#undef FI_LIB_NS
