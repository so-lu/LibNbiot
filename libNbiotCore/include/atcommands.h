/* ========================================================================
 * LibNbiot: atcommands.h
 *
 * Copyright (c) 2018, Edgar Hindemith, Yassine Amraue, Thorsten
 * Krautscheid, Kolja Vornholt, T-Systems International GmbH
 * contact: libnbiot@t-systems.com, opensource@telekom.de
 *
 * This file is distributed under the conditions of the Apache License,
 * Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * For details see the file LICENSE at the toplevel.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================================
*/

#ifndef ATCOMMANDS_H
#define ATCOMMANDS_H
#include "nbiotstring.h"
#include "serial.h"

class Serial;

typedef enum expectedReply
{
    REPLY_OK,       /* just "OK" */
    REPLY_ANY,      /* any response-line followed by "OK" */
    REPLY_COPY,     /* all response-lines will be copied to 'response' */
    REPLY_EXACT,    /* exactly one response-line that differs from "OK" */
    REPLY_IGNORE,   /* every response will be ignored */
    REPLY_NONE      /* there must be no response */
} ExpectedReply;

class AtCommands
{
public:
    explicit AtCommands(Serial& s);
    virtual ~AtCommands();

    bool sendCommand(const char* command);
    bool sendCommand(const nbiot::string& cmd);
    bool readResponse(ExpectedReply expected, unsigned short timeout_ms=defaultTimeout);
    bool readUntil(const char* expected, unsigned short timeout_ms=defaultTimeout, bool exact=false);
    const char* getResponse() const { return response.c_str(); }

    template<class UHC>
    bool addUrcFilter(const char* urc, UHC* uhc, void (UHC::*urc_handler)(const char*))
    {
        return serial.addUrcFilter(urc, uhc, urc_handler);
    }

    bool addUrcFilter(const char* urc, void (*urc_handler)(const char*))
    {
        return serial.addUrcFilter(urc, urc_handler);
    }

    bool removeUrcFilter(const char* urc)
    {
        return serial.removeUrcFilter(urc);
    }

    void clearFilter()
    {
        serial.clearFilter();
    }

    bool hasFilter() const
    {
        return serial.hasFilter();
    }

    //! @todo configurable response-size-limit for REPLY_COPY
    
    int dbg_prefix;
    Serial &serial;


private:
    AtCommands(const AtCommands&);
    AtCommands& operator=(const AtCommands&);


    nbiot::string response;

    static const unsigned short defaultTimeout = 500;
    static const unsigned long oneSecond = 1000;
    static const nbiot::string okReply;
    static const nbiot::string errReply;
    static const unsigned short replyMaxLen = 255;
    static const size_t PRINTF_BUFFER_SIZE = 80;
};

#endif // ATCOMMANDS_H

