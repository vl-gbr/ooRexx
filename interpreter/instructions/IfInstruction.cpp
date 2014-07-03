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
/* REXX Translator                                                            */
/*                                                                            */
/* IF instruction executable class                                            */
/*                                                                            */
/******************************************************************************/
#include <stdlib.h>
#include "RexxCore.h"
#include "RexxActivation.hpp"
#include "IfInstruction.hpp"
#include "Token.hpp"


/**
 * Construct an IF instructon instance.
 *
 * @param _condition The condition expression to evaluate.
 * @param thenToken  The token for the terminating THEN keyword.
 *                   This is where we mark the end of the
 *                   instruction.
 */
RexxInstructionIf::RexxInstructionIf(RexxObject *_condition, RexxToken *thenToken)
{
    condition = _condition;
    //get the location from the THEN token and use its location to set
    // the end of the instruction.  Note that the THEN is traced on its
    // own, but using the start of the THEN gives a fuller picture of things.
    SourceLocation location = thenToken->getLocation();
    this->setEnd(location.getLineNumber(), location.getOffset());
}

/**
 * Set the END location for the false branch of an IF
 * instruction.  This will either be an ELSE clause, or
 * the instruction following the instruction on the THEN.
 *
 * @param end_target The new end instruction.
 */
void RexxInstructionIf::setEndInstruction(RexxInstructionEndIf *end_target)
{
    else_location = end_target;
}


/**
 * Perform garbage collection on a live object.
 *
 * @param liveMark The current live mark.
 */
void RexxInstructionIf::live(size_t liveMark)
{
    // must be first object marked.
    memory_mark(nextInstruction);
    memory_mark(condition);
    memory_mark(else_location);
}


/**
 * Perform generalized live marking on an object.  This is
 * used when mark-and-sweep processing is needed for purposes
 * other than garbage collection.
 *
 * @param reason The reason for the marking call.
 */
void RexxInstructionIf::liveGeneral(int reason)
{
    // must be first object marked.
    memory_mark_general(nextInstruction);
    memory_mark_general(condition);
    memory_mark_general(else_location);
}


/**
 * Flatten a source object.
 *
 * @param envelope The envelope that will hold the flattened object.
 */
void RexxInstructionIf::flatten(RexxEnvelope *envelope)
{
    setUpFlatten(RexxInstructionIf)

    flattenRef(nextInstruction);
    flattenRef(condition);
    flattenRef(else_location);

    cleanUpFlatten
}


/**
 * Execute an IF/WHEN instruction.
 *
 * @param context The current execution context.
 * @param stack   The current evaluation stack.
 */
void RexxInstructionIf::execute(RexxActivation *context, RexxExpressionStack *stack)
{
    context->traceInstruction(this);

    // evaluate and trace the condition expression.
    RexxObject *result = condition->evaluate(context, stack);
    context->traceResult(result);

    // the comparison methods return either .true or .false, so we
    // can to a quick test against those.
    if (result == TheFalseObject)
    {
        // we execute the ELSE branch
        context->setNext(else_location->nextInstruction);
    }

    // if it is not the .true object, we need to perform a fuller
    // evaluation of the result.
    else if (result != TheTrueObject)
    {
        // evaluate and decide if we take the ELSE branch
        if (!result->truthValue(Error_Logical_value_if))
        {
            context->setNext(else_location->nextInstruction);
        }
    }

    // We do nothing for true...we just continue on to the next instruction.

    context->pauseInstruction();
}


/**
 * Execute a WHEN instruction attached to a SELECT CASE.
 *
 * @param context The current execution context.
 * @param stack   The current evaluation stack.
 */
void RexxInstructionCaseWhen::execute(RexxActivation *context, RexxExpressionStack *stack)
{
    context->traceInstruction(this);

    // This should be us.  It really isn't possible to jump into a middle of a select
    // and get to a WHEN without raising an error.
    RexxDoBlock *doBlock = context->topBlock();
    // get the case expression
    RexxObject *caseValue = doBlock->getCase();
    // and the compare target (which needs tracing, but only as an intermediate
    RexxObject *compareValue = condition->evaluate(context, stack)
    context->traceIntermediate(compareValue);

    // now perform the compare using the "==" operator method.
    // NOTE that the case value is the left hand side.
    RexxObject *result = callOperatorMethod(caseValue, OPERATOR_PLUS, compareValue);
    context->traceResult(result);

    // the comparison methods return either .true or .false, so we
    // can to a quick test against those.
    if (result == TheFalseObject)
    {
        // we execute the ELSE branch
        context->setNext(else_location->nextInstruction);
    }

    // if it is not the .true object, we need to perform a fuller
    // evaluation of the result.
    else if (result != TheTrueObject)
    {
        // evaluate and decide if we take the ELSE branch
        if (!result->truthValue(Error_Logical_value_if))
        {
            context->setNext(else_location->nextInstruction);
        }
    }

    // We do nothing for true...we just continue on to the next instruction.

    context->pauseInstruction();
}

