using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using Nncase.Transform.Pattern.NN;
using Nncase.Transform.Pattern.Tensors;
using static Nncase.Transform.Pattern.Utility;
using static Nncase.IR.F.NN;
using static Nncase.IR.F.Tensors;
using static Nncase.Transform.Pattern.F.Tensors;
using System.Numerics.Tensors;
using Nncase.IR;

namespace Nncase.Transform.Rule
{

    public class MatMulToConv2D : EGraphRule
    {
        MatMulWrapper matmul;
        public MatMulToConv2D()
        {
            Pattern = matmul = MatMul(IsWildCard(), IsWildCard());
        }
        public override Expr? GetRePlace(EMatchResult result)
        {
            matmul.Bind(result);
            var input_shape = matmul.Input().Shape;
            var other_shape = matmul.Other().Shape;
            var if_shape = new Shape(new[] { input_shape[0].FixedValue, input_shape[1].FixedValue, 1, 1 });
            var w_shape = new Shape(new[] { other_shape[1].FixedValue, other_shape[0].FixedValue, 1, 1 });

            var if_reshape = Reshape(matmul.Input(), if_shape);
            var w_tp = Transpose(matmul.Other(), Const.FromSpan<int>(new[] { 1, 0 }));
            var w_reshape = Reshape(w_tp, w_shape);

            return Conv2D(
              if_reshape,
              w_reshape,
              Const.FromShape(new[] { input_shape[1].FixedValue }, 0.0f),
              new DenseTensor<int>(new[] { 0, 0, 0, 0 }, new[] { 2, 2 }),
              new int[] { 1, 1 },
              new int[] { 1, 1 },
              PadMode.Constant,
              1);
        }


    }

}