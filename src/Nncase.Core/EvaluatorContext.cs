using System;
using System.Collections.Generic;
using Nncase.Evaluator;
using Nncase.IR;
using TorchSharp;

namespace Nncase
{
    public sealed class EvaluatorContext
    {
        public Call? CurrentCall;
        private readonly Dictionary<Expr, Const> _exprMemo;

        public readonly Dictionary<Var, Const> Inputs;

        public EvaluatorContext(Dictionary<Expr, Const> exprMemo, Dictionary<Var, Const> inputs)
        {
            _exprMemo = exprMemo;
            Inputs = inputs;
        }

        private Call GetCurrentCall() =>
            CurrentCall ?? throw new InvalidOperationException("Current call is not set in evaluator.");

        public torch.Tensor GetTorchArgument(Op op, ParameterInfo parameter)
        {
            return GetArgumentConst(op, parameter).ToTorchTensor();
        }

        public torch.Tensor GetTorchArgument(Expr expr)
        {
            return GetArgument(expr).ToTorchTensor();
        }
        
        public Tensorflow.Tensor GetTFArgument(Op op, ParameterInfo parameter)
        {
            return GetArgumentConst(op, parameter).ToTFTensor();
        }

        public Const GetArgument(Expr expr)
        {
            return _exprMemo[expr];
        }

        public Expr GetArgumentExpr(Op op, ParameterInfo parameter)
        {
            if (op.GetType() == parameter.OwnerType)
            {
                return GetCurrentCall().Parameters[parameter.Index];
            }
            else
            {
                throw new ArgumentOutOfRangeException($"Operator {op} doesn't have parameter: {parameter.Name}.");
            }
        }

        public T GetArgumentConstScalar<T>(Op op, ParameterInfo parameter)
            where T : unmanaged
        {
            return GetArgumentConst(op, parameter).ToScalar<T>();
        }

        public T[] GetArgumentConstArray<T>(Op op, ParameterInfo parameter)
            where T : unmanaged
        {
            return GetArgumentConst(op, parameter).ToArray<T>();
        }

        public Const GetArgumentConst(Op op, ParameterInfo parameter)
        {
            var expr = GetArgumentExpr(op, parameter);
            if (expr is Const constValue)
            {
                return constValue;
            }
            else
            {
                // maybe a valid type but not const
                return GetArgument(expr);
            }
        }

        public TensorType GetTensorType(Expr expr)
        {
            var resultType = expr.CheckedType ?? throw new InvalidOperationException($"Expr {expr} don't have CheckedType.");
            return resultType is TensorType resultTensorType ?
                resultTensorType :
                throw new InvalidOperationException($"Expr {expr} is not a TensorType.");
        }

        public TensorType CurrentCallResultTensorType() => GetTensorType(CurrentCall!);

    }
}