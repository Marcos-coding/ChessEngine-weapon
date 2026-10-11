# Weapon Chess Engine Documentation

## By Marcos Bellini

This is my first chess engine, programmed in C++ and designed with CMake in Visual Studio. It is meant to work as a competitive chess bot, and also for (human) users to practice against a machine.

### Requirements

After investigating multiple chess engines, such as Sunfish, Vice and Stockfish, there can be concluded a list of basic requirements for a functional engine that is able to run in any platform, in a reasonable amount of time. These requirements span from how the application is supposed to function as a whole, to the inner workings of the algorithms used. The Moscow technique is employed to prioritize said requirements, in order to define the most important characteristics of the engine and provide a roadmap of its design.

##### Functional Requirements

| Requirement | Name                          | Description                                                                                                                                                      | Priority |
| ----------- | ----------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------- |
| FR01        | Responsiveness                | The engine must receive chess positions, calculate its response, and return a move through the console-based interface and compatible graphical user interfaces. | Must     |
| FR02        | Move generation               | The engine must generate all legal moves for any supported chess position.                                                                                       | Must     |
| FR03        | Evaluation function           | The engine must evaluate a position from a consistent perspective, allowing it to compare the relative advantage of White and Black.                             | Must     |
| FR04        | Search algorithm              | The engine must search possible move sequences using Minimax to select the best available move according to its evaluation function.                             | Must     |
| FR05        | Opening book                  | The engine should use an opening book to select moves during the opening phase when a suitable book move is available.                                           | Must     |
| FR06        | Special moves                 | The engine must correctly support castling, en passant captures, and pawn promotion.                                                                             | Must     |
| FR07        | Check and checkmate detection | The engine must detect check, checkmate, and stalemate positions.                                                                                                | Must     |
| FR08        | Game termination              | The engine must recognize supported game-ending conditions, including checkmate, stalemate, and applicable draw conditions.                                      | Must     |
| FR09        | Position management           | The engine must maintain the board state, active player, castling rights, en passant target square, and other information required to determine legal moves.     | Must     |
| FR10        | Position initialization       | The engine must initialize a standard chess game and load custom positions using the FEN notation.                                                               | Must     |
| FR11        | Move execution                | The engine must apply a legal move to the current position and update all relevant game-state information.                                                       | Must     |
| FR12        | Search limits                 | The engine must allow the search to be limited by at least a maximum depth or a time limit.                                                                      | Must     |
| FR13        | Search information            | The engine should report search information, such as depth, evaluation score, and nodes searched, when requested by a compatible GUI.                            | Should   |

##### Non-Functional Requirements

| Requirement | Name                | Description                                                                                                                                                                            | Priority |
| ----------- | ------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------- |
| NFR01       | UCI protocol        | The engine must communicate with compatible chess GUIs using the Universal Chess Interface (UCI) protocol.                                                                             | Must     |
| NFR02       | Alpha-Beta pruning  | The search algorithm must use Alpha-Beta pruning to reduce the number of positions explored by Minimax without changing its result at a given search depth.                            | Must     |
| NFR03       | Iterative deepening | The engine should progressively increase search depth and retain the best move found at the last completed iteration.                                                                  | Should   |
| NFR04       | Quiescence search   | The engine should extend the search at unstable positions, particularly those involving captures, to reduce evaluation errors caused by stopping the search during tactical sequences. | Should   |
| NFR05       | Transposition table | The engine should cache previously searched positions and reuse valid results when the same position is encountered again.                                                             | Should   |
| NFR06       | Move ordering       | The engine should prioritize promising moves to improve the effectiveness of Alpha-Beta pruning.                                                                                       | Should   |
| NFR07       | Portability         | The engine must compile and run on Windows and Linux using a supported C++ compiler and build system.                                                                                  | Must     |
| NFR08       | Correctness         | The engine must not generate or select illegal chess moves in supported positions.                                                                                                     | Must     |
| NFR09       | Maintainability     | The source code must be organized into modules with clearly defined responsibilities and interfaces.                                                                                   | Must     |
| NFR10       | Resource management | The engine must manage memory safely and respect the configured search limits under normal operating conditions.                                                                       | Must     |
| NFR11       | Testability         | The engine must support automated tests for move generation, position updates, evaluation, and search behavior.                                                                        | Must     |
| NFR12       | Performance         | The engine should complete searches within the configured limits and provide a usable response time on supported hardware.                                                             | Should   |
| NFR13       | Bitboards           | Bitboards could be used to increase the efficiency of moves storage and calculations                                                                                                   | Could    |
| NFR14       | Reasoning           | The engine could have a deep learning system to enhance move selection and evaluation efficiently                                                                                      | Could    |

