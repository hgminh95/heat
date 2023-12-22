#!/bin/bash

bazel build --config opt heat:heat

set -x
set -e
set -o pipefail

llvm-objdump --disassemble-symbols=__ZN4heat4Empty7RunImplEv -M intel bazel-bin/heat/heat
llvm-objdump --disassemble-symbols=__ZN4heat4Simple7RunImplEv -M intel bazel-bin/heat/heat
llvm-objdump --disassemble-symbols=__ZN4heat4Math7RunImplEv -M intel bazel-bin/heat/heat
llvm-objdump --disassemble-symbols=__ZN4heat4Simd7RunImplEv -M intel bazel-bin/heat/heat

bazel-bin/heat/heat --method empty cores 4 --bm 2>&1
bazel-bin/heat/heat --method simple cores 4 --bm 2>&1
bazel-bin/heat/heat --method math cores 4 --bm 2>&1
bazel-bin/heat/heat --method simd cores 4 --bm 2>&1

