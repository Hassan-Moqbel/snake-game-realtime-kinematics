# Snake Game (Terminal Real-Time Game Loop)

![C / C++](https://img.shields.io/badge/Language-C_%2F_C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Real-Time Game Loop](https://img.shields.io/badge/Architecture-Real--Time_Game_Loop-A8B9CC?style=for-the-badge)
![Non-Blocking I/O](https://img.shields.io/badge/System-Non--Blocking_I%2FO-4B0082?style=for-the-badge)
![2D Discrete Kinematics](https://img.shields.io/badge/Math-2D_Discrete_Kinematics-28A745?style=for-the-badge)
![Terminal GUI](https://img.shields.io/badge/Platform-Terminal_GUI-FF6F00?style=for-the-badge)
![Memory Safety](https://img.shields.io/badge/Security-Memory_Safety-6f42c1?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)

## Executive Overview
Developing real-time interactive software outside of standard graphical game engines requires a deep understanding of hardware timers, asynchronous input polling, and in-memory buffer manipulation. This project is a C/C++ implementation of the classic **Snake Game**, operating purely within a Command Line Interface (CLI). It establishes the fundamental building blocks of real-time simulations: discrete 2D spatial kinematics, array-shifting coordinate propagation, and collision matrix evaluation.

> [!CAUTION]
> **Software Timing & CPU Throttling Safety Callout**
> Unbounded `while(true)` spin-wait loops will aggressively consume 100% of the active CPU core, leading to thermal throttling and system sluggishness. Real-time game loops MUST yield processing time back to the OS scheduler utilizing `Sleep()` or `std::this_thread::sleep_for()`. Additionally, rendering terminal graphics via naive `system("cls")` calls causes severe framebuffer flickering; advanced iterations should manipulate cursor positions directly via ANSI escape codes. Finally, careful array boundary checking is critical to prevent segmentation faults when the snake's tail coordinates exceed allocated memory buffers.

## System Highlights
- **Deterministic Frame Rate Game Loop**: Executes physics, input parsing, and rendering cyclically at a regulated frames-per-second (FPS) limit.
- **Asynchronous Non-Blocking Input**: Utilizes OS-level interrupts (`_kbhit()`, `_getch()`) to capture user keystrokes without halting the main kinematic loop.
- **Circular Shifting Array Propagation**: Shifts discrete Cartesian coordinate vectors iteratively to drag the tail segments seamlessly behind the head node.
- **Pseudo-Random Food Generation**: Spawns objective nodes at randomized discrete vectors and includes collision-rejection logic to prevent spawning inside the snake body.

## Software Architecture Flowchart

```mermaid
flowchart TD
    START(["Application Entry: main()"]) --> INIT[Initialize Grid Boundaries & Entities]
    INIT --> LOOP_ENTRY{Game Loop Entry: `while(!gameOver)`}
    
    LOOP_ENTRY --> POLL[Poll Asynchronous Input: `_kbhit()`]
    POLL --> DIR[Map Direction Vector dx, dy]
    
    DIR --> KINEMATICS[Update Head Position: P_head = P_head + V]
    KINEMATICS --> BOUNDARY_CHK[Boundary & Tail Collision Check]
    
    BOUNDARY_CHK --> COLLISION{Collision Detected?}
    COLLISION -->|Yes| GAME_OVER[Set `gameOver = true` \n& Terminate Loop]
    COLLISION -->|No| FOOD_EVAL[Food Collision Evaluation]
    
    FOOD_EVAL --> FOOD_CHK{Food Consumed?}
    FOOD_CHK -->|Yes| SPAWN[Increment Score & Tail Length \nSpawn New Food]
    FOOD_CHK -->|No| TAIL_SHIFT[Propagate Tail Segments: \nShift Array Coordinates]
    
    SPAWN --> RENDER
    TAIL_SHIFT --> RENDER
    
    RENDER[Render Terminal Framebuffer] --> THROTTLE[Throttle CPU Clock: `Sleep(frameDelay)`]
    THROTTLE --> LOOP_ENTRY
    
    GAME_OVER --> FINAL_SCREEN[Game Over Screen: Print Final Score]
    FINAL_SCREEN --> EXIT([Exit 0])
```

## Algorithmic & Kinematic Mathematical Models

### 1. Discrete 2D Cartesian Kinematics
The spatial position of the head node advances sequentially at each discrete time step $\Delta t$ based on the normalized velocity vector $\vec{V}$:
$$\vec{P}_{head}(t + \Delta t) = \vec{P}_{head}(t) + \vec{V} \cdot \Delta t$$
*(Where $\vec{V} \in \{(0, 1), (0, -1), (1, 0), (-1, 0)\}$ and $\Delta t = 1 \text{ grid unit}$).*

### 2. Tail Segment Recurrence Formulation
To simulate trailing motion, the entire body array is shifted backwards by one index, effectively pulling the previous spatial history forward:
$$\vec{P}_{tail}[i] = \vec{P}_{tail}[i - 1], \quad \forall i \in [L - 1, 1]$$
$$\vec{P}_{tail}[0] = \vec{P}_{head}(t)$$
*(Where $L$ is the current length of the tail).*

### 3. Collision Boundary & Self-Intersection Set
A collision trigger evaluates to `True` if the head coordinate intersects the outer rigid walls ($W, H$) or its own tail array:
$$\text{Collision} \iff x_{head} \in \{0, W\} \lor y_{head} \in \{0, H\} \lor \exists i \in [0, L - 1] : \vec{P}_{head} = \vec{P}_{tail}[i]$$

### 4. Algorithmic Complexity
$$\text{Time Complexity per Tick: } \mathcal{O}(L) \quad (\text{linear shift of tail coordinates and collision scan})$$
$$\text{Auxiliary Space Complexity: } \mathcal{O}(L_{max}) \quad (\text{static contiguous array allocation for coordinate history})$$

## Build & Compilation Matrix
To compile this project locally on a machine equipped with a MinGW/GCC toolchain, execute the following from the root directory:

```bash
g++ -O2 "src/project in language of c programming .cxx" -o bin/snake_game.exe
```

## Repository Layout Tree
```text
📦 Snake Game
 ┣ 📂 src/             # Core C++ application source code
 ┃ ┗ 📜 project in language of c programming .cxx
 ┣ 📂 docs/            # Engineering documentation (Future)
 ┣ 📂 bin/             # Compiled executable binaries (Ignored in Git)
 ┣ 📜 README.md        # This document
 ┣ 📜 LICENSE          # MIT License
 ┗ 📜 .gitignore       # Build artifact exclusions
```

## Authentic Artifacts Catalog
- **Source Code Implementation**: Available directly within [`src/`](src/).

## Engineering Audit & Tradeoffs
- **Procedural Monolith vs. Object-Oriented Engines**: The current implementation is written as a single procedural file. For larger games, migrating to an Object-Oriented (OOP) Component-Entity System allows for clean compartmentalization of physics, rendering, and logic.
- **Terminal Rendering Mechanics**: Calling `system("cls")` forces the OS to destroy and recreate the terminal window buffer at 60 FPS, resulting in severe visual tearing. An advanced refactor should utilize ANSI VT100 escape codes (e.g., `\033[H`) to simply reset the cursor position and cleanly overwrite characters in place.

---

**Hassan Moqbel Morshed Ghaleb**
Mechatronics Engineer | Mechanical Design & CAD (SolidWorks & AutoCAD) | Preventive Maintenance & Electromechanical Systems | Industrial Automation, Control Systems, Robotics & Intelligent Machines | CAD/FEA, Embedded Systems, Python & C++
[GitHub](https://github.com/Hassan-Moqbel) · [Facebook](https://www.facebook.com/share/1BqxAgVjHi/) · [LinkedIn](https://www.linkedin.com/in/hassan-moqbel)

## License
This project is licensed under the [MIT License](LICENSE).
