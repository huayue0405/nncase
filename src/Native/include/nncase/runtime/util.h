/* Copyright 2019-2021 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include "../tensor.h"
#include "allocator.h"
#include "buffer.h"
#include "error.h"
#include "host_buffer.h"
#include "simple_types.h"
#include <nncase/api.h>
#include <nncase/runtime/runtime_op_utility.h>

namespace nncase::runtime {
inline bool is_scalar(tensor t) noexcept { return t->shape().empty(); }

inline result<bool> cmp_dt_impl(datatype_t lhs, datatype_t rhs) {
    try_var(l, to_typecode(lhs));
    try_var(r, to_typecode(rhs));
    return ok(l == r);
}

inline bool cmp_dt(datatype_t lhs, datatype_t rhs) {
    auto result = cmp_dt_impl(lhs, rhs);
    return result.is_ok() && result.unwrap();
}

template <typename T> inline bool cmp_type(datatype_t dt) {
    return cmp_dt(datatype_t::from_type<T>(), dt);
}

template <typename T> inline result<bool> type_only_check(tensor input) {
    return cmp_dt_impl(input->dtype(), datatype_t::from_type<T>());
}

inline result<bool> float_only_check(tensor input) {
    return cmp_dt_impl(input->dtype(), datatype_t::float32);
}

template <typename F>
inline result<void> tuple_for_each_with_i(tuple inputs, F &&f) {
    for (int i = 0; i < inputs->fields().size(); ++i) {
        try_(f(inputs->fields()[i], i));
    }
    return ok();
}

// todo:not process nest tuple
template <typename T, bool IsResult, typename F>
inline result<std::vector<T>> get_from_tuple_with_result(tuple inputs, F &&f) {
    std::vector<T> data(inputs->fields().size());
    for (int i = 0; i < inputs->fields().size(); ++i) {
        try_var(input, inputs->fields()[i].as<tensor>());
        if constexpr (IsResult) {
            try_var(in, f(input));
            data[i] = in;
        } else {
            data[i] = f(input);
        }
    }
    return ok(data);
}

template <typename T, typename F>
inline result<std::vector<T>> get_from_tuple(tuple inputs, F &&f) {
    return get_from_tuple_with_result<T, false>(inputs, f);
}

inline result<std::vector<dims_t>> get_shapes(tuple inputs) {
    return get_from_tuple<dims_t>(inputs,
                                  [](auto &input) { return input->shape(); });
}

inline result<std::vector<dims_t>> get_strides(tuple inputs) {
    return get_from_tuple<dims_t>(inputs,
                                  [](auto &input) { return input->strides(); });
}

template <bool IsTuple, typename ShapeT = dims_t>
inline result<void> alloc_output(value_t &output, datatype_t dtype,
                                 const ShapeT &out_shape);

template <>
inline result<void> alloc_output<false, dims_t>(value_t &output,
                                                datatype_t dtype,
                                                const dims_t &out_shape) {
    // TODO: copy back output
    if (output.empty()) {
        auto out_strides = get_default_strides(out_shape);
        try_var(out_buffer, buffer_allocator::host().allocate(
                                get_bytes(dtype, out_shape, out_strides), {}));
        output =
            tensor(std::in_place, dtype, out_shape, out_strides, out_buffer);
    } else {
        try_var(
            out_tensor,
            output.as<tensor>()) if (out_tensor->shape() !=
                                     out_shape) return err(nncase_errc::
                                                               shape_mismatch);
    }
    return ok();
}

template <>
inline result<void>
alloc_output<true, std::vector<dims_t>>(value_t &outputs, datatype_t dtype,
                                        const std::vector<dims_t> &out_shapes) {
    if (outputs.empty()) {
        auto size = out_shapes.size();
        std::vector<value_t> fields(size);
        for (int i = 0; i < size; ++i) {
            auto output = value_t();
            try_(alloc_output<false>(output, dtype, out_shapes[i]));
            fields[i] = output;
        }
        outputs = tuple(std::in_place, std::move(fields));
    } else {
        try_var(output_tuple, outputs.as<tuple>());
        try_(tuple_for_each_with_i(
            output_tuple, [&](auto &output, auto i) -> result<void> {
                try_var(out_tensor, output.template as<tensor>());
                if (out_tensor->shape() != out_shapes[i]) {
                    return err(nncase_errc::shape_mismatch);
                } else {
                    return ok();
                }
            }));
    }
    return ok();
}

inline result<host_buffer_slice> get_host_buffer(tensor tensor) {
    try_var(tensor_host, tensor->to_host());
    try_var(tensor_buffer, tensor_host->buffer().as_host());
    return ok(tensor_buffer);
}

inline result<gsl::byte *> get_output_data(tensor output) {
    try_var(output_buffer, get_host_buffer(output));
    try_var(output_map, output_buffer.map(map_write));
    return ok(output_map.buffer().data());
}

inline result<std::vector<gsl::byte *>> get_output_data(tuple outputs) {
    return get_from_tuple_with_result<gsl::byte *, true>(
        outputs, [](tensor &input) { return get_output_data(input); });
}

#ifndef NODEBUG
// used for insert into some where for check nan value in DEBUG mode
inline void nan_debug(const float *in, int size)
{
    auto f = *in;
    if(f != f) {
        for (int i = 1; i < size; ++i) {
            auto fv = *(in + i);
            if(fv != fv) {
                [[maybe_unused]] auto a = 1;
            }
        }
    }
}
#else
inline void nan_debug(const float *in) {}
#endif

inline result<gsl::byte *> get_input_data(tensor input) {
    try_var(input_buffer, get_host_buffer(input));
    try_var(input_map, input_buffer.map(map_read));
    auto d = input_map.buffer().data();
    return ok(d);
}

inline result<std::vector<gsl::byte *>> get_input_data(tuple inputs) {
    return get_from_tuple_with_result<gsl::byte *, true>(
        inputs, [](tensor &input) { return get_input_data(input); });
}

inline size_t positive_index(int index, size_t rank) {
    return index < 0 ? index + rank : index;
}

#define try_alloc_output(_out_tensor, _dt, _shape, _is_tuple)                  \
    try_(alloc_output<_is_tuple>(_out_tensor, _dt, _shape));

#define try_input_impl(_var_name, _value_name, _value_kind)                    \
    try_var(_value_name##_##_value_kind, _value_name.as<_value_kind>());       \
    try_var(_var_name, get_input_data(_value_name##_##_value_kind))

#define try_input(_var_name, _value_name)                                      \
    try_input_impl(_var_name, _value_name, tensor)
#define try_tuple_input(_var_name, _value_name)                                \
    try_input_impl(_var_name, _value_name, tuple)

#define try_input_with_ty(_var_name, _value_name, _ty)                         \
    try_input(__##_var_name, _value_name);                                     \
    try_(type_only_check<_ty>(_value_name##_tensor));                          \
    auto _var_name = reinterpret_cast<const _ty *>(__##_var_name)

#define try_integer_input(_var_name, _value_name)                              \
    try_input_with_ty(_var_name, _value_name, int64_t)

#define try_f32_input(_var_name, _value_name)                                  \
    try_input_with_ty(_var_name, _value_name, float)
#define try_f32_output(_var_name, _value_name, _dt, _out_shape)                \
    try_output(__##_var_name, _value_name, _dt, _out_shape);                   \
    auto _var_name = reinterpret_cast<float *>(__##_var_name)

// todo:when _value_kind is tuple, _value_name_tensor is a bad name
#define try_output_impl(_var_name, _value_name, _dt, _out_shape, _value_kind,  \
                        _is_tuple)                                             \
    try_alloc_output(_value_name, _dt, _out_shape, _is_tuple);                 \
    try_var(_value_name##_##_value_kind, _value_name.as<_value_kind>());       \
    try_var(_var_name, get_output_data(_value_name##_##_value_kind))

#define try_output(_var_name, _value_name, _dt, _out_shape)                    \
    try_output_impl(_var_name, _value_name, _dt, _out_shape, tensor, false)

#define try_tuple_output(_var_name, _value_name, _dt, _out_shapes)             \
    try_output_impl(_var_name, _value_name, _dt, _out_shapes, tuple, true)

#define try_value_as(_var_name, _value_name, f_name)                           \
    try_var(_var_name, value_as_##f_name(_value_name))
#define try_strides(_var_name, _value_name)                                    \
    try_value_as(_var_name, _value_name, strides)
#define try_dims(_var_name, _value_name)                                       \
    try_value_as(_var_name, _value_name, dims)
#define try_positive_axes(_var_name, _value_name, _rank)                       \
    try_var(_var_name, value_as_positive_axes(_value_name, _rank))

#define try_axes(_var_name, _value_name)                                       \
    try_var(_var_name, value_as_axes(_value_name))
#define try_paddings(_var_name, _value_name)                                   \
    try_value_as(_var_name, _value_name, paddings)
#define try_value_as_t(_var_name, _value_name, _ty, f_name)                    \
    try_var(_var_name, value_as_##f_name<_ty>(_value_name))
#define try_to_scalar(_var_name, _value_name, _ty)                             \
    try_var(_var_name, value_to_scalar<_ty>(_value_name))

#define try_to_integer(_var_name, _value_name)                                 \
    try_to_scalar(_var_name, _value_name, int64_t)

#define try_positive_axis_with_rank(_var_name, _value_name, _rank)             \
    try_to_scalar(__##_var_name, _value_name, int64_t);                        \
    auto _var_name = positive_index(__##_var_name, _rank)

#define try_positive_axis(_var_name, _value_name, _input_tensor)               \
    try_positive_axis_with_rank(_var_name, _value_name,                        \
                                _input_tensor->shape().size())

#define try_array(_var_name, _value_name, _ty)                                 \
    try_value_as_t(_var_name, _value_name, _ty, array)

#define try_typecode(_var_name, _tensor_name)                                  \
    try_var(_var_name, to_typecode(_tensor_name->dtype()))

#define try_ref(op, ...) try_(reference::op(__VA_ARGS__))

#define finish return ok(output)
#define tuple_finish return ok(output_tuple)

template <typename T>
inline result<T> value_to_scalar([[maybe_unused]] value_t value) {
    try_input(input, value);
    // todo: maybe this is a bad way?
#define RETURN_RESULT(_in_type)                                                \
    if (cmp_type<_in_type>(value_tensor->dtype())) {                           \
        return ok((T)(*reinterpret_cast<const _in_type *>(input)));            \
    }
    RETURN_RESULT(bool);
    RETURN_RESULT(float);
    RETURN_RESULT(int32_t);
    RETURN_RESULT(int64_t);
    RETURN_RESULT(uint32_t);
    RETURN_RESULT(uint64_t);
    return err(nncase_errc::datatype_mismatch);
#undef RETURN_RESULT
}

inline result<scalar> tensor_as_scalar([[maybe_unused]] value_t value) {
    throw "NotImplement";
}

template <typename TI, typename TO>
itlib::small_vector<TO, 4> to_vec(const gsl::byte *input, size_t size) {
    auto in_ptr = reinterpret_cast<const TI *>(input);
    auto vec = itlib::small_vector<TO, 4>(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = (TO)in_ptr[i];
    }
    return vec;
}

template <typename T>
inline result<itlib::small_vector<T, 4>> value_as_Ts(value_t value) {
    try_input(input, value);
    auto rank = value_tensor->shape().size();
    if (rank > 1) {
        // only used for rank 1
        return Err(nncase_errc::shape_mismatch);
    }
    auto size = value_tensor->shape()[0];
#define RETURN_RESULT(_in_type)                                                \
    if (cmp_type<_in_type>(value_tensor->dtype())) {                           \
        return ok(to_vec<_in_type, T>(input, size));                           \
    }

    static_assert(std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t> ||
                      std::is_same_v<T, int64_t> ||
                      std::is_same_v<T, int64_t> || std::is_same_v<T, size_t>,
                  "not suppported type");
    RETURN_RESULT(int32_t);
    RETURN_RESULT(uint32_t);
    RETURN_RESULT(int64_t);
    RETURN_RESULT(uint64_t);
#undef RETURN_RESULT
    return err(nncase_errc::datatype_mismatch);
}

inline result<dims_t> value_as_dims(value_t value) {
    return value_as_Ts<dims_t::value_type>(value);
}

inline result<axes_t> value_as_axes(value_t value) {
    return value_as_Ts<axes_t::value_type>(value);
}

// todo:refactor
inline result<dims_t> value_as_positive_axes(value_t value, size_t rank) {
    try_input_with_ty(input, value, axes_t::value_type);
    auto size = value_tensor->shape()[0];
    auto axis = dims_t(size);
    for (int i = 0; i < size; ++i) {
        axis[i] = positive_index(input[i], rank);
    }
    return ok(axis);
}

inline result<strides_t> value_as_strides(value_t value) {
    return value_as_Ts<strides_t::value_type>(value);
}

inline result<paddings_t> value_as_paddings([[maybe_unused]] value_t value) {
    //    try_input_with_ty(input, value, size_t);
    throw "NotImplement";
}

inline result<quant_param_t>
value_as_quant_param([[maybe_unused]] value_t value) {
    throw "NotImplement";
}

template <typename T>
inline result<T *> value_as_array([[maybe_unused]] value_t v) {
    throw "NotImplement";
}

#define TYPE_SELECT(_typecode, _impl)                                          \
    switch (_typecode) {                                                       \
    case dt_float32:                                                           \
        _impl(float);                                                          \
    case dt_int8:                                                              \
        _impl(int8_t);                                                         \
    case dt_int16:                                                             \
        _impl(int16_t);                                                        \
    case dt_int32:                                                             \
        _impl(int32_t);                                                        \
    case dt_int64:                                                             \
        _impl(int64_t);                                                        \
    case dt_uint8:                                                             \
        _impl(uint8_t);                                                        \
    case dt_uint16:                                                            \
        _impl(uint16_t);                                                       \
    case dt_uint32:                                                            \
        _impl(uint32_t);                                                       \
    case dt_uint64:                                                            \
        _impl(uint64_t);                                                       \
    case dt_float64:                                                           \
        _impl(double);                                                         \
    default:                                                                   \
        return err(std::errc::not_supported);                                  \
    }

#define IN_CAST(_ty, _name) reinterpret_cast<const _ty *>(_name)
#define OUT_CAST(_ty, _name) reinterpret_cast<_ty *>(_name)
#define SCALAR_CAST(_ty, _name) *reinterpret_cast<const _ty *>(_name)

inline bool is_contiguous(tensor tensor) {
    return is_contiguous(tensor->shape(), tensor->strides());
}

// kernel dispatch for single input
#define CONTIGUOUS_KERNEL(_op, _in_tensor, ...)                                \
    if (is_contiguous(_in_tensor)) {                                           \
        try_(reference::_op(__VA_ARGS__))                                      \
    } else {                                                                   \
        try_(optimized::_op(__VA_ARGS__))                                      \
    }

#define IN_CAST(_ty, _name) reinterpret_cast<const _ty *>(_name)
#define OUT_CAST(_ty, _name) reinterpret_cast<_ty *>(_name)
#define SCALAR_CAST(_ty, _name) *reinterpret_cast<const _ty *>(_name)
} // namespace nncase::runtime