#!/bin/sh

# Only start agents if uid is greater than 1000 (regular user)
# and the .ssh and .gnupg folders are existing in the user's home.
UID="`id -u`"
if [ $UID -gt 1000 ]; then
  # GPG-Agent
  if [ -d "${HOME}/.gnupg" ]; then 
    GPG_AGENT="/usr/local/bin/gpg-agent"
    export GPG_TTY=$(tty)
    if [ -e $GPG_AGENT ] && [ -z "`pgrep -u $UID gpg-agent`" ]; then
      eval $($GPG_AGENT --daemon --write-env-file "${HOME}/.gpg-agent-info" --log-file "${HOME}/.gnupg/gpg-agent.log")
    elif [ ! -z "`pgrep -u $UID gpg-agent`" ] && [ -f "${HOME}/.gpg-agent-info" ]; then
      source "${HOME}/.gpg-agent-info"
      export GPG_AGENT_INFO
    fi
  fi
fi

