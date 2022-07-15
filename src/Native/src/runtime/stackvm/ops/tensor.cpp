/* This file is generated by tools/stackvm_gen/IsaGen at 2022/7/13 下午4:54:41 +08:00.
 *
 * Copyright 2019-2021 Canaan Inc.
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
#include "../runtime_function.h"
#include <nncase/kernels/stackvm/tensor_ops.h>
#include <nncase/debug.h>
using namespace nncase;
using namespace nncase::runtime;
using namespace nncase::runtime::stackvm;

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_batch_normalization_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(scale, pop_value());
    dump_input(scale, "scale");
    try_var(bias, pop_value());
    dump_input(bias, "bias");
    try_var(input_mean, pop_value());
    dump_input(input_mean, "input_mean");
    try_var(input_var, pop_value());
    dump_input(input_var, "input_var");
    try_var(epsilon, pop_value());
    dump_input(epsilon, "epsilon");
    try_var(momentum, pop_value());
    dump_input(momentum, "momentum");
    try_var(output, kernels::stackvm::batch_normalization(input, scale, bias, input_mean, input_var, epsilon, momentum, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_batch_to_space_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(block_shape, pop_value());
    dump_input(block_shape, "block_shape");
    try_var(crops, pop_value());
    dump_input(crops, "crops");
    try_var(output, kernels::stackvm::batch_to_space(input, block_shape, crops, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_binary_op_t &op) noexcept {
    try_var(lhs, pop_value());
    dump_input(lhs, "lhs");
    try_var(rhs, pop_value());
    dump_input(rhs, "rhs");
    try_var(output, kernels::stackvm::binary(op.binary_op, lhs, rhs, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_broadcast_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(shape, pop_value());
    dump_input(shape, "shape");
    try_var(output, kernels::stackvm::broadcast(input, shape, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_cast_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::cast(op.new_type, input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_celu_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(alpha, pop_value());
    dump_input(alpha, "alpha");
    try_var(output, kernels::stackvm::celu(input, alpha, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_clamp_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(min, pop_value());
    dump_input(min, "min");
    try_var(max, pop_value());
    dump_input(max, "max");
    try_var(output, kernels::stackvm::clamp(input, min, max, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_compare_op_t &op) noexcept {
    try_var(lhs, pop_value());
    dump_input(lhs, "lhs");
    try_var(rhs, pop_value());
    dump_input(rhs, "rhs");
    try_var(output, kernels::stackvm::compare(op.compare_op, lhs, rhs, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_concat_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(output, kernels::stackvm::concat(input, axis, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_constant_of_shape_op_t &op) noexcept {
    try_var(shape, pop_value());
    dump_input(shape, "shape");
    try_var(value, pop_value());
    dump_input(value, "value");
    try_var(output, kernels::stackvm::constant_of_shape(shape, value, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_conv2d_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(weights, pop_value());
    dump_input(weights, "weights");
    try_var(bias, pop_value());
    dump_input(bias, "bias");
    try_var(stride, pop_value());
    dump_input(stride, "stride");
    try_var(padding, pop_value());
    dump_input(padding, "padding");
    try_var(dilation, pop_value());
    dump_input(dilation, "dilation");
    try_var(groups, pop_value());
    dump_input(groups, "groups");
    try_var(fused_clamp, pop_value());
    dump_input(fused_clamp, "fused_clamp");
    try_var(output, kernels::stackvm::conv2d(op.pad_mode, input, weights, bias, stride, padding, dilation, groups, fused_clamp, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_conv2d_transpose_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(weights, pop_value());
    dump_input(weights, "weights");
    try_var(bias, pop_value());
    dump_input(bias, "bias");
    try_var(output_shape, pop_value());
    dump_input(output_shape, "output_shape");
    try_var(stride, pop_value());
    dump_input(stride, "stride");
    try_var(padding, pop_value());
    dump_input(padding, "padding");
    try_var(output_padding, pop_value());
    dump_input(output_padding, "output_padding");
    try_var(dilation, pop_value());
    dump_input(dilation, "dilation");
    try_var(groups, pop_value());
    dump_input(groups, "groups");
    try_var(fused_clamp, pop_value());
    dump_input(fused_clamp, "fused_clamp");
    try_var(output, kernels::stackvm::conv2d_transpose(op.pad_mode, input, weights, bias, output_shape, stride, padding, output_padding, dilation, groups, fused_clamp, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_cum_sum_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(exclusive, pop_value());
    dump_input(exclusive, "exclusive");
    try_var(reverse, pop_value());
    dump_input(reverse, "reverse");
    try_var(output, kernels::stackvm::cum_sum(input, axis, exclusive, reverse, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_dequantize_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(dequant_param, pop_value());
    dump_input(dequant_param, "dequant_param");
    try_var(output, kernels::stackvm::dequantize(op.target_type, input, dequant_param, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_elu_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(alpha, pop_value());
    dump_input(alpha, "alpha");
    try_var(output, kernels::stackvm::elu(input, alpha, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_expand_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(shape, pop_value());
    dump_input(shape, "shape");
    try_var(output, kernels::stackvm::expand(input, shape, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_fake_dequantize_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(dequant_param, pop_value());
    dump_input(dequant_param, "dequant_param");
    try_var(output, kernels::stackvm::fake_dequantize(op.target_type, input, dequant_param, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_fake_quantize_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(quant_param, pop_value());
    dump_input(quant_param, "quant_param");
    try_var(output, kernels::stackvm::fake_quantize(op.target_type, input, quant_param, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_flatten_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(output, kernels::stackvm::flatten(input, axis, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_gather_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(index, pop_value());
    dump_input(index, "index");
    try_var(output, kernels::stackvm::gather(input, axis, index, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_gather_nd_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(batch_dims, pop_value());
    dump_input(batch_dims, "batch_dims");
    try_var(index, pop_value());
    dump_input(index, "index");
    try_var(output, kernels::stackvm::gather_nd(input, batch_dims, index, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_get_item_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(index, pop_value());
    dump_input(index, "index");
    try_var(output, kernels::stackvm::get_item(input, index, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_hard_sigmoid_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(alpha, pop_value());
    dump_input(alpha, "alpha");
    try_var(beta, pop_value());
    dump_input(beta, "beta");
    try_var(output, kernels::stackvm::hard_sigmoid(input, alpha, beta, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_hard_swish_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::hard_swish(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_hardmax_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(output, kernels::stackvm::hardmax(input, axis, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_instance_normalization_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(scale, pop_value());
    dump_input(scale, "scale");
    try_var(bias, pop_value());
    dump_input(bias, "bias");
    try_var(epsilon, pop_value());
    dump_input(epsilon, "epsilon");
    try_var(output, kernels::stackvm::instance_normalization(input, scale, bias, epsilon, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_l2_normalization_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::l2_normalization(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_leaky_relu_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(alpha, pop_value());
    dump_input(alpha, "alpha");
    try_var(output, kernels::stackvm::leaky_relu(input, alpha, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_log_softmax_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(output, kernels::stackvm::log_softmax(input, axis, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_lp_normalization_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(p, pop_value());
    dump_input(p, "p");
    try_var(output, kernels::stackvm::lp_normalization(input, axis, p, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_lrn_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(alpha, pop_value());
    dump_input(alpha, "alpha");
    try_var(beta, pop_value());
    dump_input(beta, "beta");
    try_var(bias, pop_value());
    dump_input(bias, "bias");
    try_var(size, pop_value());
    dump_input(size, "size");
    try_var(output, kernels::stackvm::lrn(input, alpha, beta, bias, size, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_lstm_op_t &op) noexcept {
    try_var(x, pop_value());
    dump_input(x, "x");
    try_var(w, pop_value());
    dump_input(w, "w");
    try_var(r, pop_value());
    dump_input(r, "r");
    try_var(b, pop_value());
    dump_input(b, "b");
    try_var(sequence_lens, pop_value());
    dump_input(sequence_lens, "sequence_lens");
    try_var(initial_h, pop_value());
    dump_input(initial_h, "initial_h");
    try_var(initial_c, pop_value());
    dump_input(initial_c, "initial_c");
    try_var(p, pop_value());
    dump_input(p, "p");
    try_var(activation_alpha, pop_value());
    dump_input(activation_alpha, "activation_alpha");
    try_var(activation_beta, pop_value());
    dump_input(activation_beta, "activation_beta");
    try_var(clip, pop_value());
    dump_input(clip, "clip");
    try_var(hidden_size, pop_value());
    dump_input(hidden_size, "hidden_size");
    try_var(input_forget, pop_value());
    dump_input(input_forget, "input_forget");
    try_var(output_size, pop_value());
    dump_input(output_size, "output_size");
    try_var(output, kernels::stackvm::lstm(op.direction, op.layout, op.activations, x, w, r, b, sequence_lens, initial_h, initial_c, p, activation_alpha, activation_beta, clip, hidden_size, input_forget, output_size, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_mat_mul_op_t &op) noexcept {
    try_var(lhs, pop_value());
    dump_input(lhs, "lhs");
    try_var(rhs, pop_value());
    dump_input(rhs, "rhs");
    try_var(output, kernels::stackvm::mat_mul(lhs, rhs, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_normal_op_t &op) noexcept {
    try_var(mean, pop_value());
    dump_input(mean, "mean");
    try_var(scale, pop_value());
    dump_input(scale, "scale");
    try_var(seed, pop_value());
    dump_input(seed, "seed");
    try_var(shape, pop_value());
    dump_input(shape, "shape");
    try_var(output, kernels::stackvm::normal(op.type, mean, scale, seed, shape, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_normal_like_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(mean, pop_value());
    dump_input(mean, "mean");
    try_var(scale, pop_value());
    dump_input(scale, "scale");
    try_var(seed, pop_value());
    dump_input(seed, "seed");
    try_var(output, kernels::stackvm::normal_like(op.type, input, mean, scale, seed, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_one_hot_op_t &op) noexcept {
    try_var(indices, pop_value());
    dump_input(indices, "indices");
    try_var(depth, pop_value());
    dump_input(depth, "depth");
    try_var(values, pop_value());
    dump_input(values, "values");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(output, kernels::stackvm::one_hot(op.one_hot_mode, indices, depth, values, axis, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_pad_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(pads, pop_value());
    dump_input(pads, "pads");
    try_var(value, pop_value());
    dump_input(value, "value");
    try_var(output, kernels::stackvm::pad(op.pad_mode, input, pads, value, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_prelu_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(slope, pop_value());
    dump_input(slope, "slope");
    try_var(output, kernels::stackvm::prelu(input, slope, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_prod_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::prod(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_quant_param_of_op_t &op) noexcept {
    try_var(range, pop_value());
    dump_input(range, "range");
    try_var(bits, pop_value());
    dump_input(bits, "bits");
    try_var(output, kernels::stackvm::quant_param_of(op.quant_mode, range, bits, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_quantize_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(quant_param, pop_value());
    dump_input(quant_param, "quant_param");
    try_var(output, kernels::stackvm::quantize(op.target_type, input, quant_param, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_range_op_t &op) noexcept {
    try_var(begin, pop_value());
    dump_input(begin, "begin");
    try_var(end, pop_value());
    dump_input(end, "end");
    try_var(step, pop_value());
    dump_input(step, "step");
    try_var(output, kernels::stackvm::range(begin, end, step, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_range_of_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::range_of(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_reduce_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(init_value, pop_value());
    dump_input(init_value, "init_value");
    try_var(keep_dims, pop_value());
    dump_input(keep_dims, "keep_dims");
    try_var(output, kernels::stackvm::reduce(op.reduce_op, input, axis, init_value, keep_dims, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_reduce_arg_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(keep_dims, pop_value());
    dump_input(keep_dims, "keep_dims");
    try_var(select_last_index, pop_value());
    dump_input(select_last_index, "select_last_index");
    try_var(output, kernels::stackvm::reduce_arg(op.reduce_arg_op, input, axis, keep_dims, select_last_index, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_reduce_window2d_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(init_value, pop_value());
    dump_input(init_value, "init_value");
    try_var(filter, pop_value());
    dump_input(filter, "filter");
    try_var(stride, pop_value());
    dump_input(stride, "stride");
    try_var(padding, pop_value());
    dump_input(padding, "padding");
    try_var(dilation, pop_value());
    dump_input(dilation, "dilation");
    try_var(ceil_mode, pop_value());
    dump_input(ceil_mode, "ceil_mode");
    try_var(count_include_pad, pop_value());
    dump_input(count_include_pad, "count_include_pad");
    try_var(output, kernels::stackvm::reduce_window2d(op.reduce_op, input, init_value, filter, stride, padding, dilation, ceil_mode, count_include_pad, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_relu_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::relu(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_relu6_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::relu6(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_require_op_t &op) noexcept {
    try_var(predicate, pop_value());
    dump_input(predicate, "predicate");
    try_var(value, pop_value());
    dump_input(value, "value");
    try_var(output, kernels::stackvm::require(op.message, predicate, value, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_reshape_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(shape, pop_value());
    dump_input(shape, "shape");
    try_var(output, kernels::stackvm::reshape(input, shape, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_resize_image_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(roi, pop_value());
    dump_input(roi, "roi");
    try_var(new_size, pop_value());
    dump_input(new_size, "new_size");
    try_var(cubic_coeff_a, pop_value());
    dump_input(cubic_coeff_a, "cubic_coeff_a");
    try_var(exclude_outside, pop_value());
    dump_input(exclude_outside, "exclude_outside");
    try_var(extrapolation_value, pop_value());
    dump_input(extrapolation_value, "extrapolation_value");
    try_var(output, kernels::stackvm::resize_image(op.resize_mode, op.transformation_mode, op.nearest_mode, op.is_tfresize, input, roi, new_size, cubic_coeff_a, exclude_outside, extrapolation_value, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_reverse_sequence_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(seq_lens, pop_value());
    dump_input(seq_lens, "seq_lens");
    try_var(batch_axis, pop_value());
    dump_input(batch_axis, "batch_axis");
    try_var(time_axis, pop_value());
    dump_input(time_axis, "time_axis");
    try_var(output, kernels::stackvm::reverse_sequence(input, seq_lens, batch_axis, time_axis, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_select_op_t &op) noexcept {
    try_var(predicate, pop_value());
    dump_input(predicate, "predicate");
    try_var(true_value, pop_value());
    dump_input(true_value, "true_value");
    try_var(false_value, pop_value());
    dump_input(false_value, "false_value");
    try_var(output, kernels::stackvm::select(predicate, true_value, false_value, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_selu_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(alpha, pop_value());
    dump_input(alpha, "alpha");
    try_var(gamma, pop_value());
    dump_input(gamma, "gamma");
    try_var(output, kernels::stackvm::selu(input, alpha, gamma, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_shape_of_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::shape_of(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_sigmoid_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::sigmoid(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_size_of_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::size_of(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_slice_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(begins, pop_value());
    dump_input(begins, "begins");
    try_var(ends, pop_value());
    dump_input(ends, "ends");
    try_var(axes, pop_value());
    dump_input(axes, "axes");
    try_var(strides, pop_value());
    dump_input(strides, "strides");
    try_var(output, kernels::stackvm::slice(input, begins, ends, axes, strides, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_softmax_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(output, kernels::stackvm::softmax(input, axis, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_softplus_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::softplus(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_softsign_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::softsign(input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_space_to_batch_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(block_shape, pop_value());
    dump_input(block_shape, "block_shape");
    try_var(paddings, pop_value());
    dump_input(paddings, "paddings");
    try_var(output, kernels::stackvm::space_to_batch(input, block_shape, paddings, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_split_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(sections, pop_value());
    dump_input(sections, "sections");
    try_var(output, kernels::stackvm::split(input, axis, sections, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_squeeze_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(dim, pop_value());
    dump_input(dim, "dim");
    try_var(output, kernels::stackvm::squeeze(input, dim, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_stack_op_t &op) noexcept {
    try_var(inputs, pop_value());
    dump_input(inputs, "inputs");
    try_var(axis, pop_value());
    dump_input(axis, "axis");
    try_var(output, kernels::stackvm::stack(inputs, axis, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_tile_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(repeats, pop_value());
    dump_input(repeats, "repeats");
    try_var(output, kernels::stackvm::tile(input, repeats, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_transpose_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(perm, pop_value());
    dump_input(perm, "perm");
    try_var(output, kernels::stackvm::transpose(input, perm, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_unary_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(output, kernels::stackvm::unary(op.unary_op, input, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_uniform_op_t &op) noexcept {
    try_var(high, pop_value());
    dump_input(high, "high");
    try_var(low, pop_value());
    dump_input(low, "low");
    try_var(seed, pop_value());
    dump_input(seed, "seed");
    try_var(shape, pop_value());
    dump_input(shape, "shape");
    try_var(output, kernels::stackvm::uniform(op.type, high, low, seed, shape, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_uniform_like_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(high, pop_value());
    dump_input(high, "high");
    try_var(low, pop_value());
    dump_input(low, "low");
    try_var(seed, pop_value());
    dump_input(seed, "seed");
    try_var(output, kernels::stackvm::uniform_like(op.type, input, high, low, seed, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_unsqueeze_op_t &op) noexcept {
    try_var(input, pop_value());
    dump_input(input, "input");
    try_var(dim, pop_value());
    dump_input(dim, "dim");
    try_var(output, kernels::stackvm::unsqueeze(input, dim, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}

result<void> stackvm_runtime_function::visit([[maybe_unused]] const tensor_where_op_t &op) noexcept {
    try_var(cond, pop_value());
    dump_input(cond, "cond");
    try_var(x, pop_value());
    dump_input(x, "x");
    try_var(y, pop_value());
    dump_input(y, "y");
    try_var(output, kernels::stackvm::where(cond, x, y, nullptr, module().kernel_context()));
    dump_output(output);
    return stack_.push(std::move(output));
}
