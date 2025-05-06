# CCTS Project  
**Janga Tushita Sharva & Gayatri Shreeya Patnala**

---

## 📁 Directory Structure

This directory contains the following files:

- `Node.h` — Defines the types of Nodes.
- `NodeList.h`, `NodeList.cpp` — NodeList functions.
- `WaitsForGraph.h`, `WaitsForGraph.cpp` — Deadlock detection functions.
- `OperationType.h` — Enum for `READ` and `WRITE` operations (used across NodeList and WaitsForGraph).
- `Transaction.h` — Defines the `Transaction` class.
- `Logger.h` — Functions for logging outputs and debug info.
- `Scheduler.h`, `Scheduler.cpp` - Defines the O2PL Scheduler class.
- `OptimisticScheduler.h`, `OptimisticScheduler.cpp` - Defines Optimistic O2PL Scheduler class.
- `O2PL.cpp`, `O-O2PL.cpp` - Contains main function that invokes transactions and output schedules.

### Sample Folders

- `inputs/` — Input test cases.
- `outputs/` — Output files generated after execution.

---

## 📄 Input File

- `inp.txt` is a sample input file.
- You can modify this file to test different scenarios.

---

## ⚙️ Requirements

- **C++17** is required (due to use of template functions).
- Input file must be named `inp.txt` or renamed in `run.sh`.
- `outputs/` folder must exist. It will be **cleared automatically** before every run.

---

## 🏃 Execution Instructions

### Method 1: Run for One Input File

1. Give executable permission:
   ```bash
   chmod u+x run.sh
