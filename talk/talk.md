# Brownbag talk - Program in an old fashioned language

`C` is an old-fashioned language. It was created in the 1970s by Dennis Ritchie. The K&R book, which is often considered the bible of `C` was first published in 1978. Unlike `R`, `C` is standardized by ISO, so it has a much slower releasing pace.  Throughout the years, there were only four `C` standards released - `C90`, `C99`, `C11` and `C17`. To keep the language simple, very few language features are added. `C` programs today look pretty much the same as 20 years ago. 

Coding in `C` is like working in a constrained environment. It doesn't have a class system, proper built-in containers such as vectors, proper exception handling, leak-free resource management, easy-to-use string handling, and many more. Still, it is powerful enough to power `R` and `Python`.

So, what is the benefit of working with such a language?  First, `C` is very close to the metal. It forces you to think in a way the computer works. it encourages you to know your hardware. Second, as a compiled language with excellent compilers (e.g. `gcc`) evolved over at least 40 years, the emitted machine code is very efficient and fast to execute. Nowadays, even hand-written assembly code could rarely beat `C`. 
