#!/bin/sh

travis_retry() {
  local result=0
  local count=1
  while [ $count -le 3 ]; do
    [ $result -ne 0 ] && {
      echo -e "\n${ANSI_RED}The command \"$@\" failed. Retrying, $count of 3.${ANSI_RESET}\n" >&2
    }
    "$@"
    result=$?
    [ $result -eq 0 ] && break
    count=$(($count + 1))
    sleep 1
  done

  [ $count -gt 3 ] && {
    echo -e "\n${ANSI_RED}The command \"$@\" failed 3 times.${ANSI_RESET}\n" >&2
  }

  return $result
}

# Build dependencies
travis_retry yum install -y -q autoconf automake bison clang doxygen flex gcc gcc-c++ git kernel-devel ncurses-devel sqlite-devel libtool make mcpp pkg-config python sqlite sudo zlib-devel

# Set up the package builder
travis_retry yum install -y -q ruby-devel gcc make rpm-build libffi-devel
travis_retry gem install --no-ri --no-rdoc fpm

fpm --version
