// Copyright (c) Canaan Inc. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.

using System;
using Nncase.CostModel;
using Nncase.IR;
using Nncase.IR.Math;
using OrtKISharp;

namespace Nncase.Evaluator.Math;

/// <summary>
/// Evaluator for <see cref="Binary"/>.
/// </summary>
public partial class BinaryEvaluator : IEvaluator<Binary>, ITypeInferencer<Binary>, ICostEvaluator<Binary>, IOpPrinter<Binary>
{
    /// <inheritdoc />
    public IValue Visit(IEvaluateContext context, Binary binary)
    {
        var a = context.GetOrtArgumentValue(binary, Binary.Lhs);
        var b = context.GetOrtArgumentValue(binary, Binary.Rhs);
        return (binary.BinaryOp switch
        {
            BinaryOp.Add => a + b,
            BinaryOp.Sub => a - b,
            BinaryOp.Mul => a * b,
            BinaryOp.Div => a / b,
            BinaryOp.Mod => a % b,
            BinaryOp.Min => OrtKI.Min(new[] { a, b }),
            BinaryOp.Max => OrtKI.Max(new[] { a, b }),
            BinaryOp.Pow => OrtKI.Pow(a, b),
            BinaryOp.BitwiseAnd => throw new NotSupportedException("NotSupported BinaryOp BitwiseAnd"),
            BinaryOp.BitwiseOr => throw new NotSupportedException("NotSupported BinaryOp BitwiseOr"),
            BinaryOp.BitwiseXor => throw new NotSupportedException("NotSupported BinaryOp BitwiseXor"),
            BinaryOp.LogicalAnd => OrtKI.And(a, b),
            BinaryOp.LogicalOr => OrtKI.Or(a, b),
            BinaryOp.LogicalXor => OrtKI.Xor(a, b),
            BinaryOp.LeftShift => OrtKI.LeftShift(a, b),
            BinaryOp.RightShift => OrtKI.RightShift(a, b),
            _ => throw new ArgumentOutOfRangeException(nameof(binary.BinaryOp)),
        }).ToValue();
    }

    /// <inheritdoc/>
    public IRType Visit(ITypeInferenceContext context, Binary target)
    {
        var lhs = context.CheckArgumentType<TensorType>(target, Binary.Lhs);
        var rhs = context.CheckArgumentType<TensorType>(target, Binary.Rhs);
        return Visit(target, lhs, rhs);
    }

    /// <inheritdoc/>
    public Cost Visit(ICostEvaluateContext context, Binary target)
    {
        var lhsType = context.GetArgumentType<TensorType>(target, Binary.Lhs);
        var rhsType = context.GetArgumentType<TensorType>(target, Binary.Rhs);
        var outputType = context.GetReturnType<TensorType>();

        return new()
        {
            [CostFactorNames.MemoryLoad] = CostUtility.GetMemoryAccess(lhsType) + CostUtility.GetMemoryAccess(rhsType),
            [CostFactorNames.MemoryStore] = CostUtility.GetMemoryAccess(outputType),
            [CostFactorNames.CPUCycles] = CostUtility.GetCPUCycles(outputType, CostUtility.GetCPUCyclesOfBinary(target.BinaryOp)),
        };
    }

    /// <inheritdoc/>
    public string Visit(IIRPrinterContext context, Binary target, bool ILmode)
    {
        var lhs = context.GetArgument(target, Binary.Lhs);
        var rhs = context.GetArgument(target, Binary.Rhs);
        if (ILmode)
            return $"{target.BinaryOp}({lhs}, {rhs})";
        return target.BinaryOp switch
        {
            BinaryOp.Add => $"({lhs} + {rhs})",
            BinaryOp.Sub => $"({lhs} - {rhs})",
            BinaryOp.Mul => $"({lhs} * {rhs})",
            BinaryOp.Div => $"({lhs} / {rhs})",
            BinaryOp.Mod => $"({lhs} % {rhs})",
            BinaryOp.LogicalAnd => $"({lhs} & {rhs})",
            BinaryOp.LogicalOr => $"({lhs} | {rhs})",
            BinaryOp.LogicalXor => $"({lhs} ^ {rhs})",
            BinaryOp.LeftShift => $"({lhs} << {rhs})",
            BinaryOp.RightShift => $"({lhs} >> {rhs})",
            _ => $"{target.BinaryOp}({lhs}, {rhs})"
        };
    }

    private IRType Visit(Binary target, TensorType lhs, TensorType rhs)
    {
        if (target.BinaryOp is BinaryOp.LeftShift or BinaryOp.RightShift && (lhs.DType != DataTypes.UInt32 || rhs.DType != DataTypes.UInt32))
            return new InvalidType("The Binary LeftShift RightShift Only Accept The UInt32 Datatype.");
        if (target.BinaryOp is BinaryOp.Sub && (lhs.DType == DataTypes.UInt32 || rhs.DType == DataTypes.UInt32))
            return new InvalidType("The Binary Sub Only Accept The UInt32 Datatype.");
        if (lhs is { DType: PointerType { ElemType: var letype } } && rhs is { DType: PointerType { ElemType: var retype } })
        {

            if (letype == retype)
                return TensorType.Pointer(letype);
            else
                return new InvalidType($"The Binary Lhs {CompilerServices.Print(lhs)} != Rhs {CompilerServices.Print(rhs)}");
        }

        if (lhs is { DType: PointerType { ElemType: var lt } } && rhs.DType == DataTypes.Int32)
        {
            return TensorType.Pointer(lt);
        }

        if (lhs.DType == DataTypes.Int32 && rhs is { DType: PointerType { ElemType: var rt } })
        {
            return TensorType.Pointer(rt);
        }

        return TypeInference.BroadcastType(lhs, rhs);
    }
}
