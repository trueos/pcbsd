#!/bin/sh

# Only start agents if uid is greater than 1000 (regular user)
# and the .ssh and .gnupg folders are existing in the user's home.
UID="`id -u`"
if [ $UID -gt 1000 ]; then
  # SSH-Agent
  if [ -d "${HOME}/.ssh" ]; then
    SSH_AGENT="/usr/bin/ssh-agent"
    if [ -e ${SSH_AGENT} ]; then
      # Start the agent if it is not running or if the pid variable is missing.
      # In case you are logged in via several terminals there would be an agent
      # running but the pid variable would be empty.
      if [ -z "`pgrep -u $UID ssh-agent`" ] || [ -z ${SSH_AGENT_PID} ]; then
        eval $(ssh-agent)
      fi
    fi
  fi
fi

