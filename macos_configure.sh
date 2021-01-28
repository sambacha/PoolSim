#!/bin/bash
# Commands also provided by macOS have been installed with the prefix "g".
# If you need to use these commands with their normal names, you
# can add a "gnubin" directory to your PATH from your bashrc like:
# export PATH="$(brew --prefix coreutils)/libexec/gnubin:/usr/local/bin:$PATH"
echo "==> Installing GNU Coreutils via Homevrew..."
sleep 1

command -v brew >/dev/null 2>&1 || { echo >&2 "Installing Homebrew Now";  /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"; }
PATH="$(brew --prefix coreutils)/libexec/gnubin:/usr/local/bin:$PATH"
export PATH

brew install coreutils
brew install gnu-tar --with-default-names
echo "==> GNU coreutils installed, add the letter 'g' to commands, example: gtar for gnu-tar"
