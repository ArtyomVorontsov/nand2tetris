This repository contains my solutions for the **[Nand2Tetris](https://www.nand2tetris.org/)** course, which guides learners through building a modern computer system from first principles—starting with logic gates and ending with a high-level language and operating system.

## 📚 Course Overview

The course is divided into two parts:

- **Part I: From NAND to Hack** – Low-level hardware construction (Logic gates, ALU, Memory, CPU, and Computer architecture).
- **Part II: From Hack to Tetris** – High-level software stack (Assembler, VM Translator, Compiler, and Operating System).

> This project implements both hardware and software layers of a simple computer system.

---

## 📁 Project Structure

\`\`\`
nand2tetris/
├── 00-tools/           # Setup instructions and tools
├── 01-boolean-logic/   # Logic gates built from NAND
├── 02-arithmetic-logic/ # ALU and arithmetic components
├── 03-sequential-logic/ # Memory, Flip-Flops
├── 04-machine-language/ # Hack assembly and machine code
├── 05-computer-architecture/ # Full Hack computer
├── 06-assembler/       # Assembler implementation (e.g. in Python)
├── 07-vm-stack/        # Stack-based virtual machine
├── 08-vm-program-flow/ # Function calls and branching
├── 09-high-level-language/ # Compiler front-end (Jack)
├── 10-os/              # Basic operating system in Jack
\`\`\`

---

## 🚀 Getting Started

1. **Clone the repository:**
   \`\`\`bash
   git clone https://github.com/ArtyomVorontsov/nand2tetris.git
   cd nand2tetris
   \`\`\`

2. **Install Hardware Simulator:**
   Download the tools (Hardware Simulator, CPU Emulator, VM Emulator) from the official site:  
   👉 https://www.nand2tetris.org/software

3. **Run \`.hdl\` files:**
   Use the Hardware Simulator to load and test \`.hdl\` components.

4. **Run software tools:**
   - For Python-based tools like the assembler, use:
     \`\`\`bash
     python3 06-assembler/assembler.py path/to/Prog.asm
     \`\`\`

---

## 🛠️ Tools Used

- Hardware Simulator (Nand2Tetris)
- CPU Emulator
- VM Emulator
- Custom Python scripts (Assembler, VM Translator)
- Bash scripts for automation
- Jack Compiler (written in the course)

---

## 🧠 Concepts Covered

- Logic Gates (NAND, AND, OR, NOT, etc.)
- ALU and Binary Arithmetic
- Sequential Logic (Registers, RAM)
- Machine Language & Assembly
- Computer Architecture (Hack CPU)
- Virtual Machine Design
- Compiler Construction
- Operating System Primitives

---

## 📸 Screenshots

> Add images of ALU simulation, assembler CLI usage, or compiled Jack programs running in the VM emulator if desired.

---

## 📜 License

This project is for educational purposes. Original course material is © by Nisan & Schocken, under [Nand2Tetris.org](https://www.nand2tetris.org/).  
My implementations are shared for learning and reference.

---

## 🙌 Acknowledgments

- Prof. Noam Nisan and Prof. Shimon Schocken for the amazing course
- The online [Nand2Tetris community](https://forum.nand2tetris.org/) for support and discussion

---

## 💡 Note

If you're following along with the course and want to avoid spoilers, proceed with caution—this repository contains full solutions.

