using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.IO;
using System.Linq;
using Nncase.CostModel;
using Nncase.IR;
using Nncase.PatternMatch;
using Nncase.Transform;
using Xunit;
using static Nncase.IR.F.Tensors;
using static Nncase.PatternMatch.Utility;

namespace Nncase.Tests.ReWrite.FusionTest;

internal sealed class FusionCostEvaluator : Evaluator.IBaseFuncCostEvaluator
{
    public Cost VisitLeaf(BaseFunction target)
    {
        if (target is Fusion fusion && fusion.ModuleKind == Callable.StackVMModuleKind)
            return new FusionGraphCostVisitor().Visit(fusion);
        else
            throw new NotSupportedException();
    }


    private sealed class GraphOpCostEvaluateContext : Evaluator.ICostEvaluateContext
    {
        private readonly IRType? _returnType;
        private readonly IRType?[] _argumentTypes;

        public GraphOpCostEvaluateContext(IRType? returnType, IRType?[] argumentTypes)
        {
            _returnType = returnType;
            _argumentTypes = argumentTypes;
        }

        public T GetArgumentType<T>(Op op, ParameterInfo parameter)
            where T : IRType
        {
            if (op.GetType() == parameter.OwnerType)
            {
                return (T?)_argumentTypes[parameter.Index] ?? throw new InvalidOperationException("Run type infer first.");
            }
            else
            {
                throw new ArgumentOutOfRangeException($"Operator {op} doesn't have parameter: {parameter.Name}.");
            }
        }

        public T GetReturnType<T>()
            where T : IRType
        {
            return (T?)_returnType ?? throw new InvalidOperationException("Run type infer first.");
        }
    }
    private sealed class FusionGraphCostVisitor : ExprVisitor<Cost, IRType>
    {
        public override Cost VisitLeaf(Var var)
        {
            return new Cost()
            {
                [CostFactorNames.MemoryLoad] = CostUtility.GetMemoryAccess((TensorType)var.CheckedType!)
            };
        }

        public override Cost DefaultVisitLeaf(Expr expr)
        {
            return Cost.Zero;
        }

        public override Cost VisitLeaf(Call call)
        {
            Cost cost;
            if (call.Target is Op op)
            {
                var context = new GraphOpCostEvaluateContext(call.CheckedType, call.Parameters.Select(p => p.CheckedType).ToArray());
                cost = CompilerServices.EvaluateOpCost(op, context) ?? Cost.Zero;
            }
            else
                throw new NotSupportedException();
            return cost;
        }

        public override Cost VisitLeaf(Fusion fusion)
        {
            var cost = new Cost()
            {
                [CostFactorNames.MemoryStore] = CostUtility.GetMemoryAccess((TensorType)fusion.Body.CheckedType!)
            };
            cost += fusion.Parameters.Select(Visit).Sum() ?? Cost.Zero;
            return cost;
        }

    }
}

internal sealed class SingleInputFusionMergeRule : IRewriteRule
{

    public IPattern Pattern { get; } =
      IsCall(
        "caller",
        IsFusion("caller_fusion", Callable.StackVMModuleKind, IsWildcard(), IsVArgs(IsVar())),
        IsCall(
          "callee",
          IsFusion("callee_fusion", Callable.StackVMModuleKind, IsWildcard(), IsVArgs(IsVar())),
          IsWildcard("callee_input")
          )
      );

    public static Fusion MergeSingleInputFusion(Call caller, Call callee, Fusion caller_fusion, Fusion callee_fusion, RunPassOptions passOptions)
    {
        if (callee_fusion.Parameters.Count != 1 || caller_fusion.Parameters.Count != 1)
            throw new NotSupportedException("Not Support Multi Inputs Fusion Merge");

        // 1. replace the caller_fusion input_var with the callee_fusion body
        var new_fusion_body = Transform.Mutator.Substitute(e => object.ReferenceEquals(e, caller_fusion.Parameters[0]) ? callee_fusion.Body : null)().Visit(caller_fusion.Body);
        // 2. fold the store load
        // new_fusion_body = CompilerServices.Rewrite(new_fusion_body, new[] { new Transform.Rules.K510.FoldStoreLoad() }, passOptions.IndentDir("MergeSingleInputFusion"));
        var new_fusion = new Fusion($"{caller_fusion.Name}_{callee_fusion.Name}", Callable.StackVMModuleKind, new_fusion_body, callee_fusion.Parameters);

        return new_fusion;
    }

