# 🚀 42-Pipex

<div align="center">
  <h1>🔗 Pipex: Recreating Shell Pipes in C</h1>
  <p><em>"Building the backbone of command chaining."</em></p>
</div>

<div align="center">
  <img alt="Code size" src="https://img.shields.io/github/languages/code-size/jdecorte-be/42-Pipex?color=lightblue" />
  <img alt="Lines of code" src="https://img.shields.io/tokei/lines/github/jdecorte-be/42-Pipex?color=critical" />
  <img alt="Languages used" src="https://img.shields.io/github/languages/count/jdecorte-be/42-Pipex?color=yellow" />
  <img alt="Top language" src="https://img.shields.io/github/languages/top/jdecorte-be/42-Pipex?color=blue" />
  <img alt="Last commit" src="https://img.shields.io/github/last-commit/jdecorte-be/42-Pipex?color=green" />
</div>

---

## 🌟 What is Pipex?

**Pipex** is a project that replicates the behavior of shell pipes (`|`) in C. It allows you to chain commands by redirecting the output of one command to the input of another, just like in the Unix shell.

### Example:

```bash
# Shell Command
ls -l | grep txt > outfile

# Pipex Equivalent
./pipex infile "ls -l" "grep txt" outfile
```

With **Pipex**, you can:
- Redirect input/output between processes.
- Execute multiple commands seamlessly.
- Simulate the shell piping mechanism using **C**.

---

## 🛠️ How it Works

### Core Concepts

1. **Pipes (`pipe()`)**:
   Pipes create a communication channel between two processes. The output of one process (write end) becomes the input of another (read end).

   ```c
   int end[2];
   pipe(end); // Creates a pipe with two ends: end[0] (read) and end[1] (write).
   ```

2. **Fork (`fork()`)**:
   Creates a new process (child process). The parent and child processes can run simultaneously, sharing the pipe.

3. **File Descriptors (FDs)**:
   Redirect input/output using **dup2()** to manipulate the file descriptors for `stdin`, `stdout`, and pipe ends.

4. **Execve (`execve()`)**:
   Executes a new program within the current process. It replaces the current process with the specified command.

---

### 🧩 Pipex Step-by-Step

1. **Setup the Pipe:**

   ```c
   void pipex(int f1, int f2) {
       int end[2];
       pipe(end); // Initialize the pipe
   }
   ```

   - `end[0]`: The read end of the pipe.
   - `end[1]`: The write end of the pipe.

2. **Forking Processes:**

   ```c
   pid_t parent;
   parent = fork();
   if (parent < 0) perror("Fork Error");
   if (parent == 0) child_process(f1, cmd1);
   else parent_process(f2, cmd2);
   ```

   - The child process executes `cmd1` and writes its output to `end[1]`.
   - The parent process reads from `end[0]` and executes `cmd2`.

3. **Swapping FDs with `dup2`:**

   ```c
   dup2(f1, STDIN_FILENO); // Redirect input file to stdin
   dup2(end[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
   ```

   This ensures the commands read/write from the appropriate sources.

4. **Executing Commands:**

   Use `execve()` to execute commands, passing the full path, arguments, and environment variables.

   ```c
   execve(cmd, args, envp);
   ```

---

## 🌈 Visualizing the Workflow

```text
   infile                                             outfile
as stdin for cmd1                                 as stdout for cmd2            
       |                        PIPE                        ↑
       |           |---------------------------|            |
       ↓             |                       |              |
      cmd1   -->    end[1]       ↔       end[0]   -->     cmd2           
                     |                       |
            cmd1   |---------------------------|  end[0]
           output                             reads end[1]
         is written                          and sends cmd1
          to end[1]                          output to cmd2
       (end[1] becomes                      (end[0] becomes
        cmd1 stdout)                           cmd2 stdin)
```

---

## 🔑 Key Functions

### `pipe()`
- Creates a communication channel between processes.

### `fork()`
- Splits the program into parent and child processes.

### `dup2()`
- Redirects file descriptors to replace `stdin` and `stdout`.

### `execve()`
- Executes a program in the current process.

---

## 🚨 Error Handling

- Use `access()` to ensure the command exists before execution.
  ```c
  if (access(path_to_cmd, X_OK) != 0) perror("Command not found");
  ```
- Always close unused pipe ends to avoid deadlocks.
- Debug using `perror("Error")` to trace issues during execution.

---

## 🛠️ How to Compile and Run

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/your-repo/42-Pipex.git
   cd 42-Pipex
   ```

2. **Compile:**
   ```bash
   make
   ```

3. **Run:**
   ```bash
   ./pipex infile "cmd1" "cmd2" outfile
   ```

---

## 📚 Resources

- [Unix `pipe()` Documentation](https://man7.org/linux/man-pages/man2/pipe.2.html)
- [Fork and Exec](https://linux.die.net/man/2/fork)
- [File Descriptors Explained](https://en.wikipedia.org/wiki/File_descriptor)

---

<div align="center">
  <h2>✨ Happy Coding!</h2>
</div>

