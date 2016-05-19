if [ -e "/usr/local/bin/vim" ] ; then
  # Set VIM as default
  alias vi='vim'
fi

# Enable color for git diffs
MORE="-erX" ; export MORE