    private Dictionary<int, Call> MergedCache = new();

    public Expr? GetReplace(IMatchResult result, RunPassOptions passOptions)
    {
        var caller = (Call)result["caller"];
        var callee = (Call)result["callee"];
        var callee_fusion = (Fusion)result["callee_fusion"];
        var caller_fusion = (Fusion)result["caller_fusion"];
        // note manual pruning
        if ((caller_fusion.Name.Split("_").Length +
            caller_fusion.Name.Split("_").Length) > 7)
            return caller;

        // note each patter will generator the new expr. so need cache it.
        var hashcode = HashCode.Combine(ReferenceEqualityComparer.Instance.GetHashCode(caller), ReferenceEqualityComparer.Instance.GetHashCode(callee));
        if (!MergedCache.TryGetValue(hashcode, out var new_call))
        {
            // 1. merge new fusion
            var merged_fusion = MergeSingleInputFusion(caller, callee, caller_fusion, callee_fusion, passOptions);

            // if (true)
            // {
            new_call = new Call(merged_fusion, ImmutableArray.Create((Expr)result["callee_input"]));
            MergedCache.Add(hashcode, new_call);
        }
        else
        {
            // System.Console.WriteLine("Re Add Merged Fusion Call");
        }
        return new_call;
    }
}

/// <summary>
/// fusion_3(fusion_1(x), fusion_2(x)) => fusion_4(x)
/// </summary>
internal sealed class TwoInputFusionMergeRule : IRewriteRule
{

    private static Pattern _input = IsWildcard("input");

    public IPattern Pattern { get; } =
      IsCall(
        "caller",
        IsFusion("caller_fusion", Callable.StackVMModuleKind, IsWildcard(), IsVArgs(IsVar(), IsVar())),
        IsCall(
          "lhs_callee",
          IsFusion("lhs_callee_fusion", Callable.StackVMModuleKind, IsWildcard(), IsVArgs(IsVar())),
          _input
          ),
        IsCall(
          "rhs_callee",
          IsFusion("rhs_callee_fusion", Callable.StackVMModuleKind, IsWildcard(), IsVArgs(IsVar())),
          _input
          )
      );

    public static Fusion MergeTwoInputFusion(Call caller, Call lhs_callee, Call rhs_callee, Fusion caller_fusion, Fusion lhs_callee_fusion, Fusion rhs_callee_fusion, RunPassOptions passOptions)
    {
        // 1. replace the caller_fusion input_var with the callee_fusion body
        var new_fusion_body = Transform.Mutator.Substitute(e =>
        {
            if (object.ReferenceEquals(e, caller_fusion.Parameters[0]))
                return lhs_callee_fusion.Body;
            if (object.ReferenceEquals(e, caller_fusion.Parameters[1]))
                return rhs_callee_fusion.Body;
            return null;
        })().Visit(caller_fusion.Body);

        // 2. fold the store load
        // new_fusion_body = CompilerServices.Rewrite(new_fusion_body, new[] { new Transform.Rules.K510.FoldStoreLoad() }, passOptions.IndentDir("MergeSingleInputFusion"));
        var new_fusion = new Fusion($"{caller_fusion.Name}_{lhs_callee_fusion.Name}_{rhs_callee_fusion.Name}", Callable.StackVMModuleKind, new_fusion_body, lhs_callee_fusion.Parameters);

        return new_fusion;
    }

    private Dictionary<int, Call> MergedCache = new();

