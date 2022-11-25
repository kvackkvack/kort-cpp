This project was created as computational assistance in a mathematical analysis of the properties of (a slightly simplified version of) the card game Sevens for two players, including
* efficient representations of game states along with fast bit-level operations on them
* utilities to randomly generate game states
* a class of functions to exhaustively check whether or not a game state is winning under optimal play, complete with cache and other (mathematically proven) optimisations
* a test suite which combines all of the above to efficiently generate thousands of game states and check various conjectured properties, returning minimal counterexamples if found.

