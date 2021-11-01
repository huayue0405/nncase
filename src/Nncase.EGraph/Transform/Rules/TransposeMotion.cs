using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using Nncase.IR;
using Nncase.IR.Math;
using Nncase.Transform.Pattern;
using static Nncase.Transform.Pattern.F.Tensor;
using static Nncase.Transform.Pattern.Utility;
using static Nncase.IR.F.Tensors;

namespace Nncase.Transform.Rule
{
    public class TransposeBinaryMotion : EGraphRule
    {
        private WildCardPattern wx = "x", wy = "y", wperm = "perm";
        private WildCardPattern wbin = new();

        public override ExprPattern GetPattern()
        {
            return IsBinary(Transpose(wx, wperm), Transpose(wy, wperm));
        }

        public override Expr GetRePlace(EMatchResult result)
        {
            Expr x = result.GetExpr(wx), y = result.GetExpr(wy), perm = result.GetExpr(wperm);
            var bin = (Binary)result.Root.Expr;
            return Transpose(new Call(new Binary(bin.BinaryOp), x, y), perm);
        }
    }

    public class TransposeConstantBinaryMotion : EGraphRule
    {
        protected WildCardPattern wx = "x";
        protected ConstPattern wperm = IsConstTensor(), wcon = IsConst();

        protected Expr x;
        protected Const con, perm;
        protected Binary binary;

        protected Shape newShape;
        protected Const newCon;

        public Shape GetNewConstShape(Const oldCon, Const oldPerm)
        {
            var perm = oldPerm.ToTensor<int>();
            var expand_dim = perm.Length - perm[(int)perm.Length - 1] - 1;
            var shape = oldCon.Shape;
            if (shape[0] != 1)
            {
                for (int i = 0; i < expand_dim; i++)
                    shape.Add(1);
            }
            return new Shape(shape);
        }

        public void ParserResult(EMatchResult result)
        {
            x = result.GetExpr(wx);
            con = (Const)result.GetExpr(wcon);
            perm = (Const)result.GetExpr(wperm);
            binary = (Binary)result.GetRoot();
            newShape = GetNewConstShape((Const)con, (Const)perm);
            newCon = con with { ValueType = con.ValueType with { Shape = newShape } };
        }
    }

    public class TransposeConstantBinaryMotionLeft : TransposeConstantBinaryMotion
    {
        public override ExprPattern GetPattern()
        {
            return IsBinary(Transpose(wx, wperm), wcon);
        }

        public override Expr GetRePlace(EMatchResult result)
        {
            ParserResult(result);
            return Transpose(new Call(new Binary(binary.BinaryOp), x, newCon), perm);
        }
    }

    public class TransposeConstantBinaryMotionRight : TransposeConstantBinaryMotion
    {
        public override ExprPattern GetPattern()
        {
            return IsBinary(wcon, Transpose(wx, wperm));
        }

        public override Expr GetRePlace(EMatchResult result)
        {
            ParserResult(result);
            return Transpose(new Call(new Binary(binary.BinaryOp), newCon, x), perm);
        }
    }


    public class TransposeConcatMotion : EGraphRule
    {

        private class Comparer
        {
            private Expr? last_const_ = null;
            public bool Cond(Const con)
            {
                if (last_const_ is null)
                    last_const_ = con;
                return last_const_ == con;
            }
        }
        private Func<Const, bool> permCond = new(new Comparer().Cond);
        List<WildCardPattern> wcinputs = new();
        ConstPattern wcprem, wcaxis;

        public TransposeConcatMotion()
        {
            wcprem = IsConstTensor();
            wcaxis = IsConstScalar();
        }

        public override ExprPattern GetPattern()
        {
            return Concat(IsTuple(IsVArgsRepeat((n, param) =>
             {
                 for (int i = 0; i < n; i++)
                 {
                     var wcin = IsWildCard();
                     param.Add(Transpose(wcin, wcprem));
                 }
             }
            )), wcaxis);
        }

        public override Expr GetRePlace(EMatchResult result)
        {
            var newShapes = (from input in wcinputs select GetShape(result.GetExpr(input))).ToArray();
            var oldPerm = result.GetExpr<Const>(wcprem);
            var permt = oldPerm.ToTensor<int>();
            var oldAxis = result.GetExpr<Const>(wcaxis).ToScalar<int>();
            var newAxis = permt[oldAxis];

            var newCon = Concat(new IR.Tuple((from input in wcinputs select result.GetExpr(input)).ToArray()), newAxis);
            var newTran = Transpose(newCon, oldPerm);
            return newTran;
        }

    }

    public class TransPosePadMotion : EGraphRule
    {
        WildCardPattern wcin = "input";
        List<ConstPattern> wcpads = new();

        ConstPattern wcmode = IsConstScalar(), wcpadv = IsConstScalar(), wcperm = IsConstScalar();

        public override ExprPattern GetPattern()
        {
            return Transpose(Pad(wcin, IsTuple(IsVArgsRepeat(
              (n, param) =>
              {
                  for (int i = 0; i < n; i++)
                  {
                      var pad = IsConstTensor();
                      wcpads.Add(pad);
                      param.Add(pad);
                  }
              },
              (match, param) =>
              {
                  if (!match)
                  {
                      wcpads.Clear();
                      param.Clear();
                  }
              }
            )), wcmode, wcpadv), wcperm);
        }

        public override Expr GetRePlace(EMatchResult result)
        {
            var input = result.GetExpr(wcin);
            var (mode, padv, perm) = result.GetExpr(wcmode, wcpadv, wcperm);
            var newPads = perm.ToTensor<int>().Select(i => (Expr)result.GetExpr(wcpads[i])).ToImmutableArray();
            return Pad(Transpose(input, perm), new IR.Tuple(newPads), mode, padv);
        }
    }

    public class TransposeReduceMotion : EGraphRule
    {

        // public override ExprPattern GetPattern()
        // {

        // }
    }

}