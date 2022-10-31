# ctegnine

### Description
CTEngine is a simple game engine for command line games, written in C.
It is a C implementation of [TEngine](https://github.com/notTypecast/tengine).
Making a game with CTEngine consists of the following steps.

### General Steps
1. Include the "ctengine.h" header file. The file can be found in the "header" folder.
2. Create a `Queue`. This can be achieved by calling the `init_queue` function, such as `Queue *queue = init_queue()`.
3. Create a `Drawer`. Similarly, the `init_drawer` function is used, `Drawer *drawer = init_drawer()`.
4. Create a `KeyListener`. The initialization function will require two arguments, the previously created `Queue`, as well as the `Drawer`. For instance, `KeyListener *listener = init_keylistener(queue, drawer)`.
5. Using the `drawer_set_fps` function, set the framerate the game should run at, such as `drawer_set_fps(drawer, 30)`. Add the keys the `KeyListener` should listen for using the `keylistener_add_key` function: `keylistener_add_key(listener, CONST.K_ESC, 0)`. Under `CONST`, all the available keys can be found with their corresponding values. Each key can be mapped to an integer value.
6. Start the drawer thread using the `drawer_start_thread` function: `drawer_start_thread(drawer, queue, gameloop)`. Here, `gameloop` refers to the game loop function, as explained below.
7. Call `keylistener_handle_in`, passing the `KeyListener` to it. This function will block and start handling key presses.

### The game loop function
The game loop function is the main function run by the drawer thread. Its purpose is to read key press events from the event queue, handle game logic accordingly and draw to the screen.

The function's return type should be `void*`. It should only take one argument of type `void*`. At the beginning of the function, by using `args_get_drawer` and `args_get_queue` and passing to them the function's argument, the `Drawer` and `Queue` can be saved.

First, the function will require the display size. To get it, the function `display_get_size` can be called, passing `drawer->display` as an argument (where `drawer` refers to the Drawer). That function returns an `int *` which the game loop function should make sure to free once finished.

Once inside the actual loop, the function should call `queue_empty` on the queue to check for new key press events. If there are any, `queue_get` should be used to read their corresponding values and handle them accordingly. The game logic should follow.

The final part of the loop should handle drawing to the screen. At first, `display_clear` should be called on the Display. After that, the function `display_set` can be used to set a character at a position. For instance, `display_set(drawer->display, 0, 0, "x")` will set the first character of the first row to "x". Finally, `drawer_draw_display` should be called, passing the Drawer as an argument.

After the loop, the function should return 0.

#### Game logic & exiting the game
Besides the player exiting the game using a combination of keys, such as CTRL+c, or a key set to produce the value 0 in the queue, the game can also be ended by the game's logic. This is desirable in cases where the player has won or lost. 

In such cases, the game loop function is responsible for exiting the game. This can be achieved by setting the `finished` flag of the Queue to `1` (`queue->finished = 1`) and, subsequently, exiting the game loop function. Optionally, the function `drawer_set_exit_msg` can also be used to set the message to be displayed after the game ends.

#### Drawing to the Display
As shown above, using the Drawer's internal Display, specified positions on the screen can be changed to display any character. The maximum character size chosen is `CELLBYTES` (default 4), enough to fit any UTF-8 character. It must be noted, however, that this allows the user to pass multiple characters to one cell (for instance, `display_set(drawer->display, 0, 0, "abcd")`. This must never be done, as it will prevent the Display from displaying properly on the terminal screen. The additional bytes are reserved for characters that, visually, require only a single cell, but which require more than a single byte to be represented (for example, the ■ (square) character).

If a character is exactly `CELLBYTES` bytes long, the function `display_set_exact` can be used in place of `display_set`. The former is more efficient, seeing as it performs no additional checks for character length. It can even be used with characters that are less than `CELLBYTES` bytes, as long as those characters are part of a `char` array that is padded with as many `\0` characters as required to be `CELLBYTES` in size.

### Documentation

#### KeyListener

function|arguments|returns|description
-|-|-|-
`init_keylistener`|`Queue*`, `Drawer*`|`KeyListener*`|Initializes a KeyListener
`delete_keylistener`|`KeyListener*`||Deletes a KeyListener, is automatically called on `keylistener_handle_in` exit
`keylistener_add_key`|`KeyListener*`, `const char*`, `int`||Adds a key-int pair to the KeyListener's internal KeyMap
`keylistener_handle_in`|`KeyListener*`||Blocks and begins handling key press events
`keylistener_exit`|`KeyListener*`||To be called on exit by `keylistener_handle_in`, handles exit event

#### Drawer
function|arguments|returns|description
-|-|-|-
`init_drawer`||`Drawer*`|Initializes a Drawer
`delete_drawer`|`Drawer*`||Deletes a Drawer, is automatically called on `keylistener_handle_in` exit
`drawer_draw_display`|`Drawer*`||Blocks for the required amount (based on FPS value) and draws the values contained in the drawer's display to the screen
`drawer_set_fps`|`Drawer*`,`int`|`int`|Sets the FPS value for the given Drawer, returns 0 if successful, else 1
`drawer_start_thread`|`Drawer*`, `Queue*`,`void*(*f)(void*)`||Starts the drawer thread, which runs the function `f`
`drawer_set_exit_msg`|`Drawer*`,`const char*`||Sets the exit message that should be displayed after the game ends; by default, no message is displayed
`drawer_clear_exit_msg`|`Drawer*`||Deletes and clears a previously set exit message, if one exists

#### Display
function|arguments|returns|description
-|-|-|-
`init_display`||`Display*`|Initializes a Display
`delete_display`|`Display*`||Deletes a Display, is automatically called by `delete_drawer` if the Display was created by a Drawer
`display_update_size`|`Display*`||Updates the Display size to the current terminal size (should not currently be used)
`display_get_size`|`Display*`|`int*`|Returns a dynamically allocated `int[2]` containing the row and column numbers respectively, should be freed when unneeded
`display_clear`|`Display*`||Clears the Display, setting a whitespace character everywhere
`display_get`|`Display*`, `int`, `int`|`char*`|Gets the value at the specified row and column of the given Display
`display_set`|`Display*`, `int`, `int`, `char*`||Sets the value at the specified row and column of the given Display to the given character
`display_set_exact`|`Display*`, `int`, `int`, `char*`||Same as `display_set`, with the difference that the `char*` passed as an argument must be guaranteed to be exactly `CELLBYTES` in size (default 4); if it is smaller, it must be padded with enough `'\0'` characters at the end to match the required size

#### Queue
function|arguments|returns|description
-|-|-|-
`init_queue`||`Queue*`|Initializes a thread-safe Queue
`delete_queue`|`Queue*`||Deletes a Queue, is automatically called by a KeyListener when handling an exit event
`queue_empty`|`Queue*`|`int`|Checks whether a Queue is empty and returns 1 if it is, or 0 if it is not
`queue_put`|`Queue*`, `int`||Places a new item in the Queue
`queue_get`|`Queue*`|`int`|Gets the next item from the Queue, assuming the Queue is not empty
`queue_clear`|`Queue*`||Clears a Queue, deleting all items contained within

#### KeyMap
function|arguments|returns|description
-|-|-|-
`init_keymap`||`KeyMap*`|Initializes a KeyMap
`delete_keymap`|`KeyMap*`||Deletes a KeyMap, is automatically called by `delete_keylistener` if the KeyMap was created by a KeyListener
`keymap_put`|`KeyMap*`, `const char*`, `int`||Places a new key-item pair in the KeyMap
`keymap_has`|`KeyMap*`, `const char*`|`int`|Checks whether the KeyMap has the specified key and returns 1 if it does, else 0
`keymap_get`|`KeyMap*`, `const char*`|`int`|Returns the value associated with the specified key in the KeyMap, assuming it exists; might crash or return `INT_MAX` if key does not exist

### CONST Key constants
Key|CONST.
-|-
ESC|`K_ESC`
F1|`K_F1`
F2|`K_F2`
F3|`K_F3`
F4|`K_F4`
F5|`K_F5`
F6|`K_F6`
F7|`K_F7`
F8|`K_F8`
F9|`K_F9`
F10|`K_F10`
F11|`K_F11`
F12|`K_F12`
\`|`K_GRAVE_ACCENT`
1|`K_1`
2|`K_2`
3|`K_3`
4|`K_4`
5|`K_5`
6|`K_6`
7|`K_7`
8|`K_8`
9|`K_9`
0|`K_0`
~|`K_TILDE`
!|`K_EXCLAMATION_MARK`
@|`K_AT_SIGN`
#|`K_HASH`
$|`K_DOLLAR_SIGN`
%|`K_PERCENTAGE_SIGN`
^|`K_CIRCUMFLEX`
&|`K_AMPERSAND`
\*|`K_ASTERISK`
(|`K_OPARENTHESIS`
)|`K_CPARENTHESIS`
-|`K_DASH`
\_|`K_UNDERSCORE`
=|`K_EQ_SIGN`
+|`K_PLUS_SIGN`
BACKSPACE|`K_BACKSPACE`
TAB|`K_TAB`
Q|`K_Q`
q|`K_q`
W|`K_W`
w|`K_w`
E|`K_E`
e|`K_e`
R|`K_R`
r|`K_r`
T|`K_T`
t|`K_t`
Y|`K_Y`
y|`K_y`
U|`K_U`
u|`K_u`
I|`K_I`
i|`K_i`
O|`K_O`
o|`K_o`
P|`K_P`
p|`K_p`
[|`K_OBRACKET`
{|`K_OBRACE`
]|`K_CBRACKET`
}|`K_CBRACE`
\||`K_BAR`
\ |`K_BACKSLASH`
A|`K_A`
a|`K_a`
S|`K_S`
s|`K_s`
D|`K_D`
d|`K_d`
F|`K_F`
f|`K_f`
G|`K_G`
g|`K_g`
H|`K_H`
h|`K_h`
J|`K_J`
j|`K_j`
K|`K_K`
k|`K_k`
L|`K_L`
l|`K_l`
;|`K_SEMICOLON`
:|`K_COLON`
'|`K_QUOTE`
"|`K_DQUOTE`
ENTER|`K_RETURN`
Z|`K_Z`
z|`K_z`
X|`K_X`
x|`K_x`
C|`K_C`
c|`K_c`
V|`K_V`
v|`K_v`
B|`K_B`
b|`K_b`
N|`K_N`
n|`K_n`
M|`K_M`
m|`K_m`
,|`K_COMMA`
\<|`K_LT_SIGN`
.|`K_DOT`
\>|`K_GT_SIGN`
/|`K_SLASH`
?|`K_QUESTION_MARK`
SPACEBAR|`K_SPACEBAR`
INSERT|`K_INSERT`
HOME|`K_HOME`
PAGE UP|`K_PGUP`
DELETE|`K_DEL`
END|`K_END`
PAGE DOWN|`K_PGDN`
UP ARROW|`K_UARROW`
LEFT ARROW|`K_LARROW`
DOWN ARROW|`K_DARROW`
RIGHT ARROW|`K_RARROW`
