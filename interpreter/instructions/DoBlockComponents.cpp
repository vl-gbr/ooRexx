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
/* Primitive classes for implementing various DO loop types in instructions.  */
/*                                                                            */
/******************************************************************************/
#include "DoBlockComponents.hpp"
#include "DoInstruction.hpp"
#include "DoBlock.hpp"
#include "RexxActivation.hpp"
#include "MethodArguments.hpp"

/**
 * Set up for execution of a FOR loop.
 *
 * @param context The current execution context.
 * @param stack   The current evaluation stack.
 * @param doblock The context doblock useds to store loop state data.
 */
void ForLoop::setup(RexxActivation *context,
        RexxExpressionStack *stack, RexxDoBlock *doblock)
{
    // we might not have anything here, but we need to set
    // a marker in the doblock so we know not to use this
    if (forCount == OREF_NULL)
    {
        // set this to a negative value to indicate not to use this.
        doblock->setFor(SIZE_MAX);
        return;
    }

    // get the expression value.  This must be a whole number, so we need to
    // validate and convert now.
    wholenumber_t count = 0;
    RexxObject *result = forCount->evaluate(context, stack);

    // if this is an integer value already and we're at the default digits setting,
    // we should be able to use this directly.
    if (isOfClass(Integer, result) && context->digits() >= Numerics::DEFAULT_DIGITS)
    {
        // get the value directly and trace
        count = ((RexxInteger *)result)->getValue();
        context->traceResult(result);
    }
    else
    {
        // first get the string version and force numeric rounding rules.
        RexxString *strResult = REQUEST_STRING(result);
        /* force rounding                    */
        RexxObject *rounded = result->callOperatorMethod(OPERATOR_PLUS, OREF_NULL);
        context->traceResult(rounded);
        // now convert the rounded value to an integer, if possible
        if (!rounded->requestNumber(count, number_digits()))
        {
            // use original object in the error report.
            reportException(Error_Invalid_whole_number_for, result);
        }
    }

    // This must be a non-negative value.
    if (count < 0)
    {
        /* report an exception               */
        reportException(Error_Invalid_whole_number_for, result);
    }

    // set this value in the doblock
    doblock->setFor((size_t)count);
}


/**
 * Set up for execution of a controlled loop.
 *
 * @param context The current execution context.
 * @param stack   The current evaluation stack.
 * @param doblock The context doblock useds to store loop state data.
 */
void ControlledLoop::setup( RexxActivation *context,
     RexxExpressionStack *stack, RexxDoBlock *doblock)
{
    // evaluate the initial expression
    RexxObject *_initial = initial->evaluate(context, stack);

    // force rounding by adding zero to this
    _initial = _initial->callOperatorMethod(OPERATOR_PLUS, OREF_NULL);
    // now process each of the expressions.  the expressions
    // array allows us to process these in the order they were specified on
    // the instruction
    for (size_t i = 0; i < 3 && expressions[i] != 0; i++)
    {
        switch (expressions[i])
        {
            // The TO expression;
            case EXP_TO:
            {
                // get the too value and round...which has the side effect
                // of also validating that this is a valid numeric.
                RexxObject *result = to->evaluate(context, stack);
                // prefix + is like adding zero
                result = result->callOperatorMethod(OPERATOR_PLUS, OREF_NULL);

                // if the result is a string, see if we can convert this to
                // an integer value.  This is very common in loops, and can
                // save us a lot of processing on each loop iteration.
                RexxObject *temp = result->integerValue(number_digits());
                if (temp != TheNilObject)
                {
                    result = temp;
                }
                // this value gets saved in the doblock as state data we can reuse.
                doblock->setTo(result);
                break;
            }

            // BY expression
            case EXP_BY:
            {
                // get the expression value and round
                RexxObject *result = by->evaluate(context, stack);
                result = result->callOperatorMethod(OPERATOR_PLUS, OREF_NULL);
                // this gets saved in the doblock
                doblock->setBy(result);

                // now we need to check if this is a negative value so set know how to
                // compare.
                if (result->callOperatorMethod(OPERATOR_LESSTHAN, IntegerZero) == TheTrueObject)
                {
                    // we're counting down, so check less than for termination
                    doblock->setCompare(OPERATOR_LESSTHAN);
                }
                else
                {
                    // counting up...compare greater than
                    doblock->setCompare(OPERATOR_GREATERTHAN);
                }
                break;
            }

            // FOR expression...does a binary count
            // our superclass can handle that one.
            case EXP_FOR:
                ForLoop::setup(context, stack, doblock);
                break;
        }
    }
    // if the loop did not specify a BY expression, we use a default increment of 1.
    // and we are counting upward, so use a greater than comparison
    if (by == OREF_NULL)
    {
        doblock->setBy(IntegerOne);
        doblock->setCompare(OPERATOR_GREATERTHAN);
    }

    // set the control variable in the doblock (it will use it on subsequent passes).
    doblock->setControl(control);
    // do the initial assignment
    control->assign(context, _initial);
}



