﻿// Copyright (c) Canaan Inc. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Nncase.Evaluator;
using Nncase.Transform;

namespace Nncase.Quantization;

/// <summary>
/// the quantization egraph pass.
/// </summary>
public class EGraphPassWithAdaRound : EGraphPass
{
    /// <inheritdoc/>
    protected override async Task OnPostRewriteAsync(EGraph graph, RunPassContext options)
    {
        var quantizerAdaRound = new QuantizerAdaRound(graph, CompileSession);
        await quantizerAdaRound.RunAsync();
    }
}