    private Expr? GetReplace(Call caller, Call lhs_callee, Call rhs_callee, Fusion caller_fusion, Fusion lhs_callee_fusion, Fusion rhs_callee_fusion, Expr input, RunPassOptions passOptions)
    {
        // note manual pruning
        if ((caller_fusion.Name.Split("_").Length +
          lhs_callee_fusion.Name.Split("_").Length +
          rhs_callee_fusion.Name.Split("_").Length) > 7)
            return caller;
        // note each patter will generator the new expr. so need cache it.
        var hashcode = HashCode.Combine(ReferenceEqualityComparer.Instance.GetHashCode(caller_fusion),
                                        ReferenceEqualityComparer.Instance.GetHashCode(lhs_callee_fusion),
                                        ReferenceEqualityComparer.Instance.GetHashCode(rhs_callee_fusion));
        if (!MergedCache.TryGetValue(hashcode, out var new_call))
        {
            // 1. merge new fusion
            var merged_fusion = MergeTwoInputFusion(caller, lhs_callee, rhs_callee, caller_fusion, lhs_callee_fusion, rhs_callee_fusion, passOptions);
            new_call = new Call(merged_fusion, ImmutableArray.Create(input));
            MergedCache.Add(hashcode, new_call);
        }
        else
        {
            // System.Console.WriteLine("Re Add Merged Two Fusion Call");
        }
        return new_call;
    }

    public Expr? GetReplace(IMatchResult result, RunPassOptions options)
    {
        return GetReplace(
          (Call)result["caller"], (Call)result["lhs_callee"], (Call)result["rhs_callee"], (Fusion)result["caller_fusion"], (Fusion)result["lhs_callee_fusion"], (Fusion)result["rhs_callee_fusion"], (Expr)result["input"],
          options
        );
    }
}

public class UnitTestEGraphFusion : TestFixture.UnitTestFixtrue
{
    [Fact]
    public async void TestResNet18Fusion()
    {
        var passOptions = GetPassOptions();
        var compileOptions = passOptions.CompileOptions;

        var target = CompilerServices.GetTarget(compileOptions.Target);

        // step 1. import
        var input = new Var("input", new TensorType(DataTypes.Float32, new int[] { 1, 3, 224, 224 }));
        var model = new ResNet(typeof(BasicBlock), new[] { 2, 2, 2, 2 });
        var body = model.Forward(input);
        var main = new Function("main", body, ImmutableArray.Create(input));
        IRModule module = new(main);

        CompilerServices.InferenceType(main);
        CompilerServices.DumpIR(main, "", passOptions.DumpDir);

        var pass = new EGraphPass("AutoMergeFusion", new FusionCostEvaluator()){
          new SingleInputFusionMergeRule()
        };
        await pass.RunAsync(main, passOptions);
    }

    [Fact]
    public async void TestResNet18FusionWithCycle()
    {
        var passOptions = GetPassOptions();
        var compileOptions = passOptions.CompileOptions;

        var target = CompilerServices.GetTarget(compileOptions.Target);

        // step 1. import
        var input = new Var("input", new TensorType(DataTypes.Float32, new int[] { 1, 3, 224, 224 }));
        var model = new ResNet(typeof(BasicBlock), new[] { 2, 2, 2, 2 });
        var body = model.Forward(input);
        var main = new Function("main", body, ImmutableArray.Create(input));
        IRModule module = new(main);

        CompilerServices.InferenceType(main);
        CompilerServices.DumpIR(main, "", passOptions.DumpDir);

        var pass = new EGraphPass("AutoMergeFusion", new FusionCostEvaluator()){
          new SingleInputFusionMergeRule(),
          new TwoInputFusionMergeRule(),
        };
        await pass.RunAsync(main, passOptions);
    }


