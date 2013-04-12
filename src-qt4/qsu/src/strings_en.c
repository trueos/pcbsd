/*
 * Copyright (c) 2012, Guillermo A. Amaral B <g@maral.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GUILLERMO ANTONIO AMARAL BASTIDAS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "strings.h"

const char *gs_usage =
    "Usage: %s [-u <user>] [-d <description>] <command>";
const char *gs_database_authenticated =
    "Previously authenticated!";
const char *gs_default_desc_default =
    "Please enter system administrator password to perform task.";
const char *gs_default_desc_other =
    "Please enter desired user's password to perform task.";
const char *gs_default_title =
    "Authenticating as %1";
const char *gs_default_user =
    "root";
const char *gs_default_user_caption =
    "Name:";
const char *gs_error_access_denied =
    "Access Denied";
const char *gs_error_auth_expired =
    "Expired or invalid authentication token.";
const char *gs_error_auth_failed =
    "User authentication failed.";
const char *gs_error_database_init =
    "Failed to initialize session database!\n"
    "Please check that the following directory exists and that it's owned by root: %s";
const char *gs_error_failed =
    "Failed!";
const char *gs_error_no_display =
    "No X11 display was found.";

