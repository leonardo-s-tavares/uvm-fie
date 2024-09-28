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

#ifndef SRC_UVM_DSL_UVM_DSL_OPERATORS_H_
#define SRC_UVM_DSL_UVM_DSL_OPERATORS_H_

#include "uvm_dsl_node.h"
#include "uvm_dsl_constants.h"
#include "../uvm_expr/uvm_ops.h"
#include "../uvm_expr/uvm_var_types.h"

namespace uvm_dsl {

template<class Built, class Left>
struct  built_result_type{typedef Left type;};


template<class Built, class LExpr, class RExpr,
         class LeftType, class RightType>
struct bin_op;

template<class Built, class LExpr, class RExpr>
struct bin_op<Built, LExpr, RExpr,
              typename LExpr::type,
              typename LExpr::type >
  : public node<bin_op<Built, LExpr, RExpr,
                       typename LExpr::type,
                       typename LExpr::type >,
                typename built_result_type<Built, typename LExpr::type>::type > {
  typedef typename built_result_type<Built, typename LExpr::type>::type type;
  typedef Built built_type;

  LExpr left;
  RExpr right;

  bin_op(LExpr left, RExpr right) : left(left), right(right) {}
  ::uvm_expr* build() const { return new Built(left.build(), right.build()); }
};

#define UVM_DSL_BIN_OP(Operator, BuiltType) \
template<class LExpr, class RExpr> \
bin_op<BuiltType, LExpr, RExpr, \
       typename LExpr::type, typename RExpr::type> \
Operator(const LExpr& left, \
          const RExpr& right) { \
  return bin_op<BuiltType, LExpr, RExpr, \
                typename LExpr::type, typename RExpr::type>(left, right); \
} \
template<class RExpr> \
bin_op<BuiltType, typename wrap_constant<typename RExpr::type>::type, RExpr, \
       typename RExpr::type, typename RExpr::type> \
Operator(const typename RExpr::type& left, \
          const RExpr& right) { \
  typedef typename RExpr::type r_tp; \
  return bin_op<BuiltType, typename wrap_constant<r_tp>::type, RExpr, \
                r_tp, r_tp>(wrap_constant<r_tp>::type::create(left), right); \
} \
template<class LExpr> \
bin_op<BuiltType, LExpr, typename wrap_constant<typename LExpr::type>::type, \
       typename LExpr::type, typename LExpr::type> \
Operator(const LExpr& left, \
          const typename LExpr::type& right) { \
  typedef typename LExpr::type l_tp; \
  return bin_op<BuiltType, LExpr, typename wrap_constant<l_tp>::type, \
                l_tp, l_tp>(left, wrap_constant<l_tp>::type::create(right)); \
}

#define UVM_DSL_BIN_L_OP(Operator, BuiltType) \
template<class Left> \
struct  built_result_type<BuiltType, Left>{typedef bool type;}; \
UVM_DSL_BIN_OP(Operator, BuiltType)


// define operators overloads
UVM_DSL_BIN_L_OP(operator>, uvm_gt)
UVM_DSL_BIN_L_OP(operator==, uvm_eq)
UVM_DSL_BIN_L_OP(operator!=, uvm_neq)
UVM_DSL_BIN_L_OP(operator>=, uvm_gte)
UVM_DSL_BIN_L_OP(operator<, uvm_lt)
UVM_DSL_BIN_L_OP(operator<=, uvm_lte)
UVM_DSL_BIN_L_OP(operator&&, uvm_and)
UVM_DSL_BIN_L_OP(operator||, uvm_or)
UVM_DSL_BIN_OP(operator&, uvm_band)
UVM_DSL_BIN_OP(operator|, uvm_bor)
UVM_DSL_BIN_OP(operator^, uvm_xor)
UVM_DSL_BIN_OP(operator+, uvm_plus)
UVM_DSL_BIN_OP(operator-, uvm_minus)
UVM_DSL_BIN_OP(operator/, uvm_divide)
UVM_DSL_BIN_OP(operator*, uvm_mult)

} // namespace uvm_dsl

#endif /* SRC_UVM_DSL_UVM_DSL_OPERATORS_H_ */