    /// <summary>
    /// cycle type 1:  这里会存在一个bug, 如果是dataflow的话, merge single input 就会把 x y 合并在一起. 需要知道use关系才行.
    ///             x = fusion1(input)
    ///            /    \
    ///         /         \
    ///        |      y = fusion2(x)
    ///         \        /
    ///          \     /
    ///     fusion3(x,y)
    /// </summary>    
    [Fact]
    public async void TestDataFlowFusionCycleFailedCase()
    {

        var passOptions = GetPassOptions();
        var compileOptions = passOptions.CompileOptions;

        var target = CompilerServices.GetTarget(compileOptions.Target);

        // step 1. import
        var input = new Var("input", new TensorType(DataTypes.Float32, new int[] { 1, 224, 224, 3 }));
        Function main;
        {
            var fusion_1_input = new Var("fusion_1_input", new TensorType(DataTypes.Float32, new int[] { 1, 224, 224, 3 }));
            var fusion_1 = new Fusion("fusion_1", Callable.StackVMModuleKind, IR.F.Tensors.NHWCToNCHW(fusion_1_input), new[] { fusion_1_input });
            var v_0 = new Call(fusion_1, input); // 1,3,224,224

            var fusion_2_input = new Var("fusion_2_input", new TensorType(DataTypes.Float32, new int[] { 1, 3, 224, 224 }));
            var fusion_2 = new Fusion("fusion_2", Callable.StackVMModuleKind, IR.F.NN.ReduceWindow2D(ReduceOp.Max, fusion_2_input, 0.0f, new[] { 3, 3 }, new[] { 2, 2 }, new[,] { { 1, 1 }, { 1, 1 } }, new[] { 1, 1 }, false, false), new[] { fusion_2_input });
            var v_1 = new Call(fusion_2, v_0); // 1,3,112,112

            var fusion_3_input = new Var("fusion_3_input", new TensorType(DataTypes.Float32, new int[] { 1, 3, 112, 112 }));
            var fusion_3 = new Fusion("fusion_3", Callable.StackVMModuleKind, IR.F.NN.ReduceWindow2D(ReduceOp.Mean, fusion_3_input, 0.0f, new[] { 3, 3 }, new[] { 1, 1 }, new[,] { { 1, 1 }, { 1, 1 } }, new[] { 1, 1 }, false, false), new[] { fusion_3_input });
            var v_2 = new Call(fusion_3, v_1); // 1,3,112,112

            var fusion_4_input = new Var[] { new("fusion_4_input_0", new TensorType(DataTypes.Float32, new int[] { 1, 3, 112, 112 })), new("fusion_4_input_1", new TensorType(DataTypes.Float32, new int[] { 1, 3, 112, 112 })) };
            var fusion_4 = new Fusion("fusion_4", Callable.StackVMModuleKind, IR.F.Math.Add(fusion_4_input[0], fusion_4_input[1]), fusion_4_input);
            var v_3 = new Call(fusion_4, new[] { v_1, v_2 }); // 1,3,112,112
            main = new Function("main", v_3, ImmutableArray.Create(input));
        }

        IRModule module = new(main);

        CompilerServices.InferenceType(main);
        CompilerServices.DumpIR(main, "", passOptions.DumpDir);

        var pass = new DataflowPass("AutoMergeFusion"){
          new SingleInputFusionMergeRule()
        };
        var post = (Function)await pass.RunAsync(main, passOptions);
        CompilerServices.DumpDotIR(post, "", passOptions.DumpDir, false);

        var pass2 = new EGraphPass("EGraphAutoMergeFusion", new FusionCostEvaluator()){
          new SingleInputFusionMergeRule()
        };
        var post2 = (Function)await pass2.RunAsync(main, passOptions);
        CompilerServices.DumpDotIR(post2, "", passOptions.DumpDir, false);

        var input_tensor = TestFixture.Testing.Rand<float>(1, 224, 224, 3);
        var feed_dict = new Dictionary<Var, IValue>(ReferenceEqualityComparer.Instance){
          {input, Value.FromTensor(input_tensor)}
        };
        var pre_result = CompilerServices.Evaluate(main.Body, feed_dict);
        var post_result = CompilerServices.Evaluate(post.Body, feed_dict);
        var post2_result = CompilerServices.Evaluate(post2.Body, feed_dict);
        // note 这里他其实强行分成了两个分支, fusion_1_2_3 和 fusion_2_fusion_1, 虽然结果一致但是不合理.
        Assert.True(TestFixture.Comparator.AllEqual(pre_result, post_result));
        Assert.True(TestFixture.Comparator.AllEqual(pre_result, post2_result));
    }

}