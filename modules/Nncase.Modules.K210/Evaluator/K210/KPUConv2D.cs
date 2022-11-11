﻿using System.Numerics.Tensors;
using NetFabric.Hyperlinq;
using Nncase.IR;
using Nncase.IR.K210;
using Nncase.TIR;
using OrtKISharp;
using Nncase.Evaluator;
using Nncase.IR.Math;
using Range = Nncase.TIR.Range;

namespace Nncase.Evaluator.K210;

public class KPUConv2DEvaluator:IEvaluator<KPUConv2D>, ITypeInferencer<KPUConv2D>
{
    public IValue Visit(IEvaluateContext context, KPUConv2D target)
    {
        var input = context.GetOrtArgumentValue(target, KPUConv2D.Input);
        var weights = context.GetOrtArgumentValue(target, KPUConv2D.Weights);
        var batchNorms = context.GetOrtArgumentValue(target, KPUConv2D.BatchNorms).ToValue();
        var activition = context.GetOrtArgumentValue(target, KPUConv2D.OutputQuantParam).ToTensor();

        var stride = new long[] { 1, 1 };
        var pad = Enumerable.Repeat((long)KPUUtility.GetKPUPadding(target.FilterType), 4).ToArray();
        var dilation = new long[] { 1, 1 };
        //var clamp = new long[] {1, 1};
        var groups = 1L;
        //var kernelShape = weights.Shape;
        var result = OrtKI.Conv(
            input.ToType(OrtDataType.Float), weights.ToType(OrtDataType.Float), EvaluatorUtil.DefaultBias(batchNorms, weights.Shape[0]).ToType(OrtDataType.Float),
            "NOTSET", dilation.ToArray(),
            groups, new long[] { weights.Shape[2], weights.Shape[3] }, EvaluatorUtil.ToOnnxPadFormat(pad), stride.ToArray());
        if (batchNorms != Value.None)
        {
            result = result + batchNorms.AsTensor().ToOrtTensor();
        }
        return EvaluatorUtil.Act(result.ToTensor(), activition, new[] { float.MinValue, float.MaxValue});
    }

    /// <inheritdoc/>
    public IRType Visit(ITypeInferenceContext context, KPUConv2D target)
    {
        var input = context.CheckArgumentType<TensorType>(target, KPUConv2D.Input);
        var weights = context.CheckArgumentType<TensorType>(target, KPUConv2D.Weights);
        return Visit(context, target, input, weights);
    }

    private IRType Visit(ITypeInferenceContext context, KPUConv2D target, TensorType input, TensorType weights)
    {
        var stride = new[] { 1, 1 };
        var pad = Tensor.FromScalar(KPUUtility.GetKPUPadding(target.FilterType), new[] { 2, 2 });
        var dilation = new[] { 1, 1 };
        var groups = 1;
        return TypeInference.Conv2DType(input, weights, stride, pad, dilation, groups);
    }

}