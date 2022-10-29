#include "../header/keymap.h"

/* Initializes an empty KeyMap
 * DEFSIZE is used as the number of buckets, as defined in keymap.h
 * Return: Pointer to the initialized KeyMap
 */
KeyMap* init_keymap() {
    KeyMap *new = malloc(sizeof(KeyMap));
    new->size = DEFSIZE;
    new->array = malloc(sizeof(KeyMapItem*)*DEFSIZE);
    return new;
}

/* Deletes a KeyMap, as well as all the items contained within it
 */
void delete_keymap(KeyMap *keymap) {
    KeyMapItem *curr, *tmp;
    for (int i = 0; i < keymap->size; ++i) {
        curr = keymap->array[i];
        if (curr == NULL) {
            continue;
        }
        do {
            tmp = curr;
            curr = curr->next;
            free(tmp);
        } while (curr != NULL);
    }
    free(keymap);
}

/* Adds a new key-int pair to the KeyMap
 * Note: INT_MAX should not be used as a value for any key (see keymap_get)
 * The new item is placed at the beginning of its bucket's chain, ensuring the operation is O(1) regardless of chain size
 * The item's hash is calculated as (key[0] % size), where size is the number of buckets, and is used as the index to the
 *  array of buckets
 */
void keymap_put(KeyMap *keymap, const char key[KEYSIZE], int val) {
    KeyMapItem *item = malloc(sizeof(KeyMapItem));
    memcpy(item->key, key, KEYSIZE);
    item->val = val;

    int index = key[0] % keymap->size;
    item->next = keymap->array[index];
    keymap->array[index] = item;
}

/* Checks for the existence of a key in the KeyMap
 * If the key is found, it is placed at the beginning of its chain; this is done since keymap_has will almost always be
 *  followed by keymap_get, to ensure the following get operation will be O(1) instead of having to redo a potentially
 *  large linear search through the chain
 *  Return: 1 if key exists in KeyMap, else 0
 */
int keymap_has(KeyMap *keymap, const char key[KEYSIZE]) {
    int index = key[0] % keymap->size;
    KeyMapItem *curr = keymap->array[index], *prev = NULL;
    if (curr == NULL) {
        return 0;
    }
    do {
        // compare entire key to current chain item
        if (!memcmp(curr->key, key, KEYSIZE)) {
            // if found, place at beginning of chain (if it isn't there already)
            if (prev != NULL) {
                prev->next = curr->next;
                curr->next = keymap->array[index];
                keymap->array[index] = curr;
            }
            return 1;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != NULL);

    return 0;
}

/* Gets an item from the KeyMap, given its key
 * This operation assumes the key exists in the KeyMap and therefore should always be preceded by keymap_has, unless the
 *  caller knows the key exists in the KeyMap
 * If this function is called on a key which doesn't exist in the KeyMap, it will either crash (if the corresponding
 *  bucket doesn't contain any items) or will return INT_MAX (if the existing chain was traversed, but no keys matched)
 */
int keymap_get(KeyMap *keymap, const char key[KEYSIZE]) {
    int index = key[0] % keymap->size;
    KeyMapItem *curr = keymap->array[index];
    do {
        if (!memcmp(curr->key, key, KEYSIZE)) {
            return curr->val;
        }
        curr = curr->next;
    } while (curr != NULL);

    return INT_MAX;

}