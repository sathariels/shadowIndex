# Shadow Index

> A procedural narrative roguelike written in C++17.

**Shadow Index** is a terminal-based dungeon crawler where players navigate a surreal, shifting labyrinth. It combines roguelike resource management with a branching narrative system where choices have lasting consequences.

**Developer**: Sathariels (Nithilan Kumaran)

---

## Gameplay Features

- **Deep Stats System**: Manage 5 unique psychological stats—*Resonance, Dread, Clarity, Pride, Compassion*—instead of traditional HP/Mana.
- **Procedural Storytelling**: Explore a dungeon that is different every run, with events dynamically selected based on your previous choices.
- **Narrative Coherence**: Uses a custom "Story Flag" system to chain events together (e.g., finding a mysterious artifact early on unlocks specific outcomes in later depths).
- **Risk vs. Reward**: Every choice matters. Will you sacrifice *Clarity* to gain forbidden knowledge, or hold onto your *Humanity* at the cost of power?

## Technical Highlights

Built from scratch in C++17 to demonstrate core systems engineering concepts:

- **Entity-Component-Like Architecture**: 
  - `Shadows` (Equipment) rely on modular passive effect structures.
  - `Events` define data-driven narrative nodes with flexible requirements and outcomes.
- **Custom Event Engine**:
  - Implemented a **directed acyclic graph (DAG)** approach to storytelling where events can have `REQ` (prerequisites) and `ADDFLAG` (state changes).
  - Ensures narrative consistency within a procedurally generated environment.
- **Efficient Data Management**:
  - Uses `std::unordered_map` for O(1) asset lookups.
  - Data-driven design: Events and items are loaded from external text files, allowing for rapid iteration without recompiling.
- **Robust State Serialization**:
  - Full save/load system using custom text serialization for all game state, including RNG seeds, inventory, and narrative flags.
- **Deterministic RNG**:
  - Utilizes `std::mt19937` (Mersenne Twister) for high-quality, reproducible procedural generation seeded by user input.

## How to Run

### Prerequisities
- A C++17 compatible compiler (g++, clang++)
- Make

### Build & Play

1. **Clone & Enter Directory**:
   ```bash
   cd shadowIndex-2
   ```

2. **Compile**:
   ```bash
   make
   ```
   *This will generate the `shadow_index` executable.*

3. **Run**:
   ```bash
   ./shadow_index
   ```
