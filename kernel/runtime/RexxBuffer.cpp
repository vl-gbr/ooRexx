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
/* REXX Kernel                                                RexxBuffer.c    */
/*                                                                            */
/* Primitive Buffer Class                                                     */
/*                                                                            */
/******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "RexxCore.h"
#include "RexxActivity.hpp"
#include "ActivityManager.hpp"
#include "RexxBuffer.hpp"

RexxBuffer *RexxBuffer::expand(
    size_t length)                     /* minimum space needed              */
/******************************************************************************/
/* Function:  Create a larger buffer and copy existing data into it           */
/******************************************************************************/
{
  RexxBuffer * newBuffer;              /* returned new buffer               */

                                       /* we will either return a buffer    */
                                       /* twice the size of the current     */
                                       /* buffer, or this size of           */
                                       /* current(this)buffer + requested   */
                                       /* minimum length.                   */
  if (length > this->getLength())      /* need more than double?            */
                                       /* increase by the requested amount  */
    newBuffer = new_buffer(this->getLength() + length);
  else                                 /* just double the existing length   */
    newBuffer = new_buffer(this->getLength() * 2);
                                       /* have new buffer, so copy data from*/
                                       /* current buffer into new buffer.   */
  memcpy(newBuffer->address(), this->data, this->getLength());
  return newBuffer;                    /* all done, return new buffer       */

}

void *RexxBuffer::operator new(size_t size,
    size_t length)                     /* buffer length                     */
/******************************************************************************/
/* Function:  Create a new buffer object                                      */
/******************************************************************************/
{
  RexxBuffer *newBuffer;               /* new object                        */

                                       /* Get new object                    */
  newBuffer = (RexxBuffer *) new_object(size + length - sizeof(char[4]));
                                       /* Give new object its behaviour     */
  newBuffer->setBehaviour(TheBufferBehaviour);
                                       /* Initialize this new buffer        */
  newBuffer->size = length;            /* set the length of the buffer      */
  newBuffer->setHasNoReferences();     /* this has no references            */
  return (void *)newBuffer;            /* return the new buffer             */
}

#include "RexxNativeAPI.h"

#define this ((RexxBuffer *)self)

char *REXXENTRY REXX_BUFFER_ADDRESS(REXXOBJECT self)
/******************************************************************************/
/* Function:  External interface to the object method                         */
/******************************************************************************/
{
/******************************************************************************/
/* NOTE:  This method does not reaquire kernel access                         */
/******************************************************************************/
  return this->address();              /* just return this directly         */
}

size_t REXXENTRY REXX_BUFFER_LENGTH(REXXOBJECT self)
/******************************************************************************/
/* Function:  External interface to the object method                         */
/******************************************************************************/
{
/******************************************************************************/
/* NOTE:  This method does not reaquire kernel access                         */
/******************************************************************************/
  return this->getLength();               /* just return this directly         */
}

REXXOBJECT REXXENTRY REXX_BUFFER_EXTEND(REXXOBJECT self, size_t length)
/******************************************************************************/
/* Function:  Extend the length of a buffer                                   */
/******************************************************************************/
{
  native_entry;                        /* synchronize access                */
                                       /* return new expanded buffer        */
  return_object((RexxObject *)this->expand(length));
}

#undef RexxBuffer

REXXOBJECT REXXENTRY REXX_BUFFER_NEW(size_t length)
/******************************************************************************/
/* Function:  External interface to the nativeact object method               */
/******************************************************************************/
{
  native_entry;                        /* synchronize access                */
                                       /* just forward and return           */
  return_object((RexxObject *)new (length) RexxBuffer());
}
