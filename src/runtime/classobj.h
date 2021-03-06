// Copyright (c) 2014-2016 Dropbox, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PYSTON_RUNTIME_CLASSOBJ_H
#define PYSTON_RUNTIME_CLASSOBJ_H

#include "runtime/types.h"

namespace pyston {

/*
 * Class objects refer to Python's old-style classes that don't inherit from
 * `object`. This, classes declared as:
 *
 * class Foo:
 *  ...
 *
 * When debugging, "obj->cls->tp_name" will have value "instance" for all
 * old-style classes rather than the name of the class itself.
 */

void setupClassobj();

class BoxedClass;
class BoxedClassobj;
class BoxedInstance;
extern "C" {
extern BoxedClass* classobj_cls, *instance_cls;
}

class BoxedClassobj : public Box {
public:
    HCAttrs attrs;

    BoxedTuple* bases;
    BoxedString* name;

    Box** weakreflist;

    BoxedClassobj(BoxedString* name, BoxedTuple* bases) : bases(bases), name(name) {
        Py_INCREF(name);
        Py_INCREF(bases);
    }

    static void dealloc(Box* b) noexcept;
    static int traverse(Box* self, visitproc visit, void* arg) noexcept;
    static int clear(Box* self) noexcept;
};

class BoxedInstance : public Box {
public:
    HCAttrs attrs;

    BoxedClassobj* inst_cls;

    Box** weakreflist;

    BoxedInstance(BoxedClassobj* inst_cls) : inst_cls(inst_cls) { Py_INCREF(inst_cls); }

    DEFAULT_CLASS(instance_cls);

    static void dealloc(Box* b) noexcept;
    static int traverse(Box* self, visitproc visit, void* arg) noexcept;
    static int clear(Box* self) noexcept;
};

Box* instance_getattro(Box* cls, Box* attr) noexcept;
int instance_setattro(Box* cls, Box* attr, Box* value) noexcept;
class GetattrRewriteArgs;
template <ExceptionStyle S>
Box* instanceGetattroInternal(Box* self, Box* attr, GetattrRewriteArgs* rewrite_args) noexcept(S == CAPI);
void instanceSetattroInternal(Box* self, STOLEN(Box*) attr, Box* val, SetattrRewriteArgs* rewrite_args);
}

#endif
