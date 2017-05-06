\ Copyright (c) 2003 Scott Long <scottl@freebsd.org>
\ Copyright (c) 2003 Aleksander Fafula <alex@fafula.com>
\ All rights reserved.
\
\ Redistribution and use in source and binary forms, with or without
\ modification, are permitted provided that the following conditions
\ are met:
\ 1. Redistributions of source code must retain the above copyright
\    notice, this list of conditions and the following disclaimer.
\ 2. Redistributions in binary form must reproduce the above copyright
\    notice, this list of conditions and the following disclaimer in the
\    documentation and/or other materials provided with the distribution.
\
\ THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
\ ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
\ IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
\ ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
\ FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
\ DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
\ OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
\ HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
\ LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
\ OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
\ SUCH DAMAGE.
\
\ $FreeBSD: src/sys/boot/forth/beastie.4th,v 1.10.2.2 2006/04/04 17:03:44 emax Exp $

marker task-beastie.4th

include /boot/screen.4th
include /boot/frames.4th

hide

variable menuidx
variable menubllt
variable menuX
variable menuY
variable promptwidth

variable bootkey
variable bootacpikey
variable bootsafekey
variable bootverbosekey
variable rundisplaywiz
variable bootsinglekey
variable escapekey
variable rebootkey
variable xvesa
variable xvesa_set

46 constant dot

\ The BSD Daemon.  He is 19 rows high and 34 columns wide
: beastie-logo ( x y -- )
2dup at-xy ."               [1;31m,        ," 1+
2dup at-xy ."              /(        )`" 1+
2dup at-xy ."              \ \___   / |" 1+
2dup at-xy ."              /- [37m_[31m  `-/  '" 1+
2dup at-xy ."             ([37m/\/ \[31m \   /\" 1+
2dup at-xy ."             [37m/ /   |[31m `    \" 1+
2dup at-xy ."             [34mO O   [37m) [31m/    |" 1+
2dup at-xy ."             [37m`-^--'[31m`<     '" 1+
2dup at-xy ."            (_.)  _  )   /" 1+
2dup at-xy ."             `.___/`    /       " 1+
2dup at-xy ."               `-----' /" 1+
2dup at-xy ."  [33m<----.[31m     __ / __   \" 1+
2dup at-xy ."  [33m<----|====[31mO)))[33m==[31m) \) /[33m====|" 1+
2dup at-xy ."  [33m<----'[31m    `--' `.__,' \" 1+
2dup at-xy ."               |        |" 1+
2dup at-xy ."                \       /       /\" 1+
2dup at-xy ."           [36m______[31m( (_  / \______/" 1+
2dup at-xy ."         [36m,'  ,-----'   |" 1+
at-xy ."         `--{__________) [0m"
;

: beastiebw-logo ( x y -- )
	2dup at-xy ."              ,        ," 1+
	2dup at-xy ."             /(        )`" 1+
	2dup at-xy ."             \ \___   / |" 1+
	2dup at-xy ."             /- _  `-/  '" 1+
	2dup at-xy ."            (/\/ \ \   /\" 1+
	2dup at-xy ."            / /   | `    \" 1+
	2dup at-xy ."            O O   ) /    |" 1+
	2dup at-xy ."            `-^--'`<     '" 1+
	2dup at-xy ."           (_.)  _  )   /" 1+
	2dup at-xy ."            `.___/`    /" 1+
	2dup at-xy ."              `-----' /" 1+
	2dup at-xy ." <----.     __ / __   \" 1+
	2dup at-xy ." <----|====O)))==) \) /====" 1+
	2dup at-xy ." <----'    `--' `.__,' \" 1+
	2dup at-xy ."              |        |" 1+
	2dup at-xy ."               \       /       /\" 1+
	2dup at-xy ."          ______( (_  / \______/" 1+
	2dup at-xy ."        ,'  ,-----'   |" 1+
	     at-xy ."        `--{__________)"
;

: fbsdbw-logo ( x y -- )
	2dup at-xy ."     ____  ______" 1+
	2dup at-xy ."    / __ \/ ____/" 1+
	2dup at-xy ."   / /_/ / /" 1+
	2dup at-xy ."  / ____/ /___" 1+
	2dup at-xy ." /_/    \____/" 1+
	2dup at-xy ."     ____ _____ ____" 1+
	2dup at-xy ."    / __ ) ___// __ \" 1+
	2dup at-xy ."   / __  \__ \/ / / /" 1+
	2dup at-xy ."  / /_/ /__/ / /_/ /" 1+
	     at-xy ." /_____/____/_____/"
;

: print-logo ( x y -- )
	s" loader_logo" getenv
	dup -1 = if
		drop
		fbsdbw-logo
		exit
	then
	2dup s" fbsdbw" compare-insensitive 0= if
		2drop
		fbsdbw-logo
		exit
	then
	2dup s" beastiebw" compare-insensitive 0= if
		2drop
		beastiebw-logo
		exit
	then
	2dup s" beastie" compare-insensitive 0= if
		2drop
		beastie-logo
		exit
	then
	2dup s" none" compare-insensitive 0= if
		2drop
		\ no logo
		exit
	then
	2drop
	fbsdbw-logo
