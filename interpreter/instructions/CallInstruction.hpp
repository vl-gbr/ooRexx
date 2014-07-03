/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2014 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
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
/* REXX Kernel                                           CallInstruction.hpp  */
/*                                                                            */
/* Primitive CALL instruction Class Definitions                               */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxInstructionCall
#define Included_RexxInstructionCall

#include "RexxInstruction.hpp"
#include "Token.hpp"


/**
 * Base class for all instruction types that need a
 * resolution step (i.e., SIGNAL and CALL).
 */
class RexxInstructionCallBase : public RexxInstruction
{
 public:
    inline RexxInstructionCallBase() { ; };
    // subclasses must provide this
    virtual void resolve(RexxDirectory *) = 0;

 protected:

    RexxString      *targetName;              // name to call
    RexxInstruction *targetInstruction;       // resolved instruction target if this is internal
};


/**
 * Base class for instruction types that resolve a target by
 * evaluating at run time (i.e, SIGNAL VALUE or CALL (expr)
 */
class RexxInstructionDynamicCallBase : public RexxInstruction
{
 public:
    inline RexxInstructionDynamicCallBase() { ; };

 protected:

    RexxObject      *dynamicName;             // we don't have a static name or resolved target for this.
};


/**
 * Base class for SIGNAL/CALL ON trap instructions.
 */
class RexxInstructionTrapBase : public RexxInstructionCallBase
{
 public:
    inline RexxInstructionTrapBase() { ; };
    virtual void trap(RexxActivation *, RexxDirectory *) { ; };

 protected:

    RexxString *conditionName;         // the condition trap name
};


/**
 * A typical call instruction.  This only handles
 * the static CALL cases, not CALL ON/OFF
 */
class RexxInstructionCall : public RexxInstructionCallBase
{
 public:

    inline void *operator new(size_t size, void *ptr) {return ptr;}
    inline void operator delete(void *) { }
    inline void operator delete(void *, void *) { }

    RexxInstructionCall(RexxObject *, RexxString *, size_t, RexxQueue *, bool, BuiltinCode);
    inline RexxInstructionCall(RESTORETYPE restoreType) { ; };

    virtual void live(size_t);
    virtual void liveGeneral(int reason);
    virtual void flatten(RexxEnvelope*);

    virtual void execute(RexxActivation *, RexxExpressionStack *);
    virtual void resolve(RexxDirectory *);

protected:

    BuiltinCode  builtinIndex;           // builtin function index
    size_t       argumentCount;          // number of arguments
    RexxObject  *arguments[1];           // argument list
};


/**
 * A call instruction that resolves the target at runtime
 * based on a resolved expression.
 */
class RexxInstructionDynamicCall : public RexxInstructionDynamicCallBase
{
 public:

    inline void *operator new(size_t size, void *ptr) {return ptr;}
    inline void operator delete(void *) { }
    inline void operator delete(void *, void *) { }

    RexxInstructionDynamicCall(RexxObject *, size_t, RexxQueue *);
    inline RexxInstructionDynamicCall(RESTORETYPE restoreType) { ; };

    virtual void live(size_t);
    virtual void liveGeneral(int reason);
    virtual void flatten(RexxEnvelope*);

    virtual void execute(RexxActivation *, RexxExpressionStack *);

protected:

    size_t       argumentCount;          // number of arguments
    RexxObject  *arguments[1];           // argument list
};


/**
 * An instruction object to handle the basics of the
 * CALL ON/OFF instruction.
 */
class RexxInstructionCallOn : public RexxInstructionTrapBase
{
 public:

    inline void *operator new(size_t size, void *ptr) {return ptr;}
    inline void operator delete(void *) { }
    inline void operator delete(void *, void *) { }

    RexxInstructionCallOn(RexxString*, RexxString *);
    inline RexxInstructionCallOn(RESTORETYPE restoreType) { ; };

    void live(size_t);
    void liveGeneral(int reason);
    void flatten(RexxEnvelope*);

    void execute(RexxActivation *, RexxExpressionStack *);
    void resolve(RexxDirectory *);

protected:

    BuiltinCode  builtinIndex;           // builtin function index
};
#endif
