# IncrementalPTXJIT
Incremental just-in-time compiler for nvidia ptx based on cling.

# Usage
It's works like a simple version of cling. It's starts an interactive shell season. You can type in code line by line or as block command. The code will be immediately compiled. There are also some special functions, to load code from file or store compiled code to file.

Use the following to get all functions:
```bash
incrementalptxjit --help
```
