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

#ifndef UVM_VAR_TYPES_H_
#define UVM_VAR_TYPES_H_

#include <systemc>

namespace uvm_var_types{
  enum type{
    INT, LONG, LONGLONG, SHORT, CHAR, BOOL,
    UINT, ULONG, ULONGLONG, USHORT, UCHAR,
    FLOAT, DOUBLE,
    SIGNAL, SC_IN, SC_OUT,
    OTHER,
    _SC_INT_BASE = 0x8100,
    _SC_INT_END = 0x8141,
    _SC_UINT_BASE = 0x8200,
    _SC_UINT_END = 0x8241
  };

  template<class T> struct to_value {static const type value = OTHER;};
  template<> struct to_value<int>   {static const type value = INT;  };
  template<> struct to_value<unsigned int>   {static const type value = UINT;  };
  template<> struct to_value<char>   {static const type value = CHAR;  };
  template<> struct to_value<unsigned char>   {static const type value = UCHAR;  };
  template<> struct to_value<short>   {static const type value = SHORT;  };
  template<> struct to_value<unsigned short>   {static const type value = USHORT;  };
  template<> struct to_value<long>   {static const type value = LONG;  };
  template<> struct to_value<unsigned long>   {static const type value = ULONG;  };
  template<> struct to_value<long long>   {static const type value = LONGLONG;  };
  template<> struct to_value<unsigned long long>   {static const type value = ULONGLONG;  };
  template<int n> struct to_value<sc_dt::sc_int<n> > {
    static const type value = (type)(_SC_INT_BASE+n);
  };
  template<> struct to_value<float> {static const type value = FLOAT;};
  template<> struct to_value<double> {static const type value = DOUBLE;};
  template<> struct to_value<bool>  {static const type value = BOOL; };
  template<class T> struct to_value<sc_core::sc_in<T>* >  {
    static const type value = SC_IN;
  };

  template<class T> struct to_value<sc_core::sc_out<T>* >  {
    static const type value = SC_OUT;
  };

  template<class T> struct to_value<sc_core::sc_signal<T>* >  {
    static const type value = SIGNAL;
  };

  template<type value> struct from_value {
    class UNKNOWN_from_value;
    typedef UNKNOWN_from_value******** type;
  };
  template<> struct from_value<INT>   {typedef int type;};
  template<> struct from_value<UINT>   {typedef unsigned int type;};
  template<> struct from_value<CHAR>   {typedef char type;};
  template<> struct from_value<UCHAR>   {typedef unsigned char type;};
  template<> struct from_value<SHORT>   {typedef short type;};
  template<> struct from_value<USHORT>   {typedef unsigned short type;};
  template<> struct from_value<LONG>   {typedef long type;};
  template<> struct from_value<ULONG>   {typedef unsigned long type;};
  template<> struct from_value<LONGLONG>   {typedef long long type;};
  template<> struct from_value<ULONGLONG>   {typedef unsigned long long type;};
  template<> struct from_value<FLOAT>   {typedef float type;};
  template<> struct from_value<DOUBLE>   {typedef double type;};
  template<> struct from_value<BOOL>   {typedef bool type;};
  template<> struct from_value<static_cast<type>(_SC_INT_BASE+(0x1<<1))>   {typedef sc_dt::sc_int< 2> type;};
  template<> struct from_value<static_cast<type>(_SC_INT_BASE+(0x1<<2))>   {typedef sc_dt::sc_int< 4> type;};
  template<> struct from_value<static_cast<type>(_SC_INT_BASE+(0x1<<3))>   {typedef sc_dt::sc_int< 8> type;};
  template<> struct from_value<static_cast<type>(_SC_INT_BASE+(0x1<<4))>   {typedef sc_dt::sc_int<16> type;};
  template<> struct from_value<static_cast<type>(_SC_INT_BASE+(0x1<<5))>   {typedef sc_dt::sc_int<32> type;};
  template<> struct from_value<static_cast<type>(_SC_INT_BASE+(0x1<<6))>   {typedef sc_dt::sc_int<64> type;};
  template<> struct from_value<static_cast<type>(_SC_UINT_BASE+(0x1<<1))>   {typedef sc_dt::sc_int< 2> type;};
  template<> struct from_value<static_cast<type>(_SC_UINT_BASE+(0x1<<2))>   {typedef sc_dt::sc_int< 4> type;};
  template<> struct from_value<static_cast<type>(_SC_UINT_BASE+(0x1<<3))>   {typedef sc_dt::sc_int< 8> type;};
  template<> struct from_value<static_cast<type>(_SC_UINT_BASE+(0x1<<4))>   {typedef sc_dt::sc_int<16> type;};
  template<> struct from_value<static_cast<type>(_SC_UINT_BASE+(0x1<<5))>   {typedef sc_dt::sc_int<32> type;};
  template<> struct from_value<static_cast<type>(_SC_UINT_BASE+(0x1<<6))>   {typedef sc_dt::sc_int<64> type;};

  template<class T> struct assignment_type {
    typedef T type;
  };
  template<class T> struct assignment_type<sc_core::sc_in<T>* >{
    typedef T type;
  };
  template<class T> struct assignment_type<sc_core::sc_out<T>* >{
    typedef T type;
  };
  template<class T> struct assignment_type<sc_core::sc_signal<T>* >{
    typedef T type;
  };
}

#endif
