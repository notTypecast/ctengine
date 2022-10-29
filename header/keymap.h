#ifndef TENGINE_KEYMAP_H
#define TENGINE_KEYMAP_H

#include <stdlib.h>
#include <limits.h>
#include <string.h>
// DEFSIZE: size used by init_keymap
#define DEFSIZE 17
// KEYSIZE: maximum size for a character code (def. 5)
#define KEYSIZE 5

/* Defines an item of a KeyMap
 * char key[KEYSIZE]: the character that the keypress sends to stdin
 * int val: the value that character is mapped to
 * struct KeyMapItem *next: the next item in the chain of the containing bucket
 */
typedef struct KeyMapItem {
    char key[KEYSIZE];
    int val;
    struct KeyMapItem *next;
} KeyMapItem;

/* Defines a KeyMap, which maps key codes to integer values
 * int size: the size of the KeyMap
 * KeyMapItem **array: the array of size (size) containing "buckets", which contain chains of KeyMapItems,
 *  each with the corresponding hash
 * KeyMap uses hashing with chains; for each item, the operation (key[0] % size) is used to determine the index of the
 *  bucket where the item will be saved
 * If there are already items saved there, it is placed at the beginning of the chain, after which the previously first
 *  item follows it
 */
typedef struct {
    int size;
    KeyMapItem **array;
} KeyMap;

// KeyMap operations
KeyMap* init_keymap();
void delete_keymap(KeyMap *keymap);
void keymap_put(KeyMap *keymap, const char key[KEYSIZE], int val);
int keymap_has(KeyMap *keymap, const char key[KEYSIZE]);
int keymap_get(KeyMap *keymap, const char key[KEYSIZE]);

#endif //TENGINE_KEYMAP_H