;

: acpienabled? ( -- flag )
	s" acpi_load" getenv
	dup -1 = if
		drop false exit
	then
	s" YES" compare-insensitive 0<> if
		false exit
	then
	s" hint.acpi.0.disabled" getenv
	dup -1 <> if
		s" 0" compare 0<> if
			false exit
		then
	else
		drop
	then
	true
;

: printmenuitem ( -- n )
	menuidx @
	1+ dup
	menuidx !
	menuY @ + dup menuX @ swap at-xy
	menuidx @ .
	menuX @ 1+ swap at-xy
	menubllt @ emit
	menuidx @ 48 +
;

: beastie-menu ( -- )
	0 menuidx !
	dot menubllt !
	8 menuY !
	5 menuX !
	clear
	46 4 print-logo
	43 22 2 2 box
	13 6 at-xy ." Welcome to TrueOS!"
	printmenuitem ."  Boot TrueOS [default]" bootkey !
	s" arch-i386" environment? if
		drop
		printmenuitem ."  Boot TrueOS with ACPI " bootacpikey !
		acpienabled? if
			." disabled"
		else
			." enabled"
		then
	else
		-2 bootacpikey !
	then
	printmenuitem ."  Boot TrueOS in Safe Mode" bootsafekey !
	printmenuitem ."  Boot TrueOS in single user mode" bootsinglekey !
	printmenuitem ."  Boot TrueOS with verbose logging" bootverbosekey !
	printmenuitem ."  Run X in VESA mode" xvesa !
	printmenuitem ."  Escape to loader prompt" escapekey !
	printmenuitem ."  Reboot" rebootkey !
	menuX @ 20 at-xy
	." Select option, [Enter] for default"
	menuX @ 21 at-xy
	s" or [Space] to pause timer    " dup 2 - promptwidth !
	type
;

: tkey
	seconds +
	begin 1 while
		over 0<> if
			dup seconds u< if
				drop
				-1
				exit
			then
			menuX @ promptwidth @ + 21 at-xy dup seconds - .
		then
		key? if
			drop
			key
			exit
		then
	50 ms
	repeat
;

set-current

: beastie-start
	s" beastie_disable" getenv
	dup -1 <> if
		s" YES" compare-insensitive 0= if
			exit
		then
	else
		drop
	then
	s" NO" s" xvesa" setenv
	0 xvesa_set !
	beastie-menu
	s" autoboot_delay" getenv
	dup -1 = if
		drop
		10
	else
		0 0 2swap >number drop drop drop
	then
	begin
		dup tkey
		0 25 at-xy
		dup 32 = if nip 0 swap then
		dup -1 = if 0 boot then
		dup 13 = if 0 boot then
		dup bootkey @ = if 0 boot then
		dup bootacpikey @ = if
			acpienabled? if
				s" acpi_load" unsetenv
				s" 1" s" hint.acpi.0.disabled" setenv
				s" 1" s" loader.acpi_disabled_by_user" setenv
			else
				s" YES" s" acpi_load" setenv
				s" 0" s" hint.acpi.0.disabled" setenv
			then
			0 boot
		then
		dup bootsafekey @ = if
			s" arch-i386" environment? if
				drop
				s" acpi_load" unsetenv
				s" 1" s" hint.acpi.0.disabled" setenv
				s" 1" s" loader.acpi_disabled_by_user" setenv
				s" 1" s" hint.apic.0.disabled" setenv
			then
			s" 0" s" hw.ata.ata_dma" setenv
			s" 0" s" hw.ata.atapi_dma" setenv
			s" 0" s" hw.ata.wc" setenv
			s" 0" s" hw.eisa_slots" setenv
			s" 1" s" hint.kbdmux.0.disabled" setenv
			0 boot
		then
		dup bootverbosekey @ = if
			s" YES" s" boot_verbose" setenv
			0 boot
		then
		dup xvesa @ = if
			xvesa_set @ 1 = if
				menuX @ 23 at-xy
				." xvesa unset" 
				s" NO" s" xvesa" setenv
				0 xvesa_set !
			else
				menuX @ 23 at-xy
				." xvesa set  " 
				s" YES" s" xvesa" setenv
				1 xvesa_set !
			then
		then
		dup rundisplaywiz @ = if
			s" /boot/kernel;/boot/modules;/RUNXWIZ/" s" module_path" setenv
			0 boot
		then
		dup bootsinglekey @ = if
			s" YES" s" boot_single" setenv
			0 boot
		then
		dup escapekey @ = if
			2drop
			s" NO" s" autoboot_delay" setenv
			exit
		then
		rebootkey @ = if 0 reboot then
	again
;

previous
