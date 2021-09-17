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
#include "shape.h"
#include <nncase/runtime/datatypes.h>
#include <range/v3/range/concepts.hpp>

namespace nncase::ir {
/** @brief Type node */
class NNCASE_API type_node : public object_node {
    DEFINE_OBJECT_KIND(object_node, object_type);

  protected:
    type_node() = default;
};

/** @brief Type */
using type = object_t<type_node>;

/** @brief Any type node */
class NNCASE_API any_type_node : public type_node {
    DEFINE_OBJECT_KIND(object_node, object_any_type);

  public:
};

/** @brief Any type */
class any_type : public object_t<any_type_node> {
  public:
    using object_t::object_t;
};

/** @brief Invalid type node */
class NNCASE_API invalid_type_node : public type_node {
    DEFINE_OBJECT_KIND(object_node, object_invalid_type);

  public:
    invalid_type_node();
    invalid_type_node(std::string reason);

    /** @brief Get reason */
    const std::string &reason() const noexcept { return reason_; }
    /** @brief Get mutable reason */
    std::string &reason() noexcept { return reason_; }
    /** @brief Set reason */
    void reason(std::string value) noexcept { reason_ = std::move(value); }

  private:
    std::string reason_;
};

/** @brief Invalid type */
class invalid_type : public object_t<invalid_type_node> {
  public:
    using object_t::object_t;

    NNCASE_API invalid_type();
    NNCASE_API invalid_type(std::string reason);
};

/** @brief Tensor type node */
class NNCASE_API tensor_type_node : public type_node {
    DEFINE_OBJECT_KIND(object_node, object_tensor_type);

  public:
    tensor_type_node(datatype_t elem_type, shape_t shape);

    /** @brief Is this a scalar type */
    bool is_scalar() const noexcept { return shape_.is_scalar(); }
    /** @brief Is this a tensor type */
    bool is_tensor() const noexcept { return !shape_.is_scalar(); }

    /** @brief Get element datatype */
    datatype_t dtype() const noexcept { return dtype_; }
    /** @brief Set element datatype */
    void dtype(datatype_t value) noexcept { dtype_ = value; }

    /** @brief Get shape */
    const shape_t &shape() const noexcept { return shape_; }
    /** @brief Get mutable shape */
    shape_t &shape() noexcept { return shape_; }
    /** @brief Set shape */
    void shape(shape_t value) noexcept { shape_ = std::move(value); }

  private:
    datatype_t dtype_;
    shape_t shape_;
};

/** @brief Tensor type */
class tensor_type : public object_t<tensor_type_node> {
  public:
    using object_t::object_t;

    NNCASE_API tensor_type(datatype_t elem_type, shape_t shape = scalar_shape);
};

/** @brief Tuple type node */
class NNCASE_API tuple_type_node : public type_node {
    DEFINE_OBJECT_KIND(object_node, object_tuple_type);

  public:
    tuple_type_node(itlib::small_vector<type> fields);

    /** @brief Get fields */
    std::span<const type> fields() const noexcept { return fields_; }
    /** @brief Get mutable fields */
    itlib::small_vector<type> &shape() noexcept { return fields_; }
    /** @brief Set fields */
    void shape(itlib::small_vector<type> value) noexcept {
        fields_ = std::move(value);
    }

  private:
    itlib::small_vector<type> fields_;
};

/** @brief Tuple type */
class tuple_type : public object_t<tuple_type_node> {
  public:
    using object_t::object_t;

    NNCASE_API tuple_type(itlib::small_vector<type> fields);

    template <ranges::range R>
    tuple_type(R &&fields)
        : tuple_type(itlib::small_vector<type>{fields.begin(), fields.end()}) {}
};

/** @brief Callable type node */
class NNCASE_API callable_type_node : public type_node {
    DEFINE_OBJECT_KIND(object_node, object_callable_type);

  public:
    callable_type_node(itlib::small_vector<type> parameters, type return_type);

    /** @brief Get parameters */
    std::span<const type> parameters() const noexcept { return parameters_; }
    /** @brief Get parameters */
    itlib::small_vector<type> &parameters() noexcept { return parameters_; }
    /** @brief Set parameters */
    void parameters(itlib::small_vector<type> value) noexcept {
        parameters_ = std::move(value);
    }

    /** @brief Get return type */
    const type &return_type() const noexcept { return return_type_; }
    /** @brief Get mutable return type */
    type &return_type() noexcept { return return_type_; }
    /** @brief Set return type */
    void return_type(type value) noexcept { return_type_ = std::move(value); }

  private:
    itlib::small_vector<type> parameters_;
    type return_type_;
};

/** @brief Callable type */
class callable_type : public object_t<callable_type_node> {
  public:
    using object_t::object_t;

    NNCASE_API callable_type(itlib::small_vector<type> parameters,
                             type return_type);

    template <ranges::range R>
    callable_type(R &&parameters, type return_type)
        : callable_type(
              itlib::small_vector<type>{parameters.begin(), parameters.end()},
              std::move(return_type)) {}
};
} // namespace nncase::ir
