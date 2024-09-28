/*
 *  Copyright 2019 uvm-fi
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef SRC_UVM_DSL_PRIV_IS_BASE_OF_H_
#define SRC_UVM_DSL_PRIV_IS_BASE_OF_H_

namespace uvm_dsl {namespace priv {

template<bool value_> struct bool_{
  typedef bool_<value_> type;
  static const bool value = value_;
};
typedef bool_<true> true_;
typedef bool_<false> false_;

namespace detail{
struct is_base_of_test_yes {char dummy[2];};
struct is_base_of_test_no  {char dummy;   };

template<class Base> is_base_of_test_yes
is_base_of_test(Base* ptr){return is_base_of_test_yes();}
template<class Base> is_base_of_test_no
is_base_of_test(void* ptr){return is_base_of_test_no();}

template<class Base, class Derived,
         bool value=sizeof(is_base_of_test<Base>((Derived*)0))
                    == sizeof(is_base_of_test_yes)>
struct is_base_of_impl : public false_ {};

template<class Base, class Derived>
struct is_base_of_impl<Base, Derived, true> : public true_ {};

}//namespace detail

template<class Base, class Derived>
struct is_base_of : public detail::is_base_of_impl<Base, Derived> { };

}} //namespace uvm_dsl::priv

#endif /* SRC_UVM_DSL_PRIV_IS_BASE_OF_H_ */
