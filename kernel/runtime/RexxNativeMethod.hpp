/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/* REXX Kernel                                         RexxNativeMethod.hpp   */
/*                                                                            */
/* Primitive Native Code Class Definitions                                    */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxNativeCode
#define Included_RexxNativeCode

class RexxNativeCode : public RexxInternalObject {
  public:
   inline void *operator new(size_t size, void *ptr) { return ptr; }
   void        *operator new(size_t size);
   inline void  operator delete(void *) { ; }
   inline void  operator delete(void *, void *) { ; }

   inline RexxNativeCode(RESTORETYPE restoreType) { ; };
   RexxNativeCode(RexxString *, RexxString *, PFN, LONG);
   void        reinit(RexxInteger *);
   void        live();
   void        liveGeneral();
   void        flatten(RexxEnvelope *envelope);
   RexxObject *unflatten(RexxEnvelope *envelope);

   inline PFN         getEntry() { return this->entry; };
   inline void        setEntry(PFN v) { this->entry = v; };
   static void        createClass();
   static void        restoreClass();

protected:
   RexxString *library;               // the library name
   RexxString *procedure;             /* External Procedur name            */
   PFN         entry;                 /* method entry point.               */
   LONG        index;                 /* internal native method            */
};

class RexxNativeCodeClass : public RexxClass {
  public:
   inline RexxNativeCodeClass(RESTORETYPE restoreType) { ; };
   RexxNativeCodeClass();

   void       *operator new(size_t size, void *ptr) { return ptr; };
   void       *operator new(size_t size, size_t size1, const char *className, RexxBehaviour *classBehave, RexxBehaviour *instance) { return new (size, className, classBehave, instance) RexxClass; }
   RexxNativeCode *newClass(RexxString *, RexxString *);

   void        restore();
   void        live();
   void        liveGeneral();
   void        reload(RexxDirectory *);
   RexxDirectory  * load(RexxString *);
   RexxNativeCode * newInternal(LONG);
   inline RexxDirectory  * getLibraries() { return this->libraries; };

protected:

   RexxDirectory *libraries;           /* directory of loaded libraries     */
};
#endif
