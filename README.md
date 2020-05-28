# Advanced Programming Techniques

# Milestone 1

## Table of Contents 
- [Gameplay](#gameplay)   
- [Algorithm](#algorithm)  
- [Classes](#classes)  
- [Data Structures](#data_structures)  
- [Tile Management](#tile_management)  

<a name="gameplay"></a>
## Gameplay

### List of commands
Menu (m): Show the menu (new, load, save, rules, credits, quit)  
Help (h or ?): Display the list of commands  
Exit/Quit (q or x): Exit the game  
Rules (r): Display a brief example of how to play  
Load (l): Load a game  
Save (s): Save the current game  
Taking turns:  
Source:[Centre (c) or FactoryId (1-5)] Colourcode:[RYBLU] Dest:[PatternLineId (1-5) or FloorLine (f)]  
e.g to take yellow tiles from factory 2 onto pattern line 5: `2 y 5`  



<a name="algorithm"></a>
## Algorithm

```
(Game setup, occurring only at the beginning of the game)
Fill the bag with tiles
Assign Player Boards to each player
Determine which player goes first (random?)
Place the Starting Player Marker in the centre factory

(Factory setup)
This stage is automatic and no player input is required.
For each factory, draw 4 tiles from the bag and place in the factory 
    If the bag is empty before all factories are full,
        return tiles from box lid to bag, then continue filling factories.
        If the box lid and bag are both empty before the factories are filled,
            continue round as normal, using incomplete factories.

(Begin Factory Offer stage)
On a player's turn, the player must:
    Pick all the tiles of the same colour from any single factory.
    If the player chose the centre factory and,
    the starting marker is in the centre factory,
        place the starting marker in the next available space on the player's floor line.
    Else (not the centre factory)
        move any remaining tiles from the chosen factory to the centre factory.

    If the player chooses to place the tiles on a pattern line,
        If the pattern line is empty and,
        the corresponding row in the mosaic does not contain a tile of the chosen colour,
        OR
        If the pattern line contains tiles, and
        the tiles are the same colour as the new chosen tiles, and
        the pattern line is not full,
            fill the pattern line from right to left, starting at the first empty space.
            If the pattern line fills before placing all the chosen tiles,
                place excess tiles in the player's floor line.
                If the floor line fills before all tiles are placed,
                    place the excess tiles in the box lid.
    else the player chose to place the tiles directly onto their floor line,
        place the tiles in the player's floor line.
        If the floor line fills before all tiles are placed,
            place the excess tiles in the box lid.

Repeat the factory offer stage until all factories (including the centre) are empty

(Begin Wall Tiling and scoring stage)
This stage will require no input from players, it is completely automated
    For each player,
        For each pattern line, 
        If the pattern line is complete (full; no empty spaces),
            Move one tile from the pattern line to the corresponding position on the mosaic, and
            move the remaining tiles from this pattern line to the box lid.
            If the newly placed tile has horizontally adjacent tiles on the mosaic,
                For the total number of adjacent tiles (including the newly placed tile), add 1 point per tile to the player's score.
                If the total number of adjacent tiles is 5 (the row is full),
                    trigger the end of game condition.
            If the newly placed tile has vertically adjacent tiles on the mosaic,
                For the total number of adjacent tiles (including the newly placed tile), add 1 point per tile to the player's score.
            If the newly placed tile had neither horizontally or vertically adjacent tiles on the mosaic,
                Add 1 point to the player's score.
            

        For each tile in the player's floor line,
            If the tile is in position 1 or 2,
                subtract 1 point from the player's score.
            Else if the tile is in position 3, 4 or 5,
                subtract 2 points from the player's score.
            Else (the tile is in position 6 or 7)
                subtract 3 points from the player's score.

            If the tile is not the starting marker
                place the tile in the box lid.
            Else (the tile is the starting marker)
                set the current player as the player to go first,
                place the tile in the centre factory.

            If the player's score has dropped below 0,
                set the player's score to 0.
    
    If the end of game condition has not been met,
        return to the factory setup stage.
    Else (final scoring and game end)
        For each player
            For each row of a mosaic
                If the row is complete (contains 5 tiles),
                    add 2 points to the player's score,
                    increment the players completed row count by 1.
            For each column of the mosaic,
                If the column is complete (contains 5 tiles),
                    add 7 points to the player's score.
            For each colour of tile,
                If the mosaic contains 5 of this colour,
                    add 10 points to the player's score.

        If both players have the same score
            if both players have the same number of completed rows
                the game is drawn.
            else 
                the player with the greatest number of completed rows is declared winner.
        else
            the player with the greatest score is declared winner.

```

<a name="classes"></a>
## Classes

Thoughts on possible class requirements.  

### Player 
Data: Name, Score, Player Board.

### Player Board
Data: Pattern Line Collection, Floor Line, Wall/Mosaic.

### Pattern Line
Data: Size, Tile Collection.

### Mosaic
Data: Pattern, Tile Collection.  

### Input Handler
Actions: Parse input from stdin. 

### File Handler
Actions: Reading/Writing save game files.

### Game Engine
Data: Game Model.  
Actions: Game logic.

### Game Model
Data: Tile Bag, Box Lid, Factory Collection, Central Factory, Player Collection.  
Actions: Maintain game state.  

### Linked List
Generic type, used for various tile collections.

<a name="data_structures"></a> 
## Data Structures

Thoughts on the various data structures required by each element of the game.

### Bag
Properties: Variable Length (known maximum: 100). Sequential addition and removal.  
Proposed structure: Linked list  
Reasons: The bag will be growing and shrinking as the game is played. An array could be used, but there is no requirement for random access.

### Lid
Properties: Variable Length (theoretical maximum: 100). Sequential addition and removal.  
Proposed structure: Linked list  
Reasons: The lid will be growing and shrinking as the game is played. An array could be used, but there is no requirement for random access.

### Standard Factory
Properties: Fixed length (4). Sequential addition, random removal.  
Proposed structure: Array  
Reasons: Small maximum size. Low "event" count, as this object is filled once, then emptied once per round.

### Centre Factory
Properties: Variable length (theoretical maximum: 101). Sequential addition, random removal.  
Proposed structure: STL Vector  
Reasons: Will grow and shrink many times in the course of a game. The removal process could happen for any number of items and at any position. 

### Pattern Line
Properties: Variable length (known maximums: 1, 2, 3, 4, 5). Sequential addition and removal.  
Proposed structure: Array for each line    
Reasons: Each pattern line has a fixed length

### Floor Line
Properties: Variable length (known maximum: 7). Sequential addition and removal.  
Proposed structure: Linked list  
Reasons: 

### Wall/Mosaic
Properties: Fixed length (5 rows, 5 columns). Random addition, no removals.  
Proposed structure: 2D array  
Reasons: Primarily a static object. It is a representation of a visual square.

<a name="tile_management"></a> 
## Tile Management
### Initialisation
Game start will need to generate exactly 20 tiles of each colour. These will be 
stored in the tile bag, in a random order. This will need to support a seed from the command line as well, so it is possible to generate the same game setup - this is primarily for testing.

### Moving Tiles
Keeping track of tiles so none get lost and none get duplicated is a bit of a challenge. There **must always** be 20 tiles of each colour and a single starting marker in existence throughout the duration of a game.

#### Permitted tile movement options

Reading this table: *"Can a tile move from `row` to `column`?"*  
Examples:   
*Can a tile move from Bag to Factory?* :heavy_check_mark:  
*Can a tile move from Factory to Bag?* :x:


|              |         Bag        |         Lid        |       Factory      |       Centre       |    Pattern Line    |     Floor Line     |        Wall        |
|--------------|:------------------:|:------------------:|:------------------:|:------------------:|:------------------:|:------------------:|:------------------:|
| **Bag**          |         :x:        |         :x:        | :heavy_check_mark: |         :x:        |         :x:        |         :x:        |         :x:        |
| **Lid**          | :heavy_check_mark: |         :x:        |         :x:        |         :x:        |         :x:        |         :x:        |         :x:        |
| **Factory**      |         :x:        | :heavy_check_mark: |         :x:        | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |         :x:        |
| **Centre**       |         :x:        | :heavy_check_mark: |         :x:        |         :x:        | :heavy_check_mark: | :heavy_check_mark: |         :x:        |
| **Pattern Line** |         :x:        | :heavy_check_mark: |         :x:        |         :x:        |         :x:        |         :x:        | :heavy_check_mark: |
| **Floor Line**   |         :x:        | :heavy_check_mark: |         :x:        |         :x:        |         :x:        |         :x:        |         :x:        |
| **Wall**         | :heavy_minus_sign: | :heavy_minus_sign: | :heavy_minus_sign: | :heavy_minus_sign: | :heavy_minus_sign: | :heavy_minus_sign: | :heavy_minus_sign: |

*Important notes about this table:*  
*1. It only covers wall tiles, not the starting marker movement (see below).*  
*2. It assumes the game is in normal play i.e. not recovering from exceptions etc.*  
*3. It assumes the tile bag has been successfully filled, or a game successfully loaded.*  
*4. Tiles will only ever move* out *of the wall at the completion of a game, hence there are no movements defined on the relevant row of the table.*

#### Starting marker movement
The starting marker can only move to and from the centre and a player's floor line. The marker moves from the centre to a player's floor line, when that player is the first player to take tiles from the centre during a round. The marker will then move from a player's floor line, back to the centre, at the beginning of a new round (post-scoring).

# Milestone 2
