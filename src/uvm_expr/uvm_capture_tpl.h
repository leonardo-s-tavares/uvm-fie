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

#ifndef SRC_UVM_EXPR_UVM_CAPTURE_TPL_H_
#define SRC_UVM_EXPR_UVM_CAPTURE_TPL_H_

#include "uvm_capture.h"
#include <time.h>

template<class T> class uvm_capture_tpl : public uvm_capture {
public:
  uvm_capture_tpl(T& var) : uvm_capture(uvm_var_types::to_value<T>::value),
                            var(var) {}
  virtual void* get() {
    T* ptr = (T*)malloc(sizeof(T));
    *ptr = var;
    return ptr;
  }
  virtual void* peek() { return &var; }
  virtual void set(void* value) { var = *(T*)value; }
private:
  T& var;
};


template<class T> class uvm_unif_tpl : public uvm_capture {
public:
  uvm_unif_tpl(T min, T max) : uvm_capture(uvm_var_types::to_value<T>::value),
                            min(min), max(max) {}
  virtual void* get() {
    T* ptr = (T*)malloc(sizeof(T));
    *ptr = *((T*)peek());
    return ptr;
  }
  virtual void* peek() {
    value = ((double) rand() / (RAND_MAX)) * (max-min+1) + min;
    // value = ( (double) rand() / RAND_MAX) * ( max-min )+ min;
    return &value;
  }
  virtual void set(void* value) { }
private:
  T min, max;
  T value;
};


#endif /* SRC_UVM_EXPR_UVM_CAPTURE_TPL_H_ */
