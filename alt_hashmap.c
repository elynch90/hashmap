#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Python.h>

#define HASHMAP_SIZE 10

typedef struct Hashmap {
  char* key;
  int value;
  struct Hashmap* next;
} Hashmap;

unsigned long hash(char* str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

static PyObject* hashmap_put(PyObject* self, PyObject* args) {
  PyObject* map_obj;
  char* key;
  int value;
  if (!PyArg_ParseTuple(args, "Osi", &map_obj, &key, &value))
    return NULL;
  Hashmap** map = (Hashmap**) PyCapsule_GetPointer(map_obj, "hashmap");
  unsigned long hash_val = hash(key) % HASHMAP_SIZE;
  Hashmap* new_entry = (Hashmap*) malloc(sizeof(Hashmap));
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = map[hash_val];
  map[hash_val] = new_entry;
  Py_RETURN_NONE;
}

static PyObject* hashmap_get(PyObject* self, PyObject* args) {
  PyObject* map_obj;
  char* key;
  if (!PyArg_ParseTuple(args, "Os", &map_obj, &key))
    return NULL;
  Hashmap** map = (Hashmap**) PyCapsule_GetPointer(map_obj, "hashmap");
  unsigned long hash_val = hash(key) % HASHMAP_SIZE;
  Hashmap* entry = map[hash_val];
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0)
      return Py_BuildValue("i", entry->value);
    entry = entry->next;
  }
  return Py_BuildValue("i", -1);
}

static PyObject* hashmap_new(PyObject* self, PyObject* args) {
  Hashmap** map = (Hashmap**) malloc(HASHMAP_SIZE * sizeof(Hashmap*));
  memset(map, 0, HASHMAP_SIZE * sizeof(Hashmap*));
  return PyCapsule_New(map, "hashmap", NULL);
}

static PyObject* hashmap_free(PyObject* self, PyObject* args) {
  PyObject* map_obj;
  if (!PyArg_ParseTuple(args, "O", &map_obj))
    return NULL;
  Hashmap** map = (Hashmap**) PyCapsule_GetPointer(map_obj, "hashmap");
  int i;
  for (i = 0; i < HASHMAP_SIZE; i++) {
    Hashmap* entry = map[i];
    while (entry != NULL) {
      Hashmap* temp = entry;
      entry = entry->next;
      free(temp);
    }
  }
  free(map);
  Py_RETURN_NONE;
}
