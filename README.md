# IncrementalPTXJIT
Incremental just-in-time compiler for nvidia ptx based on [cling](https://github.com/root-project/cling).

The compiler consists of three parts: Prompt, Frontend and Backend.

* **Prompt:** The prompt is based on the concept of the cling prompt, but offers reduced functionality.
* **Frontend:** The frontend based on a [modifacted version](https://github.com/SimeonEhrig/cling/tree/refactorCudaPTXJIT) of cling. It's translate the input (the special c++ syntax of cling is allowed) into llvm IR. The frontend is provided by the `libcling.so`.
* **Backend:** The backend is based on this [tutorial](https://llvm.org/docs/tutorial/LangImpl08.html). It translates the llvm IR in the form of a `llvm::Module` into a PTX file. 

# Usage
It's works like a simple version of cling. It's starts an interactive shell season. You can enter the code line by line or as a block command[1]. The code is compiled immediately. There are also some special functions. They are devide into two groups.

1. Arguments which can be used at startup.
2. Special commands which begin with a dot within the shell.

Use the following command to get all functions:
```bash
incrementalptxjit --help
```

[1] Block commands are a little tricky. The block have to be opened on one line with braceleft and closed on another line with braceright.

# Example

A short example, how the IncrementalPTXJIT can be used.

```bash
$ > ./incrementalptxjit
Disabling gnu++: clang has no __float128 support on this target!
atexit not in Module!
at_quick_exit not in Module!
> __device__ int foo(){
   return 3; 
   }
> .p
//
// Generated by LLVM NVPTX Back-End
//

.version 3.2
.target sm_20
.address_size 64




> __global__ void bar(){
   int i = foo();
   }
> .p
//
// Generated by LLVM NVPTX Back-End
//

.version 3.2
.target sm_20
.address_size 64

	// .globl	_Z3barv
.weak .func  (.param .b32 func_retval0) _Z3foov
()
;

.visible .entry _Z3barv()
{
	.reg .b32 	%r<2>;

	{ // callseq 0, 0
	.reg .b32 temp_param_reg;
	.param .b32 retval0;
	call.uni (retval0), 
	_Z3foov, 
	(
	);
	ld.param.b32 	%r1, [retval0+0];
	} // callseq 0
	ret;
}

	// .weak	_Z3foov
.weak .func  (.param .b32 func_retval0) _Z3foov()
{
	.reg .b32 	%r<2>;

	mov.u32 	%r1, 3;
	st.param.b32 	[func_retval0+0], %r1;
	ret;
}



> .f kernel.ptx
> .q    	
$ > cat kernel.ptx
//
// Generated by LLVM NVPTX Back-End
//

.version 3.2
.target sm_20
.address_size 64

	// .globl	_Z3barv
.weak .func  (.param .b32 func_retval0) _Z3foov
()
;

.visible .entry _Z3barv()
{
	.reg .b32 	%r<2>;

	{ // callseq 1, 0
	.reg .b32 temp_param_reg;
	.param .b32 retval0;
	call.uni (retval0), 
	_Z3foov, 
	(
	);
	ld.param.b32 	%r1, [retval0+0];
	} // callseq 1
	ret;
}

	// .weak	_Z3foov
.weak .func  (.param .b32 func_retval0) _Z3foov()
{
	.reg .b32 	%r<2>;

	mov.u32 	%r1, 3;
	st.param.b32 	[func_retval0+0], %r1;
	ret;
}
$ >
```