### Design

Unlike regular desktop apps, most of the engine`s requirements are centered around the workings of the algorithms themselves and not necessarily the desired output. This is due to the existence of an established format for the I/O of chess engines in order to communicate with GUIs and each other correctly. In this section, the technical details and constraints of the implementation of Weapon Chess Engine will be outlined with more precision in order to implement them in actual code.

##### TD01 — Minimax Algorithm

Objective: Select the best move by exploring possible sequences of moves and evaluating the resulting positions.

- The search will use the Minimax algorithm with alternating maximizing and minimizing layers.
- The engine will evaluate positions from a consistent perspective, preferably from White's point of view, or alternatively from the perspective of the side to move. The implementation must use one convention consistently.
- The search will initially use a configurable maximum depth.
- At leaf nodes, the engine will call the evaluation function to estimate the position's favorability.
- Checkmate and draw positions will be handled as terminal nodes, with appropriate scores.
- The root search will return the best legal move found.

Implementation note: Use a recursive function that receives the position, depth, and whose turn it is to move. Keep the evaluation convention consistent throughout the search.

##### TD02 — Alpha-Beta Pruning

Objective: Reduce the number of positions explored by Minimax without changing the result of a full-width search at the same depth.

- The search will maintain two bounds: alpha, the best score guaranteed to the maximizing player, and beta, the best score guaranteed to the minimizing player.
- Alpha will be updated when a better maximizing score is found; beta will be updated when a better minimizing score is found.
- The search will stop exploring the remaining moves of a node when alpha is greater than or equal to beta.
- Move ordering will be used to improve pruning efficiency.
- The initial search window will be negative infinity to positive infinity, represented by suitable score bounds.

Implementation note: Alpha-beta should be integrated directly into Minimax rather than implemented as a separate search algorithm.

##### TD03 — Iterative Deepening

Objective: Improve move ordering and allow the engine to return a reliable move when the search is interrupted by a time limit.

- The engine will perform successive searches at depths 1, 2, 3, and so on, up to the configured maximum depth.
- Each iteration will start a new root search using the results of the previous iteration to improve move ordering.
- The engine will retain the best move from the last fully completed iteration.
- If a time limit is reached, the engine will stop the search and return the last completed result.
- The engine will support a maximum-depth limit even when no time limit is specified.

Implementation note: Iterative deepening will be implemented as an outer loop around the alpha-beta search. Search interruption must not cause the engine to return an incomplete or invalid result.

##### TD04 — Transposition Table

Objective: Avoid unnecessary repeated searches by storing and reusing information about previously searched positions.

- The engine will use a hash table to store search information associated with positions.
- Each entry will contain a position hash, the search depth, the evaluation score, the type of score bound, and the best move found.
- The table will distinguish between exact scores, lower bounds, and upper bounds so that alpha-beta results are reused correctly.
- Entries will only be used to cut off a search when their stored depth and score bounds justify doing so.
- The table will use a position hash that accounts for the side to move, castling rights, and en passant availability, in addition to the piece placement.
- The engine will use a replacement policy when the table is full, preferably favoring entries from deeper searches.

Implementation note: Zobrist hashing is a suitable approach. The table should also provide the stored best move for move ordering, even when the entry is not deep enough to justify a cutoff.

##### TD05 — Quiescence Search

Objective: Reduce evaluation errors caused by stopping the main search in the middle of tactical exchanges.

- At the nominal search depth limit, the engine will perform a quiescence search instead of immediately evaluating every position.
- The quiescence search will evaluate the current position as a stand-pat option when the side to move is not in check.
- It will explore tactical continuations, initially captures and promotions, using alpha-beta pruning.
- If the side to move is in check, the engine must not use stand-pat evaluation; it must search legal check evasions.
- The quiescence search will have a safeguard against excessive recursion.

Implementation note: Quiescence search should be a separate recursive function. It must correctly handle check evasions and terminal positions, and it should avoid infinite or excessively long tactical sequences.

##### TD06 — Move Ordering

Objective: Improve alpha-beta pruning by examining promising moves first.

- The engine will prioritize the transposition table's best move when available.
- Captures will be ordered using a simple heuristic, such as the Most Valuable Victim–Least Valuable Attacker (MVV-LVA).
- Promotions and other promising tactical moves will receive higher priority.
- If necessary, the engine may later incorporate killer moves and history heuristics.

Implementation note: Begin with the transposition table move, captures, and promotions. More advanced ordering heuristics can be added later without changing the overall search architecture.

##### TD07 — Position Evaluation

Objective: Estimate the relative favorability of a chess position.

- The initial evaluation function will consider material balance using fixed piece values.
- Piece-square tables will be used to incorporate simple positional considerations.
- The score convention will be consistent throughout the search.
- Checkmate scores will be substantially better or worse than any ordinary positional evaluation.
- Drawn positions will receive an appropriate draw score.

Implementation note: Start with material and piece-square tables. Mobility, king safety, pawn structure, and other positional factors can be added incrementally.

##### TD08 — Legal Move Generation and Position Updates

Objective: Ensure that all search operations respect the rules of chess and maintain a consistent board state.

- The board will use an array of 64 squares, with piece types and colors represented using suitable enumerations.
- The move generator will generate legal moves, accounting for check, pins, castling, en passant, and promotions.
- Position updates will preserve all relevant state, including the side to move, castling rights, en passant square, and move counters.
- The engine will provide reversible move operations, allowing a move to be applied and then undone during search.
- Special moves will be tested independently.

##### TD09 — Terminal Positions and Draw Detection

Objective: Handle game-ending positions correctly during search.

- The engine will detect checkmate and stalemate when no legal moves remain.
- It will recognize draw conditions supported by the initial implementation, including repetition and the applicable move-count rule.
- Terminal scores will be handled before ordinary static evaluation.
- Checkmate scores will account for search distance where appropriate, favoring faster wins and delaying unavoidable losses.

Implementation note: Threefold repetition and the fifty-move rule require position history and move counters. The engine must not rely solely on the current board layout to identify these conditions.

##### TD10 — UCI Search Control

Objective: Integrate the search algorithm with the UCI communication layer.

- The engine will support the required UCI commands for initialization, readiness, position setup, and search requests.
- Search requests will support depth limits and time controls.
- The engine will report the selected move using the UCI bestmove response.
- Search cancellation must be handled safely when a new search starts or a stop command is received.
- Search diagnostics will be reported using the appropriate UCI information messages.

Implementation note: Keep the UCI parser independent of the search implementation. The search layer should receive structured limits and return a result, rather than parsing protocol strings itself.

#### Implementation roadmap

Step 1: Define all the data structures to be used for the pieces, board, and moves. Create a legal move generator that works correctly and test it with a variety of positions. Design a basic evaluation function for positions.

Step 2: Build the Minimax search algorithm and make sure it works at reasonable depths like in a real game. Then implement Alpha-Beta and a basic move ordering algorithm

Step 3: Construct the UCI protocols to turn it into a functional chess engine. Test it in a console and a chess GUI.

Step 4: Implement the iterative deepening and the quiescence search

Step 5: Increase the efficiency by adding the transposition table. Also refine the evaluation function if necessary.

Step 6: If nothing is to be corrected, continue improving by using bitboards and other efficient data structures.

Step 7: Consider adding a form of artificial reasoning to the evaluation system, like a small trainable neural network.
