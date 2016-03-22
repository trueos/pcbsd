#!/bin/sh

# Only start agents if uid is greater than 1000 (regular user)
# and the .gnupg folder existd in the user's home.
# If .ssh folder also exists in the user's home, enable ssh-agent fuctionality.

UID=$(id -u)
if [ $UID -gt 1000 ]; then
  # GPG-Agent
  if [ -d "${HOME}/.gnupg" ]; then 
    GPG_AGENT="/usr/local/bin/gpg-agent"
    export GPG_TTY=$(tty)
    gpg_options=
    [ -d "${HOME}/.ssh" ] && gpg_options=--enable-ssh-support
    if [ -x "${GPG_AGENT}" ] && [ -z $(pgrep -u "${UID}" gpg-agent) ]; then
      $GPG_AGENT -s --daemon ${gpg_options} --log-file "${HOME}/.gnupg/gpg-agent.log" > "${HOME}/.gpg-agent-info"
      . "${HOME}/.gpg-agent-info"
    elif [ ! -z $(pgrep -u $UID gpg-agent) ] && [ -f "${HOME}/.gpg-agent-info" ]; then
      . "${HOME}/.gpg-agent-info"
    fi
    unset gpg_options
  fi
fi
