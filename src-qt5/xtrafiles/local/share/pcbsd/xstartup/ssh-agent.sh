#!/bin/sh

# Only start agents if uid is greater than 1000 (regular user)
# and the .ssh folder exists in the user's home.

# Do NOT start if gpg-agent executabl exists and .gnupg also exists in
# the user's home as gpg-agent provides this functionality. 

UID=$(id -u)
if [ $UID -gt 1000 ]; then
  # SSH-Agent
  if [ -d "${HOME}/.ssh" ]; then
    SSH_AGENT="/usr/bin/ssh-agent"
    GPG_AGENT="/usr/local/bin/gpg-agent"
    if [ -x "${SSH_AGENT}" -a ! '(' -x "${GPG_AGENT}" -a -d "${HOME}/.gnupg" ')' ]; then
      # Start the agent if it is not running or if the pid variable is missing.
      # In case you are logged in via several terminals there would be an agent
      # running but the pid variable would be empty.
      if [ -z $(pgrep -u "${UID}" ssh-agent) ] || [ -z "${SSH_AGENT_PID}" ]; then
          eval $(ssh-agent)
      fi
    fi
  fi
fi
