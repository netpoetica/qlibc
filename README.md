What's qLibc?
=============

qLibc is currently one of the most functionally-complete public licensed C/C++ libraries. The goal of qLibc project is to provide a **simple and powerful general purpose C/C++ library**, which includes all kinds of containers and general library routines. It provides ready-made set of common container APIs with a consistent API look.

## qLibc Copyright

All of the deliverable code in qLibc has been dedicated to the public domain by the authors. Anyone is free to copy, modify, publish, use, compile, sell, or distribute the original qLibc code, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

## API Reference

* [qlibc Core API Reference](http://wolkykim.github.io/qlibc/doc/html/files.html)
  * Containers
    * List --- Doubly Linked List.
    * List Table --- KEY/VALUE paired table implemented on linked-list.
    * Hash Table --- Hash-based KEY/VALUE paired table.
    * Static Hash Table --- Static(array/mmapped/shared) memory based KEY/VALUE paired table.
    * Vector --- implements a growable array of elements.
    * Queue --- FIFO(First In First Out) implementation.
    * Stack --- LIFO(Last In First Out) implementation.
  * General utilities.
    * String
    * I/O
    * File
    * IPC, Semaphore Shared-memory
    * En/decoders
    * Hashes
    * System
    * Time

* [qLibc Extension API Reference](http://wolkykim.github.io/qlibc/doc/html/files.html)
  * INI-style Configuration File Parser.
  * Apache-style Configuration File Parser.
  * Rotating File Logger.
  * HTTP client.
  * Database(MySQL) interface.
  * Token-Bucket.

## Consistent API Look

All container APIs have a consistent look and feel. Basically, it provides a creator function which usually returns a pointer of a container structure. And **all functions related to the container can be accessed through function pointers inside of the container**.

So, regardless of which container you use, you can simply put elements into list like pContainer->put(pContainer, ...). It looks like C++ class but it's pure C implementation. Of course it works with both of your C and C++ code**.

We used this concept as much as possible because it is a simplified way of thinking and helps to improve readability. Consequently, it helps people to write bug-free code more easily.

The example below illustrates qlibc in action:

```
  // create a hash-table.
  qhashtbl_t *tbl = qhashtbl(0, QHASHTBL_OPT_THREADSAFE);
  
  // add an element which key name is "score".
  int x = 12345;
  tbl->put(tbl, "score", &x, sizeof(int));
  
  // get the value of the element.
  int *px = tbl->get(tbl, "score");
  if(px != NULL) {
    printf("%d\n", *px);
    free(px);
  }
  
  // release table
  tbl->free(tbl);
```

Here is an identical implementation with a Linked-List-Table container.
You may notice that there isn't any code change at all, except for 1 line in the table creation.
This is why qLibc encapsulates corresponding function pointers inside of the container object.

```
  // create a linked-list-table. THE ONLY LINE YOU NEED TO CHANGE.
  qlisttbl_t *tbl = qlisttbl(QLISTTBL_OPT_THREADSAFE);
  
  // add an element which key name is "score".
  int x = 12345;
  tbl->put(tbl, "score", &x, sizeof(int));
  
  // get the value of the element.
  int *px = tbl->get(tbl, "score");
  if(px != NULL) {
    printf("%d\n", *px);             
    free(px);
  }
  
  // release table
  tbl->free(tbl);
```

## Looking for people to work with.

We're looking for people who want to work together to develop and improve qLibc.
Currently, we have high demands on following areas.

* Automated testing
* Documentation.
* New feature implementation.

## Contributors

The following people have helped with suggestions, ideas, code or fixing bugs:
(in alphabetical order by first name)

* Cesar
* Colin
* Dmitry Vorobiev
* HyoSup Woo
* Krishna
* Luis Jimenez
* Maik Beckmann
* RQ
* Umesh
* Seungyoung Kim
* Zhongchao Liu

If we have forgotten or misspelled your name, please let us know.
