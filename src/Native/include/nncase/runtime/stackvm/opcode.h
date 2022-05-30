/* This file is generated by tools/stackvm_gen/IsaGen at 2022/5/27 下午3:58:51 +08:00.
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
#pragma once
#include "../datatypes.h"
#include <vector>

BEGIN_NS_NNCASE_RT_MODULE(stackvm)

// Enums

enum class opcode_t : uint8_t
{
    NOP = 0,
    LDNULL = 1,
    LDC_I4 = 2,
    LDC_I4_0 = 3,
    LDC_I4_1 = 4,
    LDC_R4 = 5,
    LDIND_I1 = 6,
    LDIND_I2 = 7,
    LDIND_I4 = 8,
    LDIND_I = 9,
    LDIND_U1 = 10,
    LDIND_U2 = 11,
    LDIND_U4 = 12,
    LDIND_U = 13,
    LDIND_BR2 = 14,
    LDIND_R4 = 15,
    STIND_I1 = 16,
    STIND_I2 = 17,
    STIND_I4 = 18,
    STIND_I = 19,
    STIND_BR2 = 20,
    STIND_R4 = 21,
    LEA_GP = 22,
    LDELEM_I1 = 23,
    LDELEM_I2 = 24,
    LDELEM_I4 = 25,
    LDELEM_I = 26,
    LDELEM_U1 = 27,
    LDELEM_U2 = 28,
    LDELEM_U4 = 29,
    LDELEM_U = 30,
    LDELEM_BR2 = 31,
    LDELEM_R4 = 32,
    STELEM_I1 = 33,
    STELEM_I2 = 34,
    STELEM_I4 = 35,
    STELEM_I = 36,
    STELEM_BR2 = 37,
    STELEM_R4 = 38,
    LDARG = 39,
    LDARG_0 = 40,
    LDARG_1 = 41,
    LDARG_2 = 42,
    LDARG_3 = 43,
    LDARG_4 = 44,
    LDARG_5 = 45,
    DUP = 46,
    POP = 47,
    LDLOCAL = 48,
    STLOCAL = 49,
    LDSHAPE = 50,
    LDSTRIDES = 51,
    LDTUPLE_ELEM = 52,
    LDTUPLE = 53,
    LDDATATYPE = 54,
    LDTENSOR = 55,
    NEG = 56,
    ADD = 57,
    SUB = 58,
    MUL = 59,
    DIV = 60,
    DIV_U = 61,
    REM = 62,
    REM_U = 63,
    AND = 64,
    OR = 65,
    XOR = 66,
    NOT = 67,
    SHL = 68,
    SHR = 69,
    SHR_U = 70,
    CLT = 71,
    CLT_U = 72,
    CLE = 73,
    CLE_U = 74,
    CEQ = 75,
    CGE = 76,
    CGE_U = 77,
    CGT = 78,
    CGT_U = 79,
    CNE = 80,
    CONV_I1 = 81,
    CONV_I2 = 82,
    CONV_I4 = 83,
    CONV_I = 84,
    CONV_U1 = 85,
    CONV_U2 = 86,
    CONV_U4 = 87,
    CONV_U = 88,
    CONV_BR2 = 89,
    CONV_R4 = 90,
    BR = 91,
    BR_TRUE = 92,
    BR_FALSE = 93,
    RET = 94,
    CALL = 95,
    ECALL = 96,
    THROW = 97,
    BREAK = 98,
    TENSOR = 99,
};

enum class tensor_function_t : uint16_t
{
    batch_normalization = 0,
    batch_to_space = 1,
    celu = 5,
    conv2d = 9,
    conv2d_transpose = 10,
    elu = 13,
    hardmax = 19,
    hard_sigmoid = 20,
    hard_swish = 21,
    instance_normalization = 22,
    l2_normalization = 23,
    leaky_relu = 24,
    log_softmax = 25,
    lp_normalization = 26,
    lrn = 27,
    one_hot = 32,
    pad = 33,
    prelu = 34,
    reduce_window2d = 42,
    relu = 43,
    relu6 = 44,
    selu = 50,
    sigmoid = 52,
    softmax = 55,
    softplus = 56,
    softsign = 57,
    space_to_batch = 58,
    binary = 2,
    clamp = 6,
    compare = 7,
    cum_sum = 11,
    dequantize = 12,
    mat_mul = 29,
    quantize = 36,
    quant_param_of = 37,
    range_of = 39,
    reduce = 40,
    reduce_arg = 41,
    require = 45,
    select = 49,
    unary = 64,
    broadcast = 3,
    cast = 4,
    concat = 8,
    expand = 14,
    flatten = 15,
    gather = 16,
    gather_nd = 17,
    get_item = 18,
    lstm = 28,
    prod = 35,
    range = 38,
    reshape = 46,
    reverse_sequence = 48,
    shape_of = 51,
    size_of = 53,
    slice = 54,
    split = 59,
    squeeze = 60,
    stack = 61,
    tile = 62,
    transpose = 63,
    unsqueeze = 67,
    where = 68,
    normal = 30,
    normal_like = 31,
    uniform = 65,
    uniform_like = 66,
    resize_image = 47,
};

enum class binary_op_t : uint8_t
{
    add = 0,
    sub = 1,
    mul = 2,
    div = 3,
    mod = 4,
    min = 5,
    max = 6,
    pow = 7,
    bitwise_and = 8,
    bitwise_or = 9,
    bitwise_xor = 10,
    logical_and = 11,
    logical_or = 12,
    logical_xor = 13,
    left_shift = 14,
    right_shift = 15,
};

enum class compare_op_t : uint8_t
{
    equal = 0,
    not_equal = 1,
    lower_than = 2,
    lower_or_equal = 3,
    greater_than = 4,
    greater_or_equal = 5,
};

enum class pad_mode_t : uint8_t
{
    constant = 0,
    reflect = 1,
    symmetric = 2,
    edge = 3,
};

enum class lstmdirection_t : int32_t
{
    forward = 0,
    reverse = 1,
    bidirectional = 2,
};

enum class lstmlayout_t : int32_t
{
    zero = 0,
    one = 1,
};

enum class one_hot_mode_t : uint8_t
{
    normal = 0,
    process_neg = 1,
};

enum class quant_mode_t : int32_t
{
    unsigned_mode = 0,
    signed_symmetric_mode = 1,
    signed_asymmetric_mode = 2,
};

enum class reduce_op_t : uint8_t
{
    mean = 0,
    min = 1,
    max = 2,
    sum = 3,
    prod = 4,
};

enum class reduce_arg_op_t : uint8_t
{
    arg_min = 0,
    arg_max = 1,
};

enum class image_resize_mode_t : uint8_t
{
    bilinear = 0,
    nearest_neighbor = 1,
};

enum class image_resize_transformation_mode_t : int32_t
{
    half_pixel = 0,
    pytorch_half_pixel = 1,
    align_corners = 2,
    asymmetric = 3,
    tfcrop_and_resize = 4,
};

enum class image_resize_nearest_mode_t : int32_t
{
    round_prefer_floor = 0,
    round_prefer_ceil = 1,
    floor = 2,
    ceil = 3,
};

enum class unary_op_t : uint8_t
{
    abs = 0,
    acos = 1,
    acosh = 2,
    asin = 3,
    asinh = 4,
    ceil = 5,
    cos = 6,
    cosh = 7,
    exp = 8,
    floor = 9,
    log = 10,
    neg = 11,
    round = 12,
    rsqrt = 13,
    sin = 14,
    sinh = 15,
    sign = 16,
    sqrt = 17,
    square = 18,
    tanh = 19,
    bitwise_not = 20,
    logical_not = 21,
};

// Instructions
struct op_base_t
{
    opcode_t opcode;
};

struct nop_op_t : public op_base_t
{
};

struct br_op_t : public op_base_t
{
    int32_t target;
};

struct br_true_op_t : public op_base_t
{
    int32_t target;
};

struct br_false_op_t : public op_base_t
{
    int32_t target;
};

struct ret_op_t : public op_base_t
{
};

struct call_op_t : public op_base_t
{
    uint8_t args;
    int32_t target;
};

struct ecall_op_t : public op_base_t
{
    uint8_t args;
};

struct throw_op_t : public op_base_t
{
};

struct break_op_t : public op_base_t
{
};

struct ldc_i4_op_t : public op_base_t
{
    int32_t imm;
};

struct ldnull_op_t : public op_base_t
{
};

struct ldc_i4_0_op_t : public op_base_t
{
};

struct ldc_i4_1_op_t : public op_base_t
{
};

struct ldc_r4_op_t : public op_base_t
{
    float imm;
};

struct ldind_i1_op_t : public op_base_t
{
};

struct ldind_i2_op_t : public op_base_t
{
};

struct ldind_i4_op_t : public op_base_t
{
};

struct ldind_i_op_t : public op_base_t
{
};

struct ldind_u1_op_t : public op_base_t
{
};

struct ldind_u2_op_t : public op_base_t
{
};

struct ldind_u4_op_t : public op_base_t
{
};

struct ldind_u_op_t : public op_base_t
{
};

struct ldind_br2_op_t : public op_base_t
{
};

struct ldind_r4_op_t : public op_base_t
{
};

struct stind_i1_op_t : public op_base_t
{
};

struct stind_i2_op_t : public op_base_t
{
};

struct stind_i4_op_t : public op_base_t
{
};

struct stind_i_op_t : public op_base_t
{
};

struct stind_br2_op_t : public op_base_t
{
};

struct stind_r4_op_t : public op_base_t
{
};

struct lea_gp_op_t : public op_base_t
{
    uint8_t gpid;
    int32_t offset;
};

struct ldelem_i1_op_t : public op_base_t
{
};

struct ldelem_i2_op_t : public op_base_t
{
};

struct ldelem_i4_op_t : public op_base_t
{
};

struct ldelem_i_op_t : public op_base_t
{
};

struct ldelem_u1_op_t : public op_base_t
{
};

struct ldelem_u2_op_t : public op_base_t
{
};

struct ldelem_u4_op_t : public op_base_t
{
};

struct ldelem_u_op_t : public op_base_t
{
};

struct ldelem_br2_op_t : public op_base_t
{
};

struct ldelem_r4_op_t : public op_base_t
{
};

struct stelem_i1_op_t : public op_base_t
{
};

struct stelem_i2_op_t : public op_base_t
{
};

struct stelem_i4_op_t : public op_base_t
{
};

struct stelem_i_op_t : public op_base_t
{
};

struct stelem_br2_op_t : public op_base_t
{
};

struct stelem_r4_op_t : public op_base_t
{
};

struct ldarg_op_t : public op_base_t
{
    uint16_t index;
};

struct ldarg_0_op_t : public op_base_t
{
};

struct ldarg_1_op_t : public op_base_t
{
};

struct ldarg_2_op_t : public op_base_t
{
};

struct ldarg_3_op_t : public op_base_t
{
};

struct ldarg_4_op_t : public op_base_t
{
};

struct ldarg_5_op_t : public op_base_t
{
};

struct ldshape_op_t : public op_base_t
{
};

struct ldstrides_op_t : public op_base_t
{
};

struct ldtuple_elem_op_t : public op_base_t
{
};

struct ldtuple_op_t : public op_base_t
{
};

struct lddatatype_op_t : public op_base_t
{
};

struct ldtensor_op_t : public op_base_t
{
};

struct ldlocal_op_t : public op_base_t
{
    uint16_t index;
};

struct stlocal_op_t : public op_base_t
{
    uint16_t index;
};

struct dup_op_t : public op_base_t
{
};

struct pop_op_t : public op_base_t
{
};

struct neg_op_t : public op_base_t
{
};

struct add_op_t : public op_base_t
{
};

struct sub_op_t : public op_base_t
{
};

struct mul_op_t : public op_base_t
{
};

struct div_op_t : public op_base_t
{
};

struct div_u_op_t : public op_base_t
{
};

struct rem_op_t : public op_base_t
{
};

struct rem_u_op_t : public op_base_t
{
};

struct and_op_t : public op_base_t
{
};

struct or_op_t : public op_base_t
{
};

struct xor_op_t : public op_base_t
{
};

struct not_op_t : public op_base_t
{
};

struct shl_op_t : public op_base_t
{
};

struct shr_op_t : public op_base_t
{
};

struct shr_u_op_t : public op_base_t
{
};

struct clt_op_t : public op_base_t
{
};

struct clt_u_op_t : public op_base_t
{
};

struct cle_op_t : public op_base_t
{
};

struct cle_u_op_t : public op_base_t
{
};

struct ceq_op_t : public op_base_t
{
};

struct cge_op_t : public op_base_t
{
};

struct cge_u_op_t : public op_base_t
{
};

struct cgt_op_t : public op_base_t
{
};

struct cgt_u_op_t : public op_base_t
{
};

struct cne_op_t : public op_base_t
{
};

struct conv_i1_op_t : public op_base_t
{
};

struct conv_i2_op_t : public op_base_t
{
};

struct conv_i4_op_t : public op_base_t
{
};

struct conv_i_op_t : public op_base_t
{
};

struct conv_u1_op_t : public op_base_t
{
};

struct conv_u2_op_t : public op_base_t
{
};

struct conv_u4_op_t : public op_base_t
{
};

struct conv_u_op_t : public op_base_t
{
};

struct conv_br2_op_t : public op_base_t
{
};

struct conv_r4_op_t : public op_base_t
{
};

// Tensor instructions
struct tensor_op_base_t : public op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_batch_normalization_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_batch_to_space_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_binary_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    binary_op_t binary_op;
};

struct tensor_broadcast_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_cast_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    typecode_t new_type;
};

struct tensor_celu_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_clamp_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_compare_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    compare_op_t compare_op;
};

struct tensor_concat_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_conv2d_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    pad_mode_t pad_mode;
};

struct tensor_conv2d_transpose_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    pad_mode_t pad_mode;
};

struct tensor_cum_sum_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_dequantize_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    typecode_t target_type;
};

struct tensor_elu_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_expand_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_flatten_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_gather_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_gather_nd_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_get_item_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_hard_sigmoid_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_hard_swish_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_hardmax_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_instance_normalization_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_l2_normalization_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_leaky_relu_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_log_softmax_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_lp_normalization_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_lrn_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_lstm_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    lstmdirection_t direction;
    lstmlayout_t layout;
    std::vector<std::string> activations;
};

struct tensor_mat_mul_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_normal_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    typecode_t type;
};

struct tensor_normal_like_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    typecode_t type;
};

struct tensor_one_hot_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    one_hot_mode_t one_hot_mode;
};

struct tensor_pad_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    pad_mode_t pad_mode;
};

struct tensor_prelu_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_prod_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_quant_param_of_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    quant_mode_t quant_mode;
};

struct tensor_quantize_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    typecode_t target_type;
};

struct tensor_range_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_range_of_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_reduce_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    reduce_op_t reduce_op;
};

struct tensor_reduce_arg_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    reduce_arg_op_t reduce_arg_op;
};

struct tensor_reduce_window2d_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    reduce_op_t reduce_op;
};

struct tensor_relu_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_relu6_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_require_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    std::string message;
};

struct tensor_reshape_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_resize_image_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    image_resize_mode_t resize_mode;
    image_resize_transformation_mode_t transformation_mode;
    image_resize_nearest_mode_t nearest_mode;
    bool is_tfresize;
};

struct tensor_reverse_sequence_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_select_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_selu_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_shape_of_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_sigmoid_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_size_of_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_slice_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_softmax_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_softplus_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_softsign_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_space_to_batch_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_split_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_squeeze_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_stack_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_tile_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_transpose_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_unary_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    unary_op_t unary_op;
};

struct tensor_uniform_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    typecode_t type;
};

struct tensor_uniform_like_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
    typecode_t type;
};

struct tensor_unsqueeze_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

struct tensor_where_op_t : public tensor_op_base_t
{
    tensor_function_t tensor_funct;
};

END_NS_NNCASE_RT_MODULE
