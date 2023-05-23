<h1>Assembler Project</h1>

<p>This is the readme file for the "Assembler" project developed for the university. The project is implemented in the C programming language and aims to provide an assembler for a specific architecture.</p>

<h2>Introduction</h2>

<p>The Assembler project is designed to convert assembly language programs into machine code for a specific architecture. It takes assembly code as input and generates the corresponding machine code that can be executed on the target architecture.</p>

<p>The project is implemented in C programming language and follows a modular approach for easy understanding and extensibility. It aims to provide a flexible and efficient solution for assembling assembly language programs.</p>

<h2>Features</h2>

<p>The Assembler project offers the following features:</p>

<ol>
  <li><strong>Parsing</strong>: The assembler parses assembly code to identify instructions, labels, and operands.</li>
  <li><strong>Symbol Table</strong>: It maintains a symbol table to keep track of labels and their corresponding addresses.</li>
  <li><strong>Code Generation</strong>: The assembler generates machine code by translating assembly instructions and resolving addresses.</li>
  <li><strong>Error Handling</strong>: It provides comprehensive error handling and reporting for syntax errors, undefined labels, and other issues.</li>
  <li><strong>Modularity</strong>: The codebase is designed with modularity in mind, allowing easy extensibility and future improvements.</li>
</ol>

<h2>Installation</h2>

<p>To install and set up the Assembler project, follow these steps:</p>

<ol>
  <li>Clone the repository: <code>git clone &lt;https://github.com/itaybroder/Assembler-in-C&gt;</code></li>
  <li>Navigate to the project directory: <code>cd Assembler-in-C</code></li>
  <li>Compile the source code: <code>gcc -o assembler main.c assembler.c</code></li>
  <li>The assembler executable will be generated.</li>
</ol>

<h2>Usage</h2>

<p>To use the Assembler project, follow these steps:</p>

<ol>
  <li>Prepare your assembly code in a text file, e.g., <code>input.asm</code>.</li>
  <li>Run the assembler executable: <code>./assembler input.asm</code></li>
  <li>The assembler will process the input file and generate the corresponding machine code.</li>
  <li>If there are any errors or warnings, they will be displayed on the console.</li>
  <li>The generated machine code can be found in the output file, which will have the same name as the input file but with a different extension, e.g., <code>input.obj</code>.</li>
</ol>

<h2>Examples</h2>

<p>Here are a few examples to demonstrate the usage of the Assembler project:</p>

<pre><code>; Example 1: Hello World
section .data
    message db "Hello, World!",0

section .text
    global _start

_start:
    ; write the message to stdout
    mov eax, 4
    mov ebx, 1
    mov ecx, message
    mov edx, 13
    int 0x80

    ; exit the program
    mov eax, 1
    xor ebx, ebx
    int 0x80
</code></pre>

<p>Save the above code in a file named <code>hello.asm</code> and assemble it using the Assembler project:</p>

<pre><code>./assembler hello.asm</code></pre>

<p>The generated machine code will be stored in the <code>hello.obj</code> file.</p>