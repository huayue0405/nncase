using System;
using Nncase.IR;

namespace Nncase.Pattern
{
    public interface IMatchResult
    {
        public Expr this[ExprPattern pattern] { get; }
        public T GetExpr<T>(ExprPattern expr) where T : Expr;
        public Expr GetRoot();
        public T GetRoot<T>() where T : Expr;
        public Var this[VarPattern pat] => GetExpr<Var>(pat);
        public Const this[ConstPattern pat] => GetExpr<Const>(pat);
        public Function this[FunctionPattern pat] => GetExpr<Function>(pat);
        public Call this[CallPattern pat] => GetExpr<Call>(pat);
        public IR.Tuple this[TuplePattern pat] => GetExpr<IR.Tuple>(pat);

        public (Expr, Expr) this[ExprPattern pat1, ExprPattern pat2] => (this[pat1], this[pat2]);
        public (Expr, Expr, Expr) this[ExprPattern pat1, ExprPattern pat2, ExprPattern pat3] => (this[pat1], this[pat2], this[pat3]);
        public (Expr, Expr, Expr, Expr) this[ExprPattern pat1, ExprPattern pat2, ExprPattern pat3, ExprPattern pat4] => (this[pat1], this[pat2], this[pat3], this[pat4]);

        public (Const, Const) this[ConstPattern pat1, ConstPattern pat2] => (this[pat1], this[pat2]);
        public (Const, Const, Const) this[ConstPattern pat1, ConstPattern pat2, ConstPattern pat3] => (this[pat1], this[pat2], this[pat3]);
    }
}