/**
 * Set up for execution of a DO OVER loop.
 *
 * @param context The current execution context.
 * @param stack   The current evaluation stack.
 * @param doblock The context doblock useds to store loop state data.
 */
void OverLoop::setup( RexxActivation *context,
     RexxExpressionStack *stack, RexxDoBlock *doblock)
{
    // evaluate the array target
    RexxObject* result = target->evaluate(context, stack);
    // anchor immediately to protect from GC
    doblock->setTo(result);

    context->traceResult(result);
    // if this is already an array item, request the non-sparse version

    RexxArray *array;
    if (isOfClass(Array, result))
    {
        array = ((RexxArray *)result)->makeArray();
    }
    else
    {
        // some other type of collection, use the less direct means
        // of requesting an array
        array = REQUEST_ARRAY(result);
        // raise an error if this did not convert ok, or we got
        // back something other than a real Rexx array.
        if (array == TheNilObject || !isOfClass(Array, array) )
        {
            reportException(Error_Execution_noarray, result);
        }
    }

    // we use the TO field to store the array, and the for
    // counter is our index position.
    doblock->setTo(array);
    doblock->setFor(1);
}


/**
 * Evaluate a WHILE condition, return true if the condition
 * should continue and false if the WHILE condition failed.
 *
 * @param context The current execution context.
 * @param stack   The current evaluation stack.
 *
 * @return true if the WHILE condition passes, false if it fails.
 */
bool WhileUntilLoop::checkWhile(RexxActivation *context, RexxExpressionStack *stack )
{
    // evaluate the condition and trace
    RexxObject *result = conditional->evaluate(context, stack);
    context->traceResult(result);

    // most comparisons return either true or false directly, so we
    // can optimize this check.  WHILE conditions are more likely to
    // evaluate to true, so we'll check that first
    if (result == TheTrueObject)
    {
        return true;
    }
    else if (result == TheFalseObject)
    {
        return false;
    }
    // This is some sort of computed boolean, so we need to do a real
    // validation on this
    return result->truthValue(Error_Logical_value_while);
}


/**
 * Evaluate a UNTIL condition, return true if the condition is
 * true or false if it evaluates false.  The meaning of these is
 * the reverse for the UNTIL.  This is optimized for the UNTIL
 * cases.
 *
 * @param context The current execution context.
 * @param stack   The current evaluation stack.
 *
 * @return true if the WHILE condition passes, false if it fails.
 */
bool WhileUntilLoop::checkUntil(RexxActivation *context, RexxExpressionStack *stack )
{
    // evaluate the condition and trace
    RexxObject *result = conditional->evaluate(context, stack);
    context->traceResult(result);

    // most comparisons return either true or false directly, so we
    // can optimize this check.  UNTIL conditions are more likely to
    // evaluate to false, so we'll check that first
    if (result == TheFalseObject)
    {
        return false;
    }
    else if (result == TheTrueObject)
    {
        return true;
    }
    // This is some sort of computed boolean, so we need to do a real
    // validation on this
    return result->truthValue(Error_Logical_value_until);
}